#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <limits>
#include <thread>
#include <mutex>
#include <future>


struct Point {
    float x, y;

    bool operator==(const Point& other) const;
};

struct hashPoint {
    size_t operator()(const Point& p) const;
};

namespace geom {
    extern Point p0;
    float distSq(const Point& a, const Point& b);
    float dist(const Point& a, const Point& b);
    int orientation(const Point& a, const Point& b, const Point& c);
    bool compare(const Point& a, const Point& b);
    double triangleSquare(const Point& a, const Point& b, const Point& c);
    bool isCrossHull(std::list<Point>::iterator p_H, const Point& newP, const std::list<Point>& conHull);
    std::list<Point> subtract(const std::vector<Point>& set1, const std::vector<Point>& set2);
}

class minConvexHull {
public:
    minConvexHull(const std::vector<Point>& points);
    std::vector<Point> getH() const;
    std::list<Point> getG() const;
private:
    std::vector<Point> H;
    std::list<Point> G;
    std::vector<Point> getMinConvH(const std::vector<Point>& points);
};

class ConcaveHull {
public:
    ConcaveHull(const std::vector<Point>& points, double gamma);
    ConcaveHull(const std::vector<Point>& points);
    std::list<Point> getConcaveHull();
    std::list<Point> getConcaveHull(double g);

private:
    std::vector<Point> G;     // кандидаты для вогнутости
    std::vector<Point> H;     // начальная выпуклая оболочка
    size_t nH;
    double gamma;
    std::list<Point> conhull;
    std::list<Point> __getconcavehull();

};
