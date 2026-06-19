// bench/benchmark_kdtree.cpp
#include <benchmark/benchmark.h>
#include "kdtree/kdtree.hpp"
#include <random>
#include <vector>

using namespace kdtree;

namespace {

constexpr double X_MIN = 400.0, X_MAX = 600.0;
constexpr double Y_MIN = -60.0, Y_MAX = 60.0;
constexpr double Z_MIN = 0.0,   Z_MAX = 45.0;
constexpr int    DEFAULT_N = 10000;

std::vector<Point<double, 3>> make_points(int n, unsigned seed = 1337) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dx(X_MIN, X_MAX);
    std::uniform_real_distribution<double> dy(Y_MIN, Y_MAX);
    std::uniform_real_distribution<double> dz(Z_MIN, Z_MAX);

    std::vector<Point<double, 3>> pts(static_cast<size_t>(n));
    for (auto& p : pts) {
        p[0] = dx(gen);
        p[1] = dy(gen);
        p[2] = dz(gen);
    }
    return pts;
}

// Строит запрос-регион как долю (frac) каждой стороны домена,
// центрированный по центру домена. frac = 1.0 -> весь домен.
Region<double, 3> make_query(double frac) {
    auto half = [frac](double lo, double hi) {
        double c = (lo + hi) / 2.0;
        double h = (hi - lo) * frac / 2.0;
        return std::pair<double, double>{c - h, c + h};
    };
    auto [x1, x2] = half(X_MIN, X_MAX);
    auto [y1, y2] = half(Y_MIN, Y_MAX);
    auto [z1, z2] = half(Z_MIN, Z_MAX);

    Region<double, 3> r;
    r.start = Point<double, 3>{{x1, y1, z1}};
    r.end   = Point<double, 3>{{x2, y2, z2}};
    return r;
}

// Дерево на DEFAULT_N точек строится один раз (lazy static)
// и переиспользуется во всех бенчмарках поиска.
KdTree<double, 3>& shared_tree() {
    static std::vector<Point<double, 3>> points = make_points(DEFAULT_N);
    static KdTree<double, 3> tree(points);
    return tree;
}

} // namespace

// ============================================================
// 1. Построение дерева: время build() от количества точек N
// ============================================================
static void BM_Build(benchmark::State& state) {
    const int n = static_cast<int>(state.range(0));
    auto points = make_points(n);

    for (auto _ : state) {
        // build() мутирует вектор (nth_element), копируем на каждой итерации
        std::vector<Point<double, 3>> copy = points;
        KdTree<double, 3> tree(copy);
        benchmark::DoNotOptimize(tree);
    }

    state.SetComplexityN(n);
    state.counters["n_points"] = n;
}
BENCHMARK(BM_Build)
    ->Arg(1000)
    ->Arg(5000)
    ->Arg(10000)
    ->Arg(50000)
    ->Arg(100000)
    ->Unit(benchmark::kMillisecond)
    ->Complexity(benchmark::oNLogN);

// ============================================================
// 2. Поиск: фиксированное дерево (10000 точек),
//    переменный размер диапазона запроса (% от стороны домена)
// ============================================================
static void BM_Search(benchmark::State& state) {
    const double frac = state.range(0) / 100.0;
    Region<double, 3> query = make_query(frac);
    KdTree<double, 3>& tree = shared_tree();

    size_t found = 0;
    for (auto _ : state) {
        auto result = tree.search(query);
        found = result.size();
        benchmark::DoNotOptimize(result);
    }

    state.counters["found_points"] = static_cast<double>(found);
    state.counters["query_pct"] = state.range(0);
}
BENCHMARK(BM_Search)
    ->Arg(1)    // узкий запрос: 1% от каждой стороны домена
    ->Arg(5)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(75)
    ->Arg(100)  // весь домен
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();