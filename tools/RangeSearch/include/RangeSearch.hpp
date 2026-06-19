
#ifndef RANGESEARCH_HPP_
#define RANGESEARCH_HPP_


#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <tuple>
#include <vector>
#include <iostream>


template <typename T, size_t D>
struct Point {
    std::array<T, D> coords;

    T& operator[](size_t i) {
        return coords[i];
    }
    const T& operator[](size_t i) const {
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
struct Range {
    Point<T,K> start, end;
    Range() = default;
    Range (Point<T, K> st,Point<T, K> en) : start(st), end(en) { }

    bool inside(const Range<T,K>& oth) const {
        for (size_t i = 0; i < K; i++) {
            if (start[i] < oth.start[i] or end[i] > oth.end[i]) {
                return false;
            }
        }
        return true;
    }

    bool intersects(const Range<T,K>& oth) const {
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

    bool outside (const Range<T,K>& oth) const {
        return !inside(oth) and !intersects(oth);
    }

};

#endif // RANGESEARCH_HPP_