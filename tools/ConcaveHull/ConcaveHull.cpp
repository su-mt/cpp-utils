#include "ConcaveHull.hpp"

#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace std;

// ===== Point =====

bool Point::operator==(const Point& other) const {
    const float EPS = 1e-6f;
    return (fabs(x - other.x) < EPS) && (fabs(y - other.y) < EPS);
}

// ===== hashPoint =====

size_t hashPoint::operator()(const Point& p) const {
    size_t h1 = hash<float>{}(p.x);
    size_t h2 = hash<float>{}(p.y);
    return h1 ^ (h2 << 1);
}

// ===== geom =====

namespace geom {

    Point p0;

    float distSq(const Point& a, const Point& b) {
        return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    }

    float dist(const Point& a, const Point& b) {
        return sqrt(distSq(a,b));
    }

    int orientation(const Point& a, const Point& b, const Point& c) {
        float val = (b.y - a.y) * (c.x - b.x) - 
                    (b.x - a.x) * (c.y - b.y);
        if (fabs(val) < 1e-6f) return 0;
        return (val > 0) ? 1 : 2;
    }

    bool compare(const Point& a, const Point& b) {
        int o = orientation(p0, a, b);
        if (o == 0)
            return distSq(p0, a) < distSq(p0, b);
        return (o == 2);
    }

    double triangleSquare(const Point& a, const Point& b, const Point& c) {
        double ab = dist(a, b);
        double bc = dist(b, c);
        double ca = dist(c, a);
        double s = (ab + bc + ca) / 2.0;
        return sqrt(s * (s - ab) * (s - bc) * (s - ca));
    }

    bool isCrossHull(list<Point>::iterator p_H, const Point& newP, const list<Point>& conHull) {
        if (conHull.size() < 3) return false;

        auto onSegment = [](const Point& p, const Point& q, const Point& r) {
            return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
                   q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
        };

        auto it1 = conHull.begin();
        auto it2 = next(it1);
        for (; it2 != conHull.end(); ++it1, ++it2) {
            if ((&(*it1) == &(*p_H) && *it2 == newP) || (&(*it2) == &(*p_H) && *it1 == newP))
                continue;
            if (&(*it1) == &(*p_H) || &(*it2) == &(*p_H) || *it1 == newP || *it2 == newP)
                continue;

            int o1 = orientation(*p_H, newP, *it1);
            int o2 = orientation(*p_H, newP, *it2);
            int o3 = orientation(*it1, *it2, *p_H);
            int o4 = orientation(*it1, *it2, newP);

            if (o1 != o2 && o3 != o4)
                return true;

            if (o1 == 0 && onSegment(*p_H, *it1, newP)) return true;
            if (o2 == 0 && onSegment(*p_H, *it2, newP)) return true;
            if (o3 == 0 && onSegment(*it1, *p_H, *it2)) return true;
            if (o4 == 0 && onSegment(*it1, newP, *it2)) return true;
        }

        auto it_last = prev(conHull.end());
        auto it_first = conHull.begin();
        if ((&(*it_last) != &(*p_H) && &(*it_first) != &(*p_H)) &&
            !(*it_last == newP && *it_first == newP)) {

            int o1 = orientation(*p_H, newP, *it_last);
            int o2 = orientation(*p_H, newP, *it_first);
            int o3 = orientation(*it_last, *it_first, *p_H);
            int o4 = orientation(*it_last, *it_first, newP);

            if (o1 != o2 && o3 != o4)
                return true;

            if (o1 == 0 && onSegment(*p_H, *it_last, newP)) return true;
            if (o2 == 0 && onSegment(*p_H, *it_first, newP)) return true;
            if (o3 == 0 && onSegment(*it_last, *p_H, *it_first)) return true;
            if (o4 == 0 && onSegment(*it_last, newP, *it_first)) return true;
        }

        return false;
    }

    list<Point> subtract(const vector<Point>& set1, const vector<Point>& set2) {
        list<Point> result;
        unordered_set<Point, hashPoint> setH(set2.begin(), set2.end());
        for (const auto& p : set1) {
            if (setH.find(p) == setH.end()) {
                result.push_back(p);
            }
        }
        return result;
    }
}



