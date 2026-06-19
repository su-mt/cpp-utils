#include "bench.hpp"
#include "rangetree/rangetree.hpp"

namespace {

// Регистрируем бенчмарки для RangeTree
const bool registered = []() {
    bench_utils::RegisterSpatialBenchmarks<
        rangetree::RangeTreeNd<double, 3>, 
        Range<double, 3>, 
        Point<double, 3>
    >("RangeTree");
    return true;
}();

} // namespace
