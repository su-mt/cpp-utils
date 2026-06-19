#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <tuple>
#include <vector>
#include <iostream>

namespace kdtree {


    

template <typename T, size_t D>
struct Point {
    std::array<T, D> coords;

    T& operator[](std::size_t i) {
        return coords[i];
    }
    const T& operator[](std::size_t i) const {
        return coords[i];
    }

    auto operator<=>(const Point&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Point& pt) {
        os << '(';
        for (size_t i = 0; i < D; ++i) {
            os << pt[i] << (i < D - 1 ? ", " : "");
        }
        os << ")";
        return os;
    }

};

template <typename T>
using Point2D = Point<T , 2>;

template <typename T>
struct Point3D {
    T x, y, z;
};

template <typename T, size_t K>
struct Region{ 
    Point<T,K> start, end;

    Region() {
        for(int i = 0; i < K; i++) {
            start[i] = std::numeric_limits<T>::lowest();
            end[i] = std::numeric_limits<T>::max();
        }
    }

    Region (Point<T, K> st,Point<T, K> en) : start(st), end(en) { }

    bool inside(const Region<T,K>& oth) const {
        for (size_t i = 0; i < K; i++) {
            if (start[i] < oth.start[i] or end[i] > oth.end[i]) {
                return false;
            }
        }
        return true;
    }

    bool intersects(const Region<T,K>& oth) const {
        for (size_t i = 0; i < K; i++) {
            if (end[i] < oth.start[i] or start[i] > oth.end[i]) {
                return false;
            }
        }
        return true;
    }

    bool contains(const Point<T, K>& p) const {
        for (size_t i = 0; i < K; i++) {
            if (p[i] < start[i] || p[i] > end[i]) {
                return false;
            }
        }
        return true;
    }

    bool outside (const Region<T,K>& oth) const {
        return !inside(oth) and !intersects(oth);
    }
};


template <typename T, size_t K>
class KdTree {
private:
    struct Node {
        Point<T,K> point;
        int axis; // 0 - x, 1 - y, 2 - z, etc...
        Node* left, *right;

        Node(const Point<T,K>& pt, int ax) : point(pt), axis(ax), left(nullptr), right(nullptr) {}
    };

    

    Node* root;


    Node* build(std::vector<Point<T, K>>& points, int start, int end, int depth) {
        if (start >= end) {
            return nullptr;
        }

        int axis = depth % K;
        int mid = start + (end - start) / 2;

        auto cmp = [axis](const Point<T,K>& a, const Point<T,K>& b) -> bool {
            return a[axis] < b[axis];
        };
        std::nth_element(points.begin() + start, points.begin() + mid, points.begin() + end, cmp);

        Node* node = new Node(points[mid], axis);


        node->left = build(points, start, mid, depth + 1);
        node->right = build(points, mid + 1, end, depth + 1);

        return node;
    }

    void reportSubtree(Node* node, std::vector<Point<T, K>>& ans) const {
        if (node == nullptr) return;
        ans.push_back(node->point);
        reportSubtree(node->left, ans);
        reportSubtree(node->right, ans);
    }

    void searchR (Node* node, Region<T,K> reg, Region<T,K> search_range, std::vector<Point<T, K>>& ans) const {

        if (node == nullptr){
            return ;
        }

        if (/*node->left == nullptr and node->right == nullptr and*/ search_range.contains(node->point)){
            ans.push_back(node->point);
        } 
    
        auto split_val = node->point[node->axis];

        if (node->left != nullptr) {
            Region<T,K> left_region = reg;
            left_region.end[node->axis] = split_val;

            if (left_region.inside(search_range)) {
                reportSubtree(node->left, ans); 
            } else if (left_region.intersects(search_range)) {
                searchR(node->left, left_region, search_range, ans); 
            }
        }

        if (node->right != nullptr) {
            Region<T,K> right_region = reg;
            right_region.start[node->axis] = split_val;

            if (right_region.inside(search_range)) {
                reportSubtree(node->right, ans);
            } else if (right_region.intersects(search_range)) {
                searchR(node->right, right_region, search_range, ans);
            }
        }
        
        

    }


    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:

    KdTree(std::vector<Point<T,K>> points) {
        root = build(points, 0, points.size(), 0);
    }


    ~KdTree() {
        destroyTree(root);
    }


    std::vector<Point<T, K>> search (const Region<T,K>&  search_range) const {
        Region<T,K> global_region;
        std::vector<Point<T, K>> ans;
        searchR(root, global_region, search_range, ans);

        return ans;
    }


};
}

template class kdtree::KdTree<double, 3>;