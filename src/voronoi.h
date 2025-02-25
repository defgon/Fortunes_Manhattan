#ifndef VORONOI_H
#define VORONOI_H


#include <iostream>
#include <vector>
#include <limits>
#include "point.h"
#include "test.h"
#include "beachline.h"
#include "geometry.h"
#include <map>
#include <cmath>

bool VoronoiDiagram(std::vector<vorpt> &input, std::vector<Vertex> &vertex, std::vector<Edge> &edge, std::vector<point_double> &wrongs) {
    Beachline beachline = Beachline();
    std::priority_queue<event, std::vector<event>, std::greater<event>> events;

    // Add an edge to the vorpt structure
    auto add_edge = [&](int u, int v, BeachNode* c1, BeachNode* c2) {
        if (c1) c1->end = edge.size() * 2;
        if (c2) c2->end = edge.size() * 2 + 1;
        Edge e;
        e.original_edge = {u, v};
        edge.emplace_back(e);
    };

    auto write_edge = [&](int index, int v) {
        if (index % 2 == 0) {
            edge[index / 2].original_edge.first = v;
        } else {
            edge[index / 2].original_edge.second = v;
        }
    };

    auto add_event = [&](BeachNode* current) {
        double next;
        if (beachline.get_event(current, next))
            events.emplace(next, current);
    };

    int n = input.size();
    sz = 0;
    arr = new BeachNode[n * 4];

    // Sort input based on y-coordinate, then x-coordinate
    std::sort(input.begin(), input.end(), [](const vorpt &l, const vorpt &r) {
        return l.coords.second != r.coords.second ? l.coords.second < r.coords.second : l.coords.first < r.coords.first;
    });

    // vytvor beachline
    BeachNode* temp = beachline.head = new_node(input[0].coords, 0);
    beachline.tail = temp;
    BeachNode* t2;
    for (int i = 1; i < n; i++) {
        if (std::abs(input[i].coords.second - input[0].coords.second) < EPS) {
            beachline.insert(t2 = new_node(input[i].coords, i), temp, true);
            temp = t2;
        } else {
            events.emplace(input[i].coords.second, i);
        }
    }

    // Fortune's Algorithm
    int k = 0;
    while (!events.empty()) {
        event q = events.top();
        events.pop();
        BeachNode *previous, *current, *next, *site;
        int v = vertex.size();
        int index = q.index;
        beachline.sweepline = q.sweep;

        if (q.type == 0) { 
            point_double point = input[index].coords;
            current = beachline.find(point.first);
            beachline.insert(site = new_node(point, index), current, false);
            beachline.insert(previous = new_node(current->point, current->index), site, false);
            add_edge(-1, -1, site, previous);
            add_event(previous);
            add_event(current);
        } else {
            current = q.current;
            previous = current->previous;
            next = current->next;
            if (!previous || !next || previous->index != q.previous || next->index != q.next) continue;

            if (center_check(get_circumcenter2(previous->point, next->point, current->point), previous->point, next->point, current->point)) {
                Vertex vx;
                vx.points = {&input[previous->index], &input[next->index], &input[current->index]};
                vx.center = get_circumcenter2(previous->point, next->point, current->point);
                vertex.push_back(vx);
                write_edge(previous->end, v);
                write_edge(current->end, v);
                add_edge(v, -1, nullptr, previous);
            }

            beachline.erase(current);
            add_event(previous);
            add_event(next);
            if (!calculateAngles(previous->point, next->point, current->point)) {
                delete[] arr;
                return false;
            }
        }
    }

    delete[] arr;
    return true;
}




