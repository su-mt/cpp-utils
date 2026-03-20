#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <random>
#include <stdatomic.h>
#include <sys/types.h>
#include <thread>
#include <utility>
#include <chrono>
#include <vector>



class CrossRoads {
public: 
    pthread_mutex_t sectors[4];


	CrossRoads() {
		for (auto& m: sectors) {
			pthread_mutex_init(&m, NULL);
		}
	}
	~CrossRoads() {
		for (auto& m : sectors) {
			pthread_mutex_destroy(&m);
		}
	}		

};
 
enum class Direction {
    Right, Forward, Left, Turn

};

enum class Road {
	East, North,West,South
};
class Car {
    unsigned id;
    Direction direction;
    Road road;
    unsigned char speed;
public:
    Car(unsigned id, Direction direction, Road road, unsigned s) : \
     id(id), direction(direction), road(road), speed(s) { };

    auto getDir() {
        return direction;
    }
    auto getRoad () {
        return road;
    }
    auto getSpeed () {
        return speed;
    }
    auto getId() {
        return id;
    }
	auto getRoute() const {
		std::vector<int> path;
		int curr = static_cast<int>(road) + 1;
		int steps = static_cast<int>(direction)+1;
		for(int i = 0; i < steps ; i++) {
			path.push_back(curr);

			curr =(curr+1) % 4;
		}
		return path;
	}
};
 
//dummy queue
class SPSCQueue {  
    struct Node {
        std::optional<Car> car;
        std::atomic<Node*> next;
        Node () : car(std::nullopt), next(nullptr) { };
        Node (Car car) : car(std::move (car)) ,next(nullptr) { };
    };

    Node* tail_, *head_;
public:
    SPSCQueue()  { 
        Node* dummy = new Node();
        head_ = tail_ = dummy;
    }

    void push (Car car) {
        Node* node= new Node(std::move(car));
        tail_->next.store(node,std::memory_order_release);
        tail_=node;
    }
    std::optional<Car> see() {
        Node* next = head_->next.load(std::memory_order_acquire);
        if (next == nullptr) {
            return std::nullopt;  
        }

        return next->car;                          
    }

    Car pop () {
        Node* data = head_->next.load(std::memory_order_acquire);
        Node* old_head = head_;
        Car car = std::move(data->car.value());
        head_ = data;
        delete old_head;
        return car;
    }


};

class Generator {

    std::mt19937 mt;
    std::uniform_int_distribution<> speed_distrib;
    std::uniform_int_distribution<> distrib ;
    unsigned id = 0;
    SPSCQueue* queues;
public:
    Generator (SPSCQueue* q, unsigned low_speed_bound = 1,unsigned up_speed_bound = 5) : \
     mt(std::random_device{}()), speed_distrib(low_speed_bound,up_speed_bound), \
     distrib(0,3), \
     queues (q) { }

    Car createCar () {
        Direction d = static_cast<Direction>(distrib(mt));
        Road r = static_cast<Road>(distrib(mt));
        unsigned s = speed_distrib(mt);
        return Car(id++, d,r,s) ;
    }

    void pushToQueue (Car car) {
		queues[static_cast<int>(car.getRoad())].push(std::move(car));
    }


    void p () {
		std::optional<Car> nc,ec,wc,sc;
		for (;;) {

			pushToQueue(createCar());
			// nc = queues[static_cast<int>(Road::North)].see();
			// wc = queues[static_cast<int>(Road::West)].see();
			// sc = queues[static_cast<int>(Road::South)].see();
			// ec = queues[static_cast<int>(Road::East)].see();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
    }
	
    static void* start_routine(void* arg) {
        static_cast<Generator*>(arg)->p();
        return nullptr;
    }
};

class Worker {
	struct WorkerArgs {
		Worker* worker;
		std::vector<int> route;
		Car car;
		CrossRoads* xroad;
	};
	pthread_t worker;
	CrossRoads* xroad;

	static void* start_routine(void* arg) {
		WorkerArgs*args = static_cast<WorkerArgs* >(arg);
		args->worker->go(args->route, args->car, *args->xroad);
		delete args;
		return nullptr;
	}

	void go (std::vector<int>& route, Car& car, CrossRoads& xroad) {
		auto delay = [&]() {std::this_thread::sleep_for(std::chrono::seconds(car.getSpeed()));};
		delay();
		pthread_mutex_unlock(&xroad.sectors[route[0]]);
		for(int i = 1; i < route.size(); i++) {
			pthread_mutex_lock(&xroad.sectors[route[i]]);
			delay();
			pthread_mutex_unlock(&xroad.sectors[route[i]]);	
		}
	}


public: //shuld be protected and friend with sch
    Worker(CrossRoads* xr) : xroad(xr), worker(0) {}


	bool tryProcess (const std::vector<int>& route, const Car& car) {
		if (pthread_mutex_trylock(&xroad->sectors[route[0]])==0){
			if (route.size()==1){
				WorkerArgs* args = new WorkerArgs{this, route, car, xroad};
				pthread_create(&worker, nullptr,start_routine, args);
				return true;
			}

			if (pthread_mutex_trylock(&xroad->sectors[route[1]])==0) {
				WorkerArgs* args = new WorkerArgs{this, route, car, xroad};
				pthread_create(&worker, nullptr,start_routine, args);
				pthread_mutex_unlock(&xroad->sectors[route[1]]);
				return true;
			} else {
				pthread_mutex_unlock(&xroad->sectors[route[0]]);
			}

		}

	
		return false;
	}
};

class Scheduler {


	void handler (SPSCQueue* queues, Worker* workers) {
		for(Road r = Road::East; ; r = static_cast<Road>((static_cast<int>(r) + 1) % 4)) {
			auto cand = queues[static_cast<int>(r)].see(); 
			if (cand.has_value()){
				if (workers[static_cast<int>(r)].tryProcess(cand->getRoute(),cand.value())){
					auto car = queues[static_cast<int>(r)].pop();
				}
			
			}	
		}
	}

public:
struct SchArgs {
		Scheduler* sch;
		SPSCQueue* queues;
		Worker* workers	;
	};

	static void* start_routine (void* arg) {

		SchArgs* args = static_cast<SchArgs*> (arg);
		args->sch->handler(args->queues, args->workers);
		delete args;

        return nullptr;
	}
};




int main () {
	Scheduler sch;
    SPSCQueue queues[4];
    Generator gen(queues);
    pthread_t thread_generator, thread_sch;
	CrossRoads xroad;
	Worker workers[4] = {
  		Worker(&xroad), Worker(&xroad), Worker(&xroad), Worker(&xroad)
	};
    Scheduler::SchArgs* sch_args = new Scheduler::SchArgs{&sch, queues, workers};

    pthread_create(&thread_generator, nullptr, Generator::start_routine, &gen);

	pthread_create(&thread_sch, nullptr, Scheduler::start_routine, sch_args);


    pthread_join(thread_generator, nullptr);
    pthread_join(thread_sch, nullptr);

    return 0;
}
