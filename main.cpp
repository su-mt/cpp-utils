#include <atomic>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <optional>
#include <pthread.h>
#include <ratio>
#include <semaphore.h>
#include <random>
#include <stdatomic.h>
#include <sys/types.h>
#include <thread>
#include <utility>
#include <chrono>
#include <vector>
#include <csignal>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

// ─── Globals ────────────────────────────────────────────────────────────────
// C-style atomic API lives in std:: namespace in C++ (<atomic> header).
std::chrono::steady_clock::time_point program_start;
atomic_int running;

// ─── LogEntry ────────────────────────────────────────────────────────────────
struct LogEntry {
    uint64_t timestamp_ms;
    std::string message;
};

// ─── SPSCQueue (unchanged) ───────────────────────────────────────────────────
template<typename T>
class SPSCQueue {
    struct Node {
        std::optional<T> val;
        std::atomic<Node*> next;
        Node()      : val(std::nullopt), next(nullptr) {}
        Node(T val) : val(std::move(val)), next(nullptr) {}
    };
    Node* tail_, *head_;
public:
    SPSCQueue() {
        Node* dummy = new Node();
        head_ = tail_ = dummy;
    }
    void push(T val) {
        Node* node = new Node(std::move(val));
        tail_->next.store(node, std::memory_order_release);
        tail_ = node;
    }
    std::optional<T> see() {
        Node* next = head_->next.load(std::memory_order_acquire);
        if (next == nullptr) return std::nullopt;
        return next->val;
    }
    T pop() {
        Node* data    = head_->next.load(std::memory_order_acquire);
        Node* old_head = head_;
        T val = std::move(data->val.value());
        head_ = data;
        delete old_head;
        return val;
    }
};

// ─── log_event helper ────────────────────────────────────────────────────────
// Called by every actor; timestamp is captured here, on the producer side.
void log_event(SPSCQueue<LogEntry>* q, const std::string& msg) {
    auto now = std::chrono::steady_clock::now();
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now - program_start).count();
    q->push({ms, std::move(msg)});
}

// ─── CrossRoads (unchanged) ──────────────────────────────────────────────────
class CrossRoads {
public:
    pthread_mutex_t sectors[4];
    CrossRoads() {
        for (auto& m : sectors) pthread_mutex_init(&m, NULL);
    }
    ~CrossRoads() {
        for (auto& m : sectors) pthread_mutex_destroy(&m);
    }
};

// ─── Enums (unchanged) ───────────────────────────────────────────────────────
enum class Direction { Right, Forward, Left, Turn };
enum class Road      { East, North, West, South    };

// ─── Car (unchanged) ─────────────────────────────────────────────────────────
class Car {
    unsigned id;
    Direction direction;
    Road road;
    unsigned char speed;
public:
    Car(unsigned id, Direction direction, Road road, unsigned s)
        : id(id), direction(direction), road(road), speed(s) {}

    auto getDir()   { return direction; }
    auto getRoad()  { return road; }
    auto getSpeed() { return speed; }
    auto getId()    { return id; }

    auto getRoute() const {
        std::vector<int> path;
        int curr  = static_cast<int>(road) + 1;
        int steps = static_cast<int>(direction) + 1;
        for (int i = 0; i < steps; i++) {
            path.push_back(curr);
            curr = (curr + 1) % 4;
        }
        return path;
    }

    // Helper for readable log messages
    std::string describe() const {
        static const char* dirs[] = {"Right","Forward","Left","Turn"};
        static const char* rds[]  = {"East","North","West","South"};
        std::ostringstream ss;
        ss << "Car#" << id
           << " road=" << rds[static_cast<int>(road)]
           << " dir="  << dirs[static_cast<int>(direction)]
           << " speed=" << static_cast<int>(speed);
        return ss.str();
    }
};

// ─── Logger ──────────────────────────────────────────────────────────────────
// Owns the log file. Reads all queues round-robin and writes directly.
// On shutdown: does a final drain, then sorts the whole file by timestamp.
class Logger {
    static constexpr int NUM_QUEUES = 6;
    SPSCQueue<LogEntry>* queues;   // logs[0..5]
    std::string          filename;
    pthread_t            thread;

    // Write one entry to the file
    static void write_entry(std::FILE* f, const LogEntry& e) {
        std::fprintf(f, "[%08llu] %s\n",
                     (unsigned long long)e.timestamp_ms,
                     e.message.c_str());
    }