void find_new_edges(std::vector<Vertex> &vertex, std::vector<Edge> &edge,std::vector<point_double> &wrongs){
	int h = edge.size(); 
	for (size_t i = 0; i < h; ++i) {
		auto& e = edge[i];
		if (e.original_edge.first != -1 && e.original_edge.second != -1) {
			auto common_points = find_common_pts(vertex[e.original_edge.first].points, vertex[e.original_edge.second].points);

			if (common_points.size() < 2) {
                std::cerr << "Error: Not enough common points for edge " << i << std::endl;
                continue;
            }

			// points
			point_double p1 = common_points[0]->coords;
			point_double p2 = common_points[1]->coords;

			// vertices
			point_double v1 = vertex[e.original_edge.first].center;
			point_double v2 = vertex[e.original_edge.second].center;

			// control if edges lie between x but not between according to y
			bool bool_Y = !(std::min(p1.second, p2.second) < v1.second && v1.second < std::max(p1.second, p2.second)) && 
						!(std::min(p1.second, p2.second) < v2.second && v2.second < std::max(p1.second, p2.second));

			bool between_x = ((p1.first < v1.first && v1.first < p2.first) && 
							(p1.first < v2.first && v2.first < p2.first));

			// control if edges lie between y but not between according to x
			bool between_y = !(std::min(p1.first, p2.first) < v1.first && v1.first < std::max(p1.first, p2.first)) && 
							!(std::min(p1.first, p2.first) < v2.first && v2.first < std::max(p1.first, p2.first)) && 
							(std::min(p1.second, p2.second) < v1.second && v1.second < std::max(p1.second, p2.second)) &&
							(std::min(p1.second, p2.second) < v2.second && v2.second < std::max(p1.second, p2.second));

			bool between_xy = (std::min(p1.first, p2.first) < v1.first && v1.first < std::max(p1.first, p2.first)) &&
                 (std::min(p1.first, p2.first) < v2.first && v2.first < std::max(p1.first, p2.first)) &&
                 (std::min(p1.second, p2.second) < v1.second && v1.second < std::max(p1.second, p2.second)) &&
                 (std::min(p1.second, p2.second) < v2.second && v2.second < std::max(p1.second, p2.second));

			bool point_between_y = (std::min(v1.first, v2.first) < p1.first && p1.first < std::max(v1.first, v2.first)) ||
         	(std::min(v1.first, v2.first) < p2.first && p2.first < std::max(v1.first, v2.first));


			point_double v_new1, v_new2;
			Edge e1, e2;
			Vertex vex1, vex2;
			if (between_x && bool_Y) {
				// dividing into 3 edges
				double dist1 = std::abs(p1.second - v1.second);
				double dist2 = std::abs(p2.second - v1.second);
				if (dist1 < dist2) {
					v_new1 = {v1.first, p1.second};
					v_new2 = {v2.first, p2.second};
				} else {
					v_new1 = {v1.first, p2.second};
					v_new2 = {v2.first, p1.second};
				}
				vex1.center = v_new1;
				vex2.center = v_new2;
				vertex.push_back(vex1);
				vertex.push_back(vex2);
				int temp = e.original_edge.second;
				e.original_edge.second = vertex.size() - 2;
				// middle edge
				e2.original_edge.first = vertex.size() - 2;
				e2.original_edge.second = vertex.size() - 1;
				// lower edge
				e1.original_edge.first = vertex.size() - 1;
				e1.original_edge.second = temp;
				// adding new edges
				edge.push_back(e1);
				edge.push_back(e2);
				common_points[0]->edges.push_back(e);
				common_points[0]->edges.push_back(e1);
				common_points[0]->edges.push_back(e2);

				common_points[1]->edges.push_back(e);
				common_points[1]->edges.push_back(e1);
				common_points[1]->edges.push_back(e2);
			}
			else if (between_y) {
				// dividing into 3 edges
				double dist1 = std::abs(p1.first - v1.first);
				double dist2 = std::abs(p2.first - v1.first);
				if (dist1 < dist2) {
					v_new1 = {p1.first, v1.second};
					v_new2 = {p2.first, v2.second};
				} else {
					v_new1 = {p2.first, v1.second};
					v_new2 = {p1.first, v2.second};
				}
				vex1.center = v_new1;
				vex2.center = v_new2;
				vertex.push_back(vex1);
				vertex.push_back(vex2);
				int temp = e.original_edge.second;
				e.original_edge.second = vertex.size() - 2;
				// middle edge
				e2.original_edge.first = vertex.size() - 2;
				e2.original_edge.second = vertex.size() - 1;
				// lower edge
				e1.original_edge.first = vertex.size() - 1;
				e1.original_edge.second = temp;
				// adding new edges
				edge.push_back(e1);
				edge.push_back(e2);
				
				common_points[0]->edges.push_back(e);
				common_points[0]->edges.push_back(e1);
				common_points[0]->edges.push_back(e2);

				common_points[1]->edges.push_back(e);
				common_points[1]->edges.push_back(e1);
				common_points[1]->edges.push_back(e2);
			} else if (between_xy){
				common_points[0]->edges.push_back(e);
				common_points[1]->edges.push_back(e);
				// do nothing
			} else if(point_between_y) {
				if (std::min(v1.first, v2.first) < p1.first && p1.first < std::max(v1.first, v2.first)){
					v_new1 = {p1.first,v2.second};
					vex1.center = v_new1;
				} else {
					v_new1 = {p2.first,v1.second};
					vex1.center = v_new1;
				}
				vertex.push_back(vex1);
				int temp = e.original_edge.second;
				e.original_edge.second = vertex.size() - 1;
				e1.original_edge.first = vertex.size() - 1;
				e1.original_edge.second = temp;
				edge.push_back(e1);
				common_points[0]->edges.push_back(e);
				common_points[0]->edges.push_back(e1);

				common_points[1]->edges.push_back(e);
				common_points[1]->edges.push_back(e1);
			} else {
				if (std::min(v1.second, v2.second) < p1.second && p1.second < std::max(v1.second, v2.second)){
					v_new1 = {v2.first,p1.second};
					vex1.center = v_new1;
				} else {
					v_new1 = {v1.first,p2.second};
					vex1.center = v_new1;
				}
				vertex.push_back(vex1);
				int temp = e.original_edge.second;
				e.original_edge.second = vertex.size() - 1;
				e1.original_edge.first = vertex.size() - 1;
				e1.original_edge.second = temp;
				edge.push_back(e1);

				common_points[0]->edges.push_back(e);
				common_points[0]->edges.push_back(e1);

				common_points[1]->edges.push_back(e);
				common_points[1]->edges.push_back(e1);
			}
		}
	}

}


