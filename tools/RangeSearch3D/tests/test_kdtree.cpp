#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include "RangeSearch.hpp"

#include "kdtree/kdtree.hpp"

using namespace kdtree;

// ==============================================================================
// 1. БАЗОВЫЙ ТЕСТ ИЗ ПРИМЕРА (Санти-чек)
// ==============================================================================
TEST(KdTreeTest, Basic3DSearch) {
    std::vector<Point<double, 3>> points{
        {{10, 10, 10}},
        {{20, 20, 20}},
        {{30, 30, 30}},
        {{40, 40, 40}}
    };

    KdTree<double, 3> kd(points);

    // Внимание: чтобы синтаксис {{15,15,15}, {35,35,35}} работал, 
    // у структуры Region должен быть конструктор: Region(Point start, Point end)
    // Либо заполняем вручную:
    Region <double,3> query;
    query.start = {{15, 15, 15}};
    query.end   = {{35, 35, 35}};

    auto ans = kd.search(query);

    // Сортируем для корректного сравнения
    std::sort(ans.begin(), ans.end());

    std::vector<Point<double, 3>> expected{
        {{20, 20, 20}},
        {{30, 30, 30}}
    };
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(ans, expected);
}


// ==============================================================================
// 2. ОРАКУЛ И ГЕНЕРАТОРЫ (Для случайных тестов)
// ==============================================================================

// Тупой, но 100% верный линейный поиск для проверки kd-дерева
template <typename T, size_t K>
std::vector<Point<T, K>> brute_force_search(const std::vector<Point<T, K>>& points, const Region<T,K>& reg) {
    std::vector<Point<T, K>> result;
    for (const auto& p : points) {
        if (reg.contains(p)) {
            result.push_back(p);
        }
    }
    return result;
}

// Генератор случайных точек
template <size_t K>
std::vector<Point<double, K>> generate_random_points(int count, const std::array<double, K>& mins, const std::array<double, K>& maxs, int seed = 42) {
    std::mt19937 gen(seed);
    std::vector<std::uniform_real_distribution<double>> dists;
    for (size_t i = 0; i < K; ++i) {
        dists.emplace_back(mins[i], maxs[i]);
    }

    std::vector<Point<double, K>> points(count);
    for (int i = 0; i < count; ++i) {
        for (size_t d = 0; d < K; ++d) {
            points[i][d] = dists[d](gen);
        }
    }
    return points;
}


// ==============================================================================
// 3. СПЕЦИФИЧНЫЙ ТЕСТ 3D (x: 400-600, y: -60-60, z: 0-45)
// ==============================================================================
TEST(KdTreeTest, Specific3DRanges) {
    const int num_points = 10000;
    std::array<double, 3> mins = {400.0, -60.0, 0.0};
    std::array<double, 3> maxs = {600.0, 60.0, 45.0};

    auto points = generate_random_points<3>(num_points, mins, maxs, 1337);
    KdTree<double, 3> tree(points);

    // Сделаем 50 случайных запросов-параллелепипедов внутри этой области
    std::mt19937 gen(101);
    std::uniform_real_distribution<double> dist_x(400.0, 600.0);
    std::uniform_real_distribution<double> dist_y(-60.0, 60.0);
    std::uniform_real_distribution<double> dist_z(0.0, 45.0);

    for (int i = 0; i < 50; ++i) {
        Region<double,3> query;
        // Гарантируем, что start <= end
        double x1 = dist_x(gen), x2 = dist_x(gen);
        double y1 = dist_y(gen), y2 = dist_y(gen);
        double z1 = dist_z(gen), z2 = dist_z(gen);

        query.start = {{std::min(x1, x2), std::min(y1, y2), std::min(z1, z2)}};
        query.end   = {{std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)}};

        auto tree_result = tree.search(query);
        auto brute_result = brute_force_search(points, query);

        std::sort(tree_result.begin(), tree_result.end());
        std::sort(brute_result.begin(), brute_result.end());

        ASSERT_EQ(tree_result.size(), brute_result.size()) << "Mismatch in sizes on iter " << i;
        EXPECT_EQ(tree_result, brute_result) << "Mismatch in points on iter " << i;
    }
}


// ==============================================================================
// 4. ТЕСТИРОВАНИЕ РАЗМЕРНОСТЕЙ 1..10
// ==============================================================================

template <size_t K>
void run_dimension_test() {
    const int num_points = 1000;
    std::array<double, K> mins; mins.fill(-100.0);
    std::array<double, K> maxs; maxs.fill(100.0);

    auto points = generate_random_points<K>(num_points, mins, maxs);
    KdTree<double, K> tree(points);

    std::mt19937 gen(K); // Seed зависит от размерности
    std::uniform_real_distribution<double> dist(-100.0, 100.0);

    for (int i = 0; i < 20; ++i) {
        Region<double, K> query;
        for (size_t d = 0; d < K; ++d) {
            double v1 = dist(gen);
            double v2 = dist(gen);
            query.start[d] = std::min(v1, v2);
            query.end[d] = std::max(v1, v2);
        }

        auto tree_result = tree.search(query);
        auto brute_result = brute_force_search(points, query);

        std::sort(tree_result.begin(), tree_result.end());
        std::sort(brute_result.begin(), brute_result.end());

        EXPECT_EQ(tree_result.size(), brute_result.size());
        EXPECT_EQ(tree_result, brute_result);
    }
}


TEST(KdTreeTest, Dim1)  { run_dimension_test<1>(); }
TEST(KdTreeTest, Dim2)  { run_dimension_test<2>(); }
TEST(KdTreeTest, Dim3)  { run_dimension_test<3>(); }
TEST(KdTreeTest, Dim4)  { run_dimension_test<4>(); }
TEST(KdTreeTest, Dim5)  { run_dimension_test<5>(); }
TEST(KdTreeTest, Dim6)  { run_dimension_test<6>(); }
TEST(KdTreeTest, Dim7)  { run_dimension_test<7>(); }
TEST(KdTreeTest, Dim8)  { run_dimension_test<8>(); }
TEST(KdTreeTest, Dim9)  { run_dimension_test<9>(); }
TEST(KdTreeTest, Dim10) { run_dimension_test<10>(); }


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}