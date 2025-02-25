#ifndef GEOMETRY_H
#define GEOMETRY_H

// dodatečné funkce
double sq(double x){ 
	return x*x; 
}
double size(point_double p){ 
	return hypot(p.first, p.second); 
}
double sz2(point_double p){ 
	return sq(p.first) + sq(p.second); 
}

// průnik hran
point_double line_intersect(point_double a, point_double b, point_double u, point_double v){ 
	return u + (((a-u)/b) / (v/b))*v; 
}

double parabola_intersect(point_double left, point_double right, double sweepline) {
    if (fabs(left.second - right.second) < fabs(left.first - right.first) * EPS) {
        point_double midpoint = 0.5 * (left + right);
        point_double direction = point_double(-(right.second - left.second), right.first - left.first);
        point_double vertical_intersection = line_intersect(midpoint, direction, point_double(0, sweepline), point_double(1, 0));
        double delta_x = size(left - vertical_intersection);
        double l1 = size(vertical_intersection - midpoint);
        double l2 = sqrt(std::max(0.0, sq(sweepline - midpoint.second) - sq(size(midpoint - vertical_intersection))));
        return vertical_intersection.first + (left.first < right.first ? delta_x : -delta_x) / (l1 + l2);
    }
    point_double vertical_intersection = line_intersect(left, right - left, point_double(0, sweepline), point_double(1, 0));
    double midpoint_distance_squared = 0.25 * sq(size(left - right));
    double sweep_distance_squared = sq(size(0.5 * (left + right) - vertical_intersection));
    double adjustment = sqrt(std::max(0.0, sweep_distance_squared - midpoint_distance_squared));
    return vertical_intersection.first + (left.second < right.second ? -adjustment : adjustment);
}

point_double get_circumcenter(point_double p0, point_double p1, point_double p2) {
    point_double direction1 = point_double(-(p1.second - p0.second), p1.first - p0.first);
    point_double direction2 = point_double(-(p2.second - p1.second), p2.first - p1.first);
    return line_intersect(0.5 * (p0 + p1), direction1, 0.5 * (p1 + p2), direction2);
}

// střed kružnice pro manhattanskou metriku
point_double get_circumcenter2(point_double p0, point_double p1, point_double p2) {
    std::vector<point_double> points = {p0, p1, p2};
    std::vector<std::pair<double, std::pair<int, int>>> distances;
	double center_x,center_y;

    
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            double dist = manhattan_distance(points[i], points[j]) / 2.0;
            distances.push_back({dist, {i, j}});
        }
    }

    // Sort distances from shortest to longest
    std::sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // Extract pairs and calculate differences
    auto pair1 = distances[0].second;
    auto pair2 = distances[1].second;
    
    // X and Y differences of the points
    double diffX1 = std::abs(points[pair1.first].first - points[pair1.second].first);
    double diffY1 = std::abs(points[pair1.first].second - points[pair1.second].second);

    if (diffX1 < diffY1) {
        // jsou blíž v X-ku takže počítají Y
        point_double point1 = points[pair1.first];
        point_double point2 = points[pair1.second];
	if (point1.first > point2.first) {
	    std::swap(point1, point2);
	}
        center_y = (point1.second > point2.second) ? point2.second + distances[0].first : 
        point2.second - distances[0].first;

        // Calculate center_x for pair2
        point_double point3 = points[pair2.first];
        point_double point4 = points[pair2.second];
        if (std::abs(point3.second - center_y) < std::abs(point4.second - center_y)) {
            std::swap(point3, point4);
        }
        center_x = (point3.first > point4.first) ? point4.first + distances[1].first : 
        point4.first - distances[1].first;

		//dodatek
		if (std::abs(point1.first - center_x) < std::abs(point2.first - center_x)) {
            std::swap(point1, point2);
        }
		center_y = (point1.second > point2.second) ? point2.second + distances[0].first : 
        point2.second - distances[0].first;
    } else {
        // jsou blíž v Y-ku takže počítaji X
        point_double point1 = points[pair1.first];
        point_double point2 = points[pair1.second];
        if (point1.second > point2.second) {
            std::swap(point1, point2);
        }
        // znamenko
        center_x = (point1.first > point2.first) ? point2.first + distances[0].first : 
        point2.first - distances[0].first;
    	
    	// kalkuluju Y
        point_double point3 = points[pair2.first];
        point_double point4 = points[pair2.second];
        if (std::abs(point3.first - center_x) < std::abs(point4.first - center_x)) {
            std::swap(point3, point4);
        }
        // znamenko
        center_y = (point3.second > point4.second) ? point4.second + distances[1].first : 
        point4.second - distances[1].first;

		//dodatek
		if (std::abs(point1.second - center_y) < std::abs(point2.second - center_y)) {
            std::swap(point1, point2);
        }
		center_x = (point1.first > point2.first) ? point2.first + distances[0].first : 
        point2.first - distances[0].first;
    }
	return {center_x,center_y};
}

#endif