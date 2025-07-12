#include <iostream>
#include <fstream>
#include <thread>
#include <unordered_set>
#include <vector>
#include <list>
#include <set>

#define FORCE

using namespace std;





struct Point {
    float x, y;

    bool operator==(const Point& other) const {
        const float EPS = 1e-6f;
        return (fabs(x - other.x) < EPS) && (fabs(y - other.y) < EPS);
    }
};


struct hashPoint {
    size_t operator()(const Point& p) const {
        size_t h1 = std::hash<float>{}(p.x);
        size_t h2 = std::hash<float>{}(p.y);
        return h1 ^ (h2 << 1);  // простой комбинированный хеш
    }
};





namespace geom {

    static Point p0;
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

            auto it1 = conHull.begin();
            auto it2 = next(it1);
            for (; it2 != conHull.end(); ++it1, ++it2) {
        
                if ((&(*it1) == &(*p_H) && *it2 == newP) || (&(*it2) == &(*p_H) && *it1 == newP))
                    continue;
                if (&(*it1) == &(*p_H) || &(*it2) == &(*p_H) || *it1 == newP || *it2 == newP)
                    continue;

                auto onSegment = [](const Point& p, const Point& q, const Point& r) {
                    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
                        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
                };

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

                auto onSegment = [](const Point& p, const Point& q, const Point& r) {
                    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
                        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
                };
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
    list<Point>::iterator prev;
    list<Point>::iterator next;
    float length;
    Edge(list<Point>::iterator p1, list<Point>::iterator p2)
        : prev(p1), next(p2), length(geom::dist(*p1, *p2)) {}

    bool operator<(const Edge& other) const {
        if (fabs(length - other.length) > 1e-9f)
            return length > other.length;


        if (&(*prev) != &(*other.prev))
            return &(*prev) < &(*other.prev);
        
        
        return &(*next) < &(*other.next);
    }
};





class EdgeQueue {
    set<Edge> edges;
    list<Point>& hull;
public:
    EdgeQueue(list<Point>& hull_) : hull(hull_) {
        buildFromHull();
    }

    void buildFromHull() {
        edges.clear();
        if (hull.size() < 2) return;
        auto it = hull.begin();
        auto prev = it++;
        for (; it != hull.end(); ++it, ++prev) {
            edges.emplace(prev, it);
        }

        edges.emplace(--hull.end(), hull.begin());
    }

    Edge getMaxEdge() const {
        return *edges.begin();
    }

    void splitEdge(list<Point>::iterator ptNew) {
        Edge e = getMaxEdge();
        edges.erase(e);
        edges.emplace(e.prev, ptNew);
        edges.emplace(ptNew, e.next);
    }

    void removeEdge(list<Point>::iterator p1, list<Point>::iterator p2) {
        edges.erase(Edge(p1, p2));
    }
};



class minConvexHull {
    vector<Point> H;
    list <Point> G;

    vector<Point> getMinConvH(const vector<Point>& points) {
        if (points.size() < 3) return points;

        int n = points.size();
        vector<Point> p = points;
        int ymin = 0;
        for (int i = 1; i < n; i++) {
            if (p[i].y < p[ymin].y || (p[i].y == p[ymin].y && p[i].x < p[ymin].x))
                ymin = i;
        }
        swap(p[0], p[ymin]);
        geom::p0 = p[0];

        sort(p.begin() + 1, p.end(), geom::compare);

        vector<Point> filtered;
        filtered.push_back(p[0]);
        for (int i = 1; i < n; ++i) {
            while (i < n - 1 && geom::orientation(geom::p0, p[i], p[i + 1]) == 0)
                ++i;
            filtered.push_back(p[i]);
        }

        vector<Point> hull;
        if (filtered.size() < 3) return filtered;
        hull.push_back(filtered[0]);
        hull.push_back(filtered[1]);
        hull.push_back(filtered[2]);
        for (size_t i = 3; i < filtered.size(); ++i) {
            while (hull.size() >= 2 && geom::orientation(hull[hull.size()-2], hull[hull.size()-1], filtered[i]) != 2)
                hull.pop_back();
            hull.push_back(filtered[i]);
        }
        return hull;
        
    }

public:
    minConvexHull(const vector<Point>& points) {
        H = getMinConvH(points);
        G = geom::subtract(points,H);
    }

