#include "bench.hpp"
#include "RangeSearch.hpp"
#include "kdtree/kdtree.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

// 1. Определяем типы Boost.Geometry
using BoostPoint = bg::model::point<double, 3, bg::cs::cartesian>;
using BoostBox   = bg::model::box<BoostPoint>;

// 2. Пишем обертку-адаптер для R* дерева
template <typename PointT, typename QueryT>
class BoostRTreeWrapper {
public:
    // Конструктор принимает наш вектор точек и строит R* дерево
    BoostRTreeWrapper(const std::vector<PointT>& points) {
        // Конвертируем наши точки в Boost-точки.
        // Накладные расходы на этот цикл O(N) ничтожны по сравнению с 
        // O(N log N) балансировкой самого R* дерева при вставке.
        std::vector<BoostPoint> b_points;
        b_points.reserve(points.size());
        for (const auto& p : points) {
            b_points.emplace_back(p[0], p[1], p[2]);
        }
        
        // Передача вектора в конструктор запускает алгоритм "Bulk loading" (packing).
        // Это строит дерево сразу целиком, что в разы быстрее последовательных .insert()
        // и дает максимально сбалансированное R* дерево. Максимум 16 элементов в узле.
        tree_ = bgi::rtree<BoostPoint, bgi::rstar<16>>(b_points);
    }

    // Метод поиска, который ждет bench_common.hpp
    std::vector<PointT> search(const QueryT& query) const {
        // Конвертируем наш Region в BoostBox
        BoostBox box(
            BoostPoint(query.start[0], query.start[1], query.start[2]),
            BoostPoint(query.end[0],   query.end[1],   query.end[2])
        );

        // Ищем пересечения
        std::vector<BoostPoint> b_result;
        tree_.query(bgi::intersects(box), std::back_inserter(b_result));

        // Конвертируем результат обратно в наш формат
        std::vector<PointT> result;
        result.reserve(b_result.size());
        for (const auto& bp : b_result) {
            result.push_back(PointT{{bg::get<0>(bp), bg::get<1>(bp), bg::get<2>(bp)}});
        }
        return result;
    }

private:
    bgi::rtree<BoostPoint, bgi::rstar<16>> tree_;
};

namespace {

// 3. Используем наши алиасы типов из kdtree как транспортные типы
using PointT = Point<double, 3>;
using QueryT = kdtree::Region<double, 3>;
using TreeT  = BoostRTreeWrapper<PointT, QueryT>;

// 4. Регистрируем бенчмарк
const bool registered = []() {
    bench_utils::RegisterSpatialBenchmarks<TreeT, QueryT, PointT>("Boost_RStarTree");
    return true;
}();

} // namespace