struct Edge {
    std::list<Point>::iterator prev;
    std::list<Point>::iterator next;
    float length;

    Edge(std::list<Point>::iterator p1, std::list<Point>::iterator p2)
        : prev(p1), next(p2), length(geom::distSq(*p1, *p2)) {}

    bool operator<(const Edge& other) const {
    if (fabs(length - other.length) > 1e-9f)
        return length > other.length; 


    return std::tuple<Point*, Point*>(&(*prev), &(*next)) <
       std::tuple<Point*, Point*>(&(*other.prev), &(*other.next));

    }

};


struct EdgeKeyHash {
    size_t operator()(const std::pair<Point*, Point*>& p) const {
        return reinterpret_cast<uintptr_t>(p.first) ^ reinterpret_cast<uintptr_t>(p.second);
    }
};

struct EdgeKeyEq {
    bool operator()(const std::pair<Point*, Point*>& a, const std::pair<Point*, Point*>& b) const {
        return a.first == b.first && a.second == b.second;
    }
};

class EdgeQueue {
    std::set<std::shared_ptr<Edge>, 
             bool(*)(const std::shared_ptr<Edge>&, const std::shared_ptr<Edge>&)> sortedEdges;

    std::unordered_map<std::pair<Point*, Point*>, std::shared_ptr<Edge>, EdgeKeyHash, EdgeKeyEq> edgeMap;

    std::list<Point>& hull;

    static bool edgeCompare(const std::shared_ptr<Edge>& a, const std::shared_ptr<Edge>& b) {
        return *a < *b;
    }

public:
    EdgeQueue(std::list<Point>& hull_) 
        : hull(hull_), sortedEdges(edgeCompare) {
        buildFromHull();
    }

    void buildFromHull() {
        sortedEdges.clear();
        edgeMap.clear();

        if (hull.size() < 2) return;
        auto it = hull.begin();
        auto prev = it++;
        for (; it != hull.end(); ++it, ++prev) {
            addEdge(prev, it);
        }
        addEdge(--hull.end(), hull.begin());
    }

    void addEdge(std::list<Point>::iterator p1, std::list<Point>::iterator p2) {
        auto edge = std::make_shared<Edge>(p1, p2);
        sortedEdges.insert(edge);
        edgeMap[{&(*p1), &(*p2)}] = edge;
    }

    void removeEdge(std::list<Point>::iterator p1, std::list<Point>::iterator p2) {
        auto key = std::make_pair(&(*p1), &(*p2));
        auto it = edgeMap.find(key);
        if (it != edgeMap.end()) {
            sortedEdges.erase(it->second);
            edgeMap.erase(it);
        }
    }

    void splitEdge(std::list<Point>::iterator ptNew) {
        auto maxEdge = *sortedEdges.begin();
        auto prev = maxEdge->prev;
        auto next = maxEdge->next;

        removeEdge(prev, next);
        addEdge(prev, ptNew);
        addEdge(ptNew, next);
    }

    Edge getMaxEdge() const {
        return **sortedEdges.begin();
    }

    bool empty() const {
        return sortedEdges.empty();
    }
};
// ===== minConvexHull =====

minConvexHull::minConvexHull(const vector<Point>& points) {
    H = getMinConvH(points);
    G = geom::subtract(points, H);
}