    // Drain all queues to file; returns number of entries written
    int drain(std::FILE* f) {
        int count = 0;
        for (int i = 0; i < NUM_QUEUES; i++) {
            while (queues[i].see().has_value()) {
                write_entry(f, queues[i].pop());
                count++;
            }
        }
        return count;
    }

    // Re-read file, sort by timestamp, rewrite
    void sort_file() {
        std::ifstream in(filename);
        if (!in.is_open()) return;

        std::vector<std::pair<uint64_t, std::string>> lines;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            uint64_t ts = 0;
            // format: [00012345] message
            if (line[0] == '[') {
                auto end = line.find(']');
                if (end != std::string::npos)
                    ts = std::stoull(line.substr(1, end - 1));
            }
            lines.push_back({ts, line});
        }
        in.close();

        std::stable_sort(lines.begin(), lines.end(),
                         [](const auto& a, const auto& b){
                             return a.first < b.first;
                         });

        std::ofstream out(filename, std::ios::trunc);
        for (auto& [ts, l] : lines)
            out << l << '\n';

        std::cout << "[Logger] File sorted. Total entries: "
                  << lines.size() << '\n';
    }

    void loop() {
        std::FILE* f = std::fopen(filename.c_str(), "w");
        if (!f) {
            std::cerr << "[Logger] Cannot open log file!\n";
            return;
        }

        while (atomic_load(&running)) {
            drain(f);
            std::fflush(f);
            // small sleep to avoid busy-spin when queues are empty
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Final drain — producers have already stopped
        drain(f);
        std::fflush(f);
        std::fclose(f);

        sort_file();
    }

    static void* start_routine(void* arg) {
        static_cast<Logger*>(arg)->loop();
        return nullptr;
    }

public:
    Logger(SPSCQueue<LogEntry>* qs, std::string fname = "crossroads.log")
        : queues(qs), filename(std::move(fname)), thread(0) {}

    void start() {
        pthread_create(&thread, nullptr, start_routine, this);
    }
    void join() {
        pthread_join(thread, nullptr);
    }
};

// ─── Generator ───────────────────────────────────────────────────────────────
class Generator {
    std::mt19937 mt;
    std::uniform_int_distribution<> speed_distrib;
    std::uniform_int_distribution<> distrib;
    unsigned id = 0;
    SPSCQueue<Car>*     queues;
    SPSCQueue<LogEntry>* log_q;  // logs[0]
public:
    Generator(SPSCQueue<Car>* q, SPSCQueue<LogEntry>* lq,
              unsigned low = 1, unsigned up = 5)
        : mt(std::random_device{}()),
          speed_distrib(low, up), distrib(0, 3),
          queues(q), log_q(lq) {}

    Car createCar() {
        Direction d = static_cast<Direction>(distrib(mt));
        Road      r = static_cast<Road>(distrib(mt));
        unsigned  s = speed_distrib(mt);
        return Car(id++, d, r, s);
    }

    void pushToQueue(Car car) {
        log_event(log_q, "Created " + car.describe());
        queues[static_cast<int>(car.getRoad())].push(std::move(car));
    }

    void p() {
        while (atomic_load(&running)) {
            pushToQueue(createCar());
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }

    static void* start_routine(void* arg) {
        static_cast<Generator*>(arg)->p();
        return nullptr;
    }
};

// ─── Worker ───────────────────────────────────────────────────────────────────
class Worker {
    struct WorkerArgs {
        Worker*             worker;
        std::vector<int>    route;
        Car                 car;
        CrossRoads*         xroad;
        SPSCQueue<LogEntry>* log_q;
    };

    pthread_t            thread;
    CrossRoads*          xroad;
    SPSCQueue<LogEntry>* log_q;  // logs[2..5]

    static void* start_routine(void* arg) {
        WorkerArgs* args = static_cast<WorkerArgs*>(arg);
        args->worker->go(args->route, args->car, *args->xroad, args->log_q);
        delete args;
        return nullptr;
    }

    void go(std::vector<int>& route, Car& car,
            CrossRoads& xroad, SPSCQueue<LogEntry>* lq) {

        auto delay = [&]() {
            std::this_thread::sleep_for(std::chrono::seconds(car.getSpeed()));
        };

        // First sector was already locked by tryProcess
        delay();
        log_event(lq, "Car#" + std::to_string(car.getId())
                      + " left sector " + std::to_string(route[0]));
        pthread_mutex_unlock(&xroad.sectors[route[0]]);

        for (int i = 1; i < (int)route.size(); i++) {
            pthread_mutex_lock(&xroad.sectors[route[i]]);
            log_event(lq, "Car#" + std::to_string(car.getId())
                          + " entered sector " + std::to_string(route[i]));
            delay();
            log_event(lq, "Car#" + std::to_string(car.getId())
                          + " left sector " + std::to_string(route[i]));
            pthread_mutex_unlock(&xroad.sectors[route[i]]);
        }
    }

public:
    Worker(CrossRoads* xr, SPSCQueue<LogEntry>* lq)
        : thread(0), xroad(xr), log_q(lq) {}

