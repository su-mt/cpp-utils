#ifndef RANGETREE_RANGETREE_HPP_
#define RANGETREE_RANGETREE_HPP_

#include "RangeSearch.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <memory>
#include <numeric>
#include <set>
#include <tuple>
#include <vector>


namespace rangetree {

template <typename T, size_t D, size_t OrigD = D>
class RangeTreeNd {

    template <typename, size_t, size_t> 
    friend class RangeTreeNd;

private:
    struct Node {
        T split_coord;
        Node* left;
        Node* right;
        RangeTreeNd<T, D - 1, OrigD> next_tree;

        Node(T coord, RangeTreeNd<T, D - 1, OrigD>&& assoc_tree)
            : split_coord(coord), 
              left(nullptr), 
              right(nullptr),
              next_tree(std::move(assoc_tree)) {}

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(Node&&) = delete;

        ~Node() {
            delete left;
            delete right;
        }
    };

    Node* root;

    RangeTreeNd(const std::vector<Point<T, OrigD>>& points, std::vector<size_t> indices) {
        root = buildR(points, indices, 0, indices.size() - 1);
    }

    Node* buildR(const std::vector<Point<T, OrigD>>& base_points, std::vector<size_t>& indices, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        constexpr size_t current_axis = OrigD - D;
        int mid = start + (end - start) / 2;

        std::nth_element(
            indices.begin() + start, 
            indices.begin() + mid,
            indices.begin() + end + 1, 
            [&base_points](size_t a, size_t b) {
                return base_points[a][current_axis] < base_points[b][current_axis];
            }
        );

        T mid_coord = base_points[indices[mid]][current_axis];

        std::vector<size_t> assoc_indices(indices.begin() + start, indices.begin() + end + 1);
        RangeTreeNd<T, D - 1, OrigD> assoc_tree(base_points, std::move(assoc_indices));

        Node* node = new Node(mid_coord, std::move(assoc_tree));

        if (start < end) {
            node->left = buildR(base_points, indices, start, mid);
            node->right = buildR(base_points, indices, mid + 1, end);
        }

        return node;
    }




public:
    RangeTreeNd(std::vector<Point<T, OrigD>> points) {
        std::vector<size_t> indices(points.size());
        std::iota(indices.begin(), indices.end(), 0LL);
        root = buildR(points, indices, 0, indices.size() - 1);
    }

    RangeTreeNd(const RangeTreeNd&) = delete;
    RangeTreeNd& operator=(const RangeTreeNd&) = delete;

    RangeTreeNd(RangeTreeNd&& other) noexcept : root(other.root) {
        other.root = nullptr;
    }

    RangeTreeNd& operator=(RangeTreeNd&& other) noexcept {
        if (this != &other) {
            delete root;
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }

    std::vector<Point<T, OrigD>> search(const Range<T, OrigD>& search_range) const {
        std::vector<Point<T, OrigD>> result;
        if (root == nullptr) {
            return result;
        }

        constexpr size_t current_axis = OrigD - D;
        const T low  = search_range.start[current_axis];   
        const T high = search_range.end[current_axis];  

        auto is_leaf = [](Node* n) { return n->left == nullptr && n->right == nullptr; };

        auto collect_subtree = [&](Node* n) {
            if (n == nullptr) return;
            auto sub = n->next_tree.search(search_range);
            result.insert(result.end(), sub.begin(), sub.end());
        };

        auto collect_leaf = [&](Node* n) {
            if (n != nullptr && n->split_coord >= low && n->split_coord <= high) {
                collect_subtree(n);
            }
        };


        Node* v = root;
        while (!is_leaf(v)) {
            if (high < v->split_coord) {
                v = v->left;
            } else if (low > v->split_coord) {
                v = v->right;
            } else {
                break; 
            }
        }

        if (is_leaf(v)) {
            collect_leaf(v);
            return result;
        }


        Node* vl = v->left;
        while (!is_leaf(vl)) {
            if (low <= vl->split_coord) {
                collect_subtree(vl->right); 
                vl = vl->left;
            } else {
                vl = vl->right;
            }
        }
        collect_leaf(vl);


        Node* vr = v->right;
        while (!is_leaf(vr)) {
            if (high >= vr->split_coord) {
                collect_subtree(vr->left); 
                vr = vr->right;
            } else {
                vr = vr->left;
            }
        }
        collect_leaf(vr);

        return result;
    }

    ~RangeTreeNd() {
        delete root;
    }
};


template <typename T, size_t OrigD>
class RangeTreeNd<T, 1, OrigD> {

    template <typename, size_t, size_t> 
    friend class RangeTreeNd;
    
private:
    std::vector<Point<T, OrigD>> sorted_points;
    

    RangeTreeNd(const RangeTreeNd&) = delete;
    RangeTreeNd& operator=(const RangeTreeNd&) = delete;
    RangeTreeNd(RangeTreeNd&&) noexcept = default;
    RangeTreeNd& operator=(RangeTreeNd&&) noexcept = default;

    RangeTreeNd(const std::vector<Point<T, OrigD>>& base_points, std::vector<size_t> indices) {
        sorted_points.reserve(indices.size());
        for (size_t idx : indices) {
            sorted_points.push_back(base_points[idx]);
        }
        constexpr size_t last_axis = OrigD - 1;
        auto cmp_last_axis=[](const Point<T, OrigD>& a, const Point<T, OrigD>& b) {
                return a[last_axis] < b[last_axis];
        };
        std::ranges::sort(sorted_points, cmp_last_axis);
    }

public:
    std::vector<Point<T, OrigD>> search (const Range<T,OrigD>& search_range) const {

        constexpr size_t last_axis = OrigD - 1;
        auto proj = [](const Point<T, OrigD>& p) { return p[last_axis]; };

        assert(std::ranges::is_sorted(sorted_points, {}, proj));

        auto it1 = std::ranges::lower_bound(sorted_points, search_range.start[last_axis], {}, proj);
        auto it2 = std::ranges::upper_bound(sorted_points, search_range.end[last_axis], {}, proj);

        return std::vector<Point<T, OrigD>> (it1, it2);
    }

};

template <typename T>
using RangeTree3d = RangeTreeNd<T, 3>;

} // namespace rangetree

template class rangetree::RangeTreeNd<double, 3>;

#endif // RANGETREE_RANGETREE_HPP_