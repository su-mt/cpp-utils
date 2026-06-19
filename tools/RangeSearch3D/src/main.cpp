#include <cassert>
#include <iostream>
#include <vector>
#include "kdtree/kdtree.hpp"

int main() {
    std::vector<kdtree::Point<double, 3>> points{
        {{10,10,10}},
        {{20,20,20}},
        {{30,30,30}},
        {{40,40,40}}
    };

    kdtree::KdTree<double, 3> kd(points);

    auto ans = kd.search({
        {15,15,15},
        {35,35,35}
    });

    return 0;
}