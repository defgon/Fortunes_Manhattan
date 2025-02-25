#ifndef VORPT_H
#define VORPT_H

#include <set>
#include "point.h"

struct Edge {
	std::pair<int,int> original_edge;
    std::vector<point_double> final_edges;	
};

struct vorpt{
    point_double coords;
    std::vector<Edge> edges;
    std::vector<point_double> vertices;
};

struct Vertex{
	point_double center;
	std::vector<vorpt*> points;
};

std::vector<vorpt*> find_common_pts(const std::vector<vorpt*>& first_t, const std::vector<vorpt*>& second_t) {
    std::vector<vorpt*> common_points;

    // Use a set for efficient lookup
    std::set<vorpt*> points_set(first_t.begin(), first_t.end());

    for (auto* p : second_t) {
        if (points_set.find(p) != points_set.end()) {
            common_points.push_back(p);
        }
    }

    return common_points;
}

#endif