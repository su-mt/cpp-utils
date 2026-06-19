#pragma once

#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <algorithm>
#include <array>



template <typename PointType, typename QueryType>
std::vector<PointType> brute_force_search(const std::vector<PointType>& points, const QueryType& reg) {
    std::vector<PointType> result;
    for (const auto& p : points) {
        if (reg.contains(p)) {
            result.push_back(p);
        }
    }
    return result;
}

template <size_t K, typename PointType>
std::vector<PointType> generate_random_points(int count, const std::array<double, K>& mins, const std::array<double, K>& maxs, int seed = 42) {
    std::mt19937 gen(seed);
    std::vector<std::uniform_real_distribution<double>> dists;
    for (size_t i = 0; i < K; ++i) {
        dists.emplace_back(mins[i], maxs[i]);
    }

    std::vector<PointType> points(count);
    for (int i = 0; i < count; ++i) {
        for (size_t d = 0; d < K; ++d) {
            points[i][d] = dists[d](gen);
        }
    }
    return points;
}

// ==============================================================================
// ШАБЛОНЫ ТЕСТОВ
// ==============================================================================

template <typename TreeType, typename QueryType, typename PointType>
void run_basic_3d_search() {
    std::vector<PointType> points{
        {{10, 10, 10}},
        {{20, 20, 20}},
        {{30, 30, 30}},
        {{40, 40, 40}}
    };

    TreeType tree(points);

    QueryType query;
    query.start = {{15, 15, 15}};
    query.end   = {{35, 35, 35}};

    auto ans = tree.search(query);
    std::sort(ans.begin(), ans.end());

    std::vector<PointType> expected{
        {{20, 20, 20}},
        {{30, 30, 30}}
    };
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(ans, expected);
}

template <typename TreeType, typename QueryType, typename PointType>
void run_specific_3d_ranges() {
    const int num_points = 10000;
    std::array<double, 3> mins = {400.0, -60.0, 0.0};
    std::array<double, 3> maxs = {600.0, 60.0, 45.0};

    auto points = generate_random_points<3, PointType>(num_points, mins, maxs, 1337);
    TreeType tree(points);

    std::mt19937 gen(101);
    std::uniform_real_distribution<double> dist_x(400.0, 600.0);
    std::uniform_real_distribution<double> dist_y(-60.0, 60.0);
    std::uniform_real_distribution<double> dist_z(0.0, 45.0);

    for (int i = 0; i < 50; ++i) {
        QueryType query;
        double x1 = dist_x(gen), x2 = dist_x(gen);
        double y1 = dist_y(gen), y2 = dist_y(gen);
        double z1 = dist_z(gen), z2 = dist_z(gen);

        query.start = {{std::min(x1, x2), std::min(y1, y2), std::min(z1, z2)}};
        query.end   = {{std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)}};

        auto tree_result = tree.search(query);
        auto brute_result = brute_force_search<PointType, QueryType>(points, query);

        std::sort(tree_result.begin(), tree_result.end());
        std::sort(brute_result.begin(), brute_result.end());

        ASSERT_EQ(tree_result.size(), brute_result.size()) << "Mismatch in sizes on iter " << i;
        EXPECT_EQ(tree_result, brute_result) << "Mismatch in points on iter " << i;
    }
}

template <size_t K, typename TreeType, typename QueryType, typename PointType>
void run_dimension_test() {
    const int num_points = 1000;
    std::array<double, K> mins; mins.fill(-100.0);
    std::array<double, K> maxs; maxs.fill(100.0);

    auto points = generate_random_points<K, PointType>(num_points, mins, maxs);
    TreeType tree(points);

    std::mt19937 gen(K); 
    std::uniform_real_distribution<double> dist(-100.0, 100.0);

    for (int i = 0; i < 20; ++i) {
        QueryType query;
        for (size_t d = 0; d < K; ++d) {
            double v1 = dist(gen);
            double v2 = dist(gen);
            query.start[d] = std::min(v1, v2);
            query.end[d] = std::max(v1, v2);
        }

        auto tree_result = tree.search(query);
        auto brute_result = brute_force_search<PointType, QueryType>(points, query);

        std::sort(tree_result.begin(), tree_result.end());
        std::sort(brute_result.begin(), brute_result.end());

        EXPECT_EQ(tree_result.size(), brute_result.size());
        EXPECT_EQ(tree_result, brute_result);
    }
}