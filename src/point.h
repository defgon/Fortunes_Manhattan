#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

// zavedení bodu
typedef std::pair<double, double> point_double;

// operátory pro bod
double operator / (point_double a,    point_double b){ 
	return a.first * b.second - a.second * b.first; 
}
point_double operator * (double b, point_double a){
	return point_double(b * a.first, b * a.second); 
}
point_double operator + (point_double a,    point_double b){
	return point_double(a.first + b.first, a.second + b.second); 
}
point_double operator - (point_double a,    point_double b){ 
	return point_double(a.first - b.first, a.second - b.second); 
}
std::ostream& operator<<(std::ostream& os, const point_double& a) {
    os << a.first << " " << a.second;
    return os;
}

#endif