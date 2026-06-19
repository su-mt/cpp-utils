#include "test_generic.hpp"
#include "rangetree/rangetree.hpp"

// Предполагается, что ваше RangeTree находится в своем namespace, 
// если нет — уберите или замените.
using namespace rangetree;

template <size_t K> using TreeT  = RangeTreeNd<double, K>;
template <size_t K> using QueryT = Range<double, K>;
template <size_t K> using PointT = Point<double, K>;

TEST(RangeTreeTest, Basic3DSearch) {
    run_basic_3d_search<TreeT<3>, QueryT<3>, PointT<3>>();
}

TEST(RangeTreeTest, Specific3DRanges) {
    run_specific_3d_ranges<TreeT<3>, QueryT<3>, PointT<3>>();
}

TEST(RangeTreeTest, Dim1)  { run_dimension_test<1,  TreeT<1>,  QueryT<1>,  PointT<1>>(); }
TEST(RangeTreeTest, Dim2)  { run_dimension_test<2,  TreeT<2>,  QueryT<2>,  PointT<2>>(); }
TEST(RangeTreeTest, Dim3)  { run_dimension_test<3,  TreeT<3>,  QueryT<3>,  PointT<3>>(); }
TEST(RangeTreeTest, Dim4)  { run_dimension_test<4,  TreeT<4>,  QueryT<4>,  PointT<4>>(); }
TEST(RangeTreeTest, Dim5)  { run_dimension_test<5,  TreeT<5>,  QueryT<5>,  PointT<5>>(); }
TEST(RangeTreeTest, Dim6)  { run_dimension_test<6,  TreeT<6>,  QueryT<6>,  PointT<6>>(); }
TEST(RangeTreeTest, Dim7)  { run_dimension_test<7,  TreeT<7>,  QueryT<7>,  PointT<7>>(); }
TEST(RangeTreeTest, Dim8)  { run_dimension_test<8,  TreeT<8>,  QueryT<8>,  PointT<8>>(); }
TEST(RangeTreeTest, Dim9)  { run_dimension_test<9,  TreeT<9>,  QueryT<9>,  PointT<9>>(); }
TEST(RangeTreeTest, Dim10)  { run_dimension_test<10,  TreeT<10>,  QueryT<10>,  PointT<10>>(); }  