vector<Point> minConvexHull::getMinConvH(const vector<Point>& points) {
    if (points.size() < 3) return points;

    vector<Point> p = points;
    int ymin = 0;
    for (int i = 1; i < p.size(); i++) {
        if (p[i].y < p[ymin].y || (p[i].y == p[ymin].y && p[i].x < p[ymin].x))
            ymin = i;
    }
    swap(p[0], p[ymin]);
    geom::p0 = p[0];

    sort(p.begin() + 1, p.end(), geom::compare);

    vector<Point> filtered{p[0]};
    for (size_t i = 1; i < p.size(); ++i) {
        while (i < p.size() - 1 && geom::orientation(geom::p0, p[i], p[i + 1]) == 0)
            ++i;
        filtered.push_back(p[i]);
    }

    if (filtered.size() < 3) return filtered;

    vector<Point> hull = {filtered[0], filtered[1], filtered[2]};
    for (size_t i = 3; i < filtered.size(); ++i) {
        while (hull.size() >= 2 &&
               geom::orientation(hull[hull.size() - 2], hull[hull.size() - 1], filtered[i]) != 2)
            hull.pop_back();
        hull.push_back(filtered[i]);
    }
    return hull;
}

vector<Point> minConvexHull::getH() const {
    return H;
}

list<Point> minConvexHull::getG() const {
    return G;
}


// ===== ConcaveHull =====

ConcaveHull::ConcaveHull(const std::vector<Point>& points, double g)
    : H(minConvexHull(points).getH()), gamma(g) {
    std::unordered_set<Point, hashPoint> hullSet(H.begin(), H.end());
    G.reserve(points.size() - H.size());
    for (const auto& p : points) {
        if (hullSet.find(p) == hullSet.end())
            G.push_back(p);
    }
    nH = H.size();
}

ConcaveHull::ConcaveHull(const std::vector<Point>& points)
    : H(minConvexHull(points).getH()) {
    std::unordered_set<Point, hashPoint> hullSet(H.begin(), H.end());
    G.reserve(points.size() - H.size());
    for (const auto& p : points) {
        if (hullSet.find(p) == hullSet.end())
            G.push_back(p);
    }
    nH = H.size();
}

inline void removeCandidate(std::vector<Point>& G, size_t idx) {
    G[idx] = G.back();
    G.pop_back();
}


std::list<Point> ConcaveHull::__getconcavehull() {
    std::list<Point> conHull(H.begin(), H.end());
    EdgeQueue edgesq(conHull);

    while (true) {
        Edge edge = edgesq.getMaxEdge();
        auto pb_it = edge.prev;
        auto pe_it = edge.next;
        Point pb = *pb_it, pe = *pe_it;
        double qd = geom::dist(pb, pe);

        size_t Gsize = G.size();
        int bestIdx = -1;
        double globalMinS = std::numeric_limits<double>::infinity();
        std::mutex mutex;

        const unsigned numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

        for (unsigned t = 0; t < numThreads; ++t) {
            threads.emplace_back([&, t]() {
                size_t chunkSize = (Gsize + numThreads - 1) / numThreads;
                size_t startIdx = t * chunkSize;
                size_t endIdx = std::min(Gsize, startIdx + chunkSize);

                double localMinS = std::numeric_limits<double>::infinity();
                int localBest = -1;

                for (size_t i = startIdx; i < endIdx; ++i) {
                    const Point& pt = G[i];
                    double d1 = geom::dist(pt, pb);
                    double d2 = geom::dist(pe, pt);
                    if (d1 + d2 - qd > gamma * std::min(d1, d2)) continue;

                    double S = geom::triangleSquare(pb, pt, pe);
                    if (S >= localMinS) continue;

                    if (geom::isCrossHull(pb_it, pt, conHull)) continue;

                    localMinS = S;
                    localBest = static_cast<int>(i);
                }

                if (localBest >= 0) {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (localMinS < globalMinS) {
                        globalMinS = localMinS;
                        bestIdx = localBest;
                    }
                }
            });
        }

        for (auto& th : threads) {
            th.join();
        }

        if (bestIdx < 0)
            break;

        auto insertPos = std::next(pb_it);
        auto newIt = conHull.insert(insertPos, G[bestIdx]);
        edgesq.splitEdge(newIt);

        removeCandidate(G, bestIdx);
    }

    conhull = conHull;
    cout << conHull.size() << endl;
    return conHull;
}



std::list<Point> ConcaveHull::getConcaveHull() {
    return __getconcavehull();
}

std::list<Point> ConcaveHull::getConcaveHull(double g) {
    gamma = g;
    return __getconcavehull();
}