    vector<Point> getH() const {
        return H;
    }

    list <Point> getG() const {
        return G;
    }
};



class ConcaveHull {
private:
    vector<Point> P;
    double gamma = 0;
    vector<Point> H;
    list <Point> G;
    size_t nH;  
    list<Point> conhull;

    

    list<Point> __getconcavehull() {
        list<Point> conHull (H.begin(), H.end());
        EdgeQueue edgesq(conHull);

        while(true){

            std::cout << "G.size() = " << G.size() << ", conHull.size() = " << conHull.size() << std::endl;
            Edge edge = edgesq.getMaxEdge();
            auto pb = edge.prev;
            auto pe = edge.next;
            double qd = edge.length;
            double minS = 4*qd;


            auto insertG = G.end();
            for(auto it = G.begin(); it != G.end(); ++it) {
                Point pt = *it;
                float qd1 =  geom::distSq(pt,*pb), qd2 = geom::distSq(*pe,pt);

                if (qd1+qd2 - qd > gamma * min(qd1,qd2)){
                    continue;
                }

                double St = geom::triangleSquare(*pb,pt,*pe);

                if (St < minS){
                    if (geom::isCrossHull(pb,pt,conHull)){
                        continue;
                    }

                    insertG = it;
                    minS = St;
                }

            }

            if (insertG != G.end()){
                auto ptNew = conHull.insert(next(pb), *insertG);
                edgesq.splitEdge(ptNew);
                G.erase(insertG);

            } else {
                break;
            }
        }
        conhull = conHull;
        return conHull;
    }


public:
    ConcaveHull(const vector<Point>& points, double g = 0)
        : P(points), gamma(g),H(), G(), nH(0) {
            minConvexHull convex(points);
            H = convex.getH();
            G = convex.getG();
            nH = H.size();
        }

    ConcaveHull(const minConvexHull& convex)
        : H(convex.getH()),G(convex.getG()), nH(H.size()){}  

    list<Point> getConcaveHull() {
        return __getconcavehull();
    }

    list<Point> getConcaveHull(double g) {
        gamma = g;
        return __getconcavehull();
    }
};


int main(int argc, char** argv)
{

    // vector<Point> p = {{1,2}, {1,5}, {2,3}, {2,4},{3,3},{3,4}, {4,2}, {4,5}};
    // ConcaveHull ch(p);
    // auto chh = ch.getConcaveHull(1555);
    
    // for (auto x: chh) {
    //     cout << x.x << " " << x.y << "\n";
    // }

    
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " </input_file/path> </output_file/path>" << endl;
        return 1;
    }

    ifstream data(argv[1]);
    if (!data) {
        cerr << "Input file: open error" << endl;
        return 1;
    }

    #ifndef FORCE

    constexpr int wait_seconds = 5;
    if (filesystem::exists(argv[2])) {
        cout << "Warning: file \"" << argv[2] << "\" already exists and will be overwritten!" << endl;
        cout << "The program will continue in " << wait_seconds << " seconds..." << endl;
        this_thread::sleep_for(std::chrono::seconds(wait_seconds));

    }
    #endif

    ofstream out(argv[2], ios::binary);
    if (!out) {
        cerr << "Output file: create error" << endl;
        return 1;
    }

    vector<Point> points;
    float x, y;
    while (data >> x >> y) {
        points.push_back({x, y});
    }

    ConcaveHull CH(points);
    list<Point> cnh = CH.getConcaveHull(1500);

    for (const auto& pt : cnh) {
        out << pt.x << ' ' << pt.y << '\n';
    }
    auto pt = cnh.front();
    out << pt.x << ' ' << pt.y << '\n';
    return 0;
}