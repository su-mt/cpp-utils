#pragma once
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

namespace bench_utils {

constexpr double X_MIN = 400.0, X_MAX = 600.0;
constexpr double Y_MIN = -60.0, Y_MAX = 60.0;
constexpr double Z_MIN = 0.0,   Z_MAX = 45.0;
constexpr int    DEFAULT_N = 10000;

template <typename PointType>
std::vector<PointType> make_points(int n, unsigned seed = 1337) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dx(X_MIN, X_MAX);
    std::uniform_real_distribution<double> dy(Y_MIN, Y_MAX);
    std::uniform_real_distribution<double> dz(Z_MIN, Z_MAX);

    std::vector<PointType> pts(static_cast<size_t>(n));
    for (auto& p : pts) {
        p[0] = dx(gen);
        p[1] = dy(gen);
        p[2] = dz(gen);
    }
    return pts;
}

template <typename QueryType, typename PointType>
QueryType make_query(double frac) {
    auto half = [frac](double lo, double hi) {
        double c = (lo + hi) / 2.0;
        double h = (hi - lo) * frac / 2.0;
        return std::pair<double, double>{c - h, c + h};
    };
    auto [x1, x2] = half(X_MIN, X_MAX);
    auto [y1, y2] = half(Y_MIN, Y_MAX);
    auto [z1, z2] = half(Z_MIN, Z_MAX);

    QueryType r;
    r.start = PointType{{x1, y1, z1}};
    r.end   = PointType{{x2, y2, z2}};
    return r;
}

// Дерево на DEFAULT_N точек строится один раз для КАЖДОГО типа дерева
template <typename TreeType, typename PointType>
TreeType& shared_tree() {
    static std::vector<PointType> points = make_points<PointType>(DEFAULT_N);
    static TreeType tree(points);
    return tree;
}

// ============================================================
// ШАБЛОНЫ БЕНЧМАРКОВ
// ============================================================

template <typename TreeType, typename PointType>
static void BM_Build(benchmark::State& state) {
    const int n = static_cast<int>(state.range(0));
    auto points = make_points<PointType>(n);

    for (auto _ : state) {
        std::vector<PointType> copy = points;
        TreeType tree(copy);
        benchmark::DoNotOptimize(tree);
    }

    state.SetComplexityN(n);
    state.counters["n_points"] = n;
}

template <typename TreeType, typename QueryType, typename PointType>
static void BM_Search(benchmark::State& state) {
    const double frac = state.range(0) / 100.0;
    QueryType query = make_query<QueryType, PointType>(frac);
    
    TreeType& tree = shared_tree<TreeType, PointType>();

    size_t found = 0;
    for (auto _ : state) {
        auto result = tree.search(query);
        found = result.size();
        benchmark::DoNotOptimize(result);
    }

    state.counters["found_points"] = static_cast<double>(found);
    state.counters["query_pct"] = state.range(0);
}


// ============================================================
// 1. Зависимость времени поиска от количества точек (Vary N)
// ============================================================
template <typename TreeType, typename QueryType, typename PointType>
static void BM_Search_VaryN(benchmark::State& state) {
    const int n = static_cast<int>(state.range(0));
    const double frac = 0.10; // Фиксируем размер запроса (например, 10% от всего пространства)

    // Подготовка данных (не входит в замер времени)
    auto points = make_points<PointType>(n);
    TreeType tree(points);
    QueryType query = make_query<QueryType, PointType>(frac);

    size_t found = 0;
    for (auto _ : state) {
        auto result = tree.search(query);
        found = result.size();
        benchmark::DoNotOptimize(result);
    }

    state.SetComplexityN(state.range(0));
    state.counters["n_points"] = n;
    state.counters["found_points"] = static_cast<double>(found);
}

// ============================================================
// 2. Зависимость времени поиска от размера диапазона (Vary Range)
// ============================================================
template <typename TreeType, typename QueryType, typename PointType>
static void BM_Search_VaryRange(benchmark::State& state) {
    const int n = 100000; // Фиксируем размер дерева (например, 100k точек)
    const double frac = state.range(0) / 100.0;

    // Подготовка данных (не входит в замер времени)
    auto points = make_points<PointType>(n);
    TreeType tree(points);
    QueryType query = make_query<QueryType, PointType>(frac);

    size_t found = 0;
    for (auto _ : state) {
        auto result = tree.search(query);
        found = result.size();
        benchmark::DoNotOptimize(result);
    }

    state.counters["query_pct"] = state.range(0);
    state.counters["found_points"] = static_cast<double>(found);
}

// ============================================================
// ФУНКЦИЯ ДИНАМИЧЕСКОЙ РЕГИСТРАЦИИ
// ============================================================

template <typename TreeType, typename QueryType, typename PointType>
void RegisterSpatialBenchmarks(const std::string& name_prefix) {
    // Регистрируем бенчмарк сборки


    benchmark::RegisterBenchmark((name_prefix + "_Build").c_str(), BM_Build<TreeType, PointType>)
        ->Arg(1000)
        ->Arg(5000)
        ->Arg(10000)
        ->Arg(50000)
        ->Arg(100000)
        ->Unit(benchmark::kMillisecond)
        ->Complexity(benchmark::oNLogN);

    // Регистрируем бенчмарк поиска
    benchmark::RegisterBenchmark((name_prefix + "_Search").c_str(), BM_Search<TreeType, QueryType, PointType>)
        ->Arg(1)
        ->Arg(5)
        ->Arg(10)
        ->Arg(25)
        ->Arg(50)
        ->Arg(75)
        ->Arg(100)
        ->Unit(benchmark::kMicrosecond);


    benchmark::RegisterBenchmark((name_prefix + "_Search_VaryN").c_str(), BM_Search_VaryN<TreeType, QueryType, PointType>)
        ->RangeMultiplier(2)->Range(1024, 1024 * 1024) // От ~1k до ~1M точек
        ->Unit(benchmark::kMicrosecond)
        ->Complexity(benchmark::oLogN); // Ожидаемая сложность поиска в дереве

    benchmark::RegisterBenchmark((name_prefix + "_Search_VaryRange").c_str(), BM_Search_VaryRange<TreeType, QueryType, PointType>)
        ->DenseRange(5, 100, 5) // Шаг 5%: 5, 10, 15... 100
        ->Unit(benchmark::kMicrosecond);

}





} // namespace bench_utils