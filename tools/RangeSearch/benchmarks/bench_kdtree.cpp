#include "bench.hpp"
#include "kdtree/kdtree.hpp"

namespace {

// Регистрируем бенчмарки для KdTree при загрузке единицы трансляции
const bool registered = []() {
    bench_utils::RegisterSpatialBenchmarks<
        kdtree::KdTree<double, 3>, 
        kdtree::Region<double, 3>, 
        Point<double, 3>
    >("KdTree");
    return true;
}();

} // namespace

