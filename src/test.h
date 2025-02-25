#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <cmath>
#include "const.h"

// manhattanská vzdálenost
double manhattan_distance(const point_double& p1, const point_double& p2) {
    return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
}

// eukleidovská vzdálenost
double eucleid_distance(const point_double& p1, const point_double& p2) {
    return std::sqrt((p2.first - p1.first) * (p2.first - p1.first) + (p2.second - p1.second) * (p2.second - p1.second));
}

bool center_check(point_double center, point_double p1, point_double p2, point_double p3) {
    double dist1 = std::round(manhattan_distance(center, p1));
    double dist2 = std::round(manhattan_distance(center, p2));
    double dist3 = std::round(manhattan_distance(center, p3));

    if (std::abs(dist1 - dist2) < EPS && std::abs(dist2 - dist3) < EPS) {
		//std::cout << " dist1: " << dist1 << " dist2: " << dist2 << " dist3: " << dist3 << "\n";
        //std::cout << "Stejne daleko\n";
		return true;
    } else {
        //std::cout << "Blbe\n";
    }
	return false;
}


// Function to calculate the angle between three points
bool calculateAngles(const point_double& p0, const point_double& p1, const point_double& p2) {
    double tol = 0.01;
    double a = eucleid_distance(p1, p2);  
    double b = eucleid_distance(p0, p2);  
    double c = eucleid_distance(p0, p1);  

    double angleP0 = acos((b*b + c*c - a*a) / (2*b*c));
    double angleP1 = acos((a*a + c*c - b*b) / (2*a*c));
    double angleP2 = acos((a*a + b*b - c*c) / (2*a*b));

    angleP0 = angleP0 * 180.0 / PI;
    angleP1 = angleP1 * 180.0 / PI;
    angleP2 = angleP2 * 180.0 / PI;

    // Output the angles
	if((std::fmod(angleP0, 45) <= tol || std::fmod(angleP0, 45) <= tol || std::fmod(angleP0, 45) <= tol) &&
        angleP0 >= 1e-4 && angleP1 >= 1e-4 && angleP2 >= 1e-4){
		// std::cout << "Angle at p0: " << angleP0 << " degrees\n";
		// std::cout << "Angle at p1: " << angleP1 << " degrees\n";
		// std::cout << "Angle at p2: " << angleP2 << " degrees\n";
		//std::cout << "Wrong angle! \n";
		return false;
	}

	return true;
}

#endif