std::vector<vorpt> get_unique_shapes(std::vector<vorpt>& points, std::vector<Vertex>& vertex) {
    for (auto& point : points) {
        for (const auto& edge : point.edges) {
            if (edge.original_edge.first != -1) {
                point_double v1 = vertex[edge.original_edge.first].center;
                if (std::find(point.vertices.begin(), point.vertices.end(), v1) == point.vertices.end()) {
                    point.vertices.push_back(v1);
                }
            }
        }
    }

    std::map<int, std::vector<vorpt>> categorized_points;
    for (const auto& point : points) {
        categorized_points[point.vertices.size()].push_back(point);
    }

    std::vector<vorpt> unique_shapes;

    for (const auto& [vertex_count, region_group] : categorized_points) {
        //std::cout << "Počet vrcholů: " << vertex_count << "\n";

        std::vector<vorpt> category_unique_shapes;
		//int r = 0;
        for (const auto& point : region_group) {
            bool is_unique = true;
			//std::cout << "int r:" << r++ << " point: " << point.coords << "\n";

            std::vector<double> distances1;
            for (const auto& vertex : point.vertices) {
                distances1.push_back(eucleid_distance(vertex,point.coords));
            }
            std::sort(distances1.begin(), distances1.end());

            for (const auto& unique_shape : category_unique_shapes) {
                std::vector<double> distances2;
                for (const auto& vertex : unique_shape.vertices) {
                    distances2.push_back(eucleid_distance(vertex,unique_shape.coords));
                }
                std::sort(distances2.begin(), distances2.end());

                double ratio1 = distances1[0] / distances2[0];
                bool same_ratios = true;

                for (size_t k = 1; k < distances1.size(); ++k) {
                    double ratio = distances1[k] / distances2[k];
					//std::cout << ratio << " " << ratio1 << "\n";
                    if (std::abs(ratio - ratio1) > 1e-7) {
                        same_ratios = false;
                        break;
                    }
                }
                if (!same_ratios) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) {
                category_unique_shapes.push_back(point);
            }
        }
        unique_shapes.insert(unique_shapes.end(), category_unique_shapes.begin(), category_unique_shapes.end());
    }

    return unique_shapes;
}



#endif