    bool tryProcess(const std::vector<int>& route, const Car& car) {
        if (pthread_mutex_trylock(&xroad->sectors[route[0]]) == 0) {
            if (route.size() == 1) {
                WorkerArgs* args = new WorkerArgs{this, route, car, xroad, log_q};
                pthread_create(&thread, nullptr, start_routine, args);
                return true;
            }
            if (pthread_mutex_trylock(&xroad->sectors[route[1]]) == 0) {
                WorkerArgs* args = new WorkerArgs{this, route, car, xroad, log_q};
                pthread_create(&thread, nullptr, start_routine, args);
                pthread_mutex_unlock(&xroad->sectors[route[1]]);
                return true;
            } else {
                pthread_mutex_unlock(&xroad->sectors[route[0]]);
            }
        }
        return false;
    }

    void join() {
        if (thread != 0) pthread_join(thread, nullptr);
    }
};

// ─── Scheduler ───────────────────────────────────────────────────────────────
class Scheduler {
    void handler(SPSCQueue<Car>* queues, Worker* workers, SPSCQueue<LogEntry>* log_q) {

        for (Road r = Road::East; atomic_load(&running) != 0; r = static_cast<Road>((static_cast<int>(r) + 1) % 4)) {
            auto cand = queues[static_cast<int>(r)].see();
            if (cand.has_value()) {
                auto route = cand->getRoute();

                // Build route string for log
                std::ostringstream rs;
                rs << "[";
                for (int i = 0; i < (int)route.size(); i++) {
                    if (i) rs << ",";
                    rs << route[i];
                }
                rs << "]";

                log_event(log_q, "Car#" + std::to_string(cand->getId())
                                 + " attempting route " + rs.str());

                if (workers[static_cast<int>(r)].tryProcess(route, cand.value())) {
                    log_event(log_q, "Car#" + std::to_string(cand->getId())
                                     + " accepted");
                    queues[static_cast<int>(r)].pop();
                } else {
                    log_event(log_q, "Car#" + std::to_string(cand->getId())
                                     + " rejected");
                }
            }

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

public:
    struct SchArgs {
        Scheduler*          sch;
        SPSCQueue<Car>*     queues;
        Worker*             workers;
        SPSCQueue<LogEntry>* log_q;
    };

    static void* start_routine(void* arg) {
        SchArgs* args = static_cast<SchArgs*>(arg);
        args->sch->handler(args->queues, args->workers, args->log_q);
        delete args;
        return nullptr;
    }
};

// ─── SIGINT handler ──────────────────────────────────────────────────────────
void sigint_handler(int) {
    atomic_store(&running, 0);
}

// ─── main ────────────────────────────────────────────────────────────────────
int main() {
    // Init globals
    program_start = std::chrono::steady_clock::now();
    atomic_init(&running, 1);

    // Register SIGINT
    struct sigaction sa{};
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, nullptr);

    // Queues
    SPSCQueue<Car>     queues[4];
    SPSCQueue<LogEntry> logs[6];
    // logs[0] → Generator
    // logs[1] → Scheduler
    // logs[2..5] → Worker[0..3]

    CrossRoads xroad;

    Worker workers[4] = {
        Worker(&xroad, &logs[2]),
        Worker(&xroad, &logs[3]),
        Worker(&xroad, &logs[4]),
        Worker(&xroad, &logs[5]),
    };

    Generator gen(queues, &logs[0]);

    Logger logger(logs);
    logger.start();

    pthread_t thread_generator, thread_sch;
    pthread_create(&thread_generator, nullptr, Generator::start_routine, &gen);

    Scheduler sch;
    Scheduler::SchArgs* sch_args =
        new Scheduler::SchArgs{&sch, queues, workers, &logs[1]};
    pthread_create(&thread_sch, nullptr, Scheduler::start_routine, sch_args);

    // Wait for Ctrl+C → running = 0
    pthread_join(thread_generator, nullptr);
    pthread_join(thread_sch, nullptr);
    for (auto& w : workers) w.join();

    // Logger last — guarantees all entries are flushed and sorted
    logger.join();

    std::cout << "[main] Done. See crossroads.log\n";
    return 0;
}