#ifndef TRIBONACCI_H
#define TRIBONACCI_H

#include <iostream>
#include <vector>
#include <complex>
#include "const.h"
#include "vorpt.h"

bool in_interval(float z_cara, std::complex<float> z , float gamma_cara, int n) {
    float upper_bound = n / (1 - gamma_cara);
    return (z_cara >= 0) && (z_cara < upper_bound) && (std::abs(z) < 5);
}

void generate_tribonacci_points(std::vector<vorpt>& points, int window_width, int window_height, double angle) {
    // alespoň jedna hodnota kladná
    std::pair<int, int> interval_a = {-5, 5};
    std::pair<int, int> interval_b = {-5, 5};
    std::pair<int, int> interval_c = {-5, 5};

    std::complex<float> z;
    float z_cara;
    std::complex<float> gamma = {-0.7718445, 1.11514250};
    float gamma_cara = 0.5436890;
    int n = 2;

    int step = 1;
    float max_x = std::numeric_limits<float>::min();
    float max_y = std::numeric_limits<float>::min();
    float min_x = std::numeric_limits<float>::max();
    float min_y = std::numeric_limits<float>::max();

    // Generate points and find min/max
    for (int k = interval_a.first; k <= interval_a.second; k += step) {
        for (int l = interval_b.first; l <= interval_b.second; l += step) {
            for (int m = interval_c.first; m <= interval_c.second; m += step) {
                z_cara = k + (float)l * gamma_cara + (float)m * std::pow(gamma_cara, 2);
                z = std::complex<float>(k) + std::complex<float>(l) * gamma + std::complex<float>(m) * gamma * gamma;
                //std::cout << z.real() << " " << z.imag() << "\n";
                if (in_interval(z_cara, z, gamma_cara, n)) {
                    max_x = std::max(max_x, z.real());
                    min_x = std::min(min_x, z.real());
                    max_y = std::max(max_y, z.imag());
                    min_y = std::min(min_y, z.imag());
					double alpha = angle * PI / 180;
                    vorpt point;
					if(std::abs(z.real())<= EPS && std::abs(z.imag()) <= EPS){
                        point.coords = std::make_pair(z.real(), z.imag());
                    	points.push_back(point);
					} else {
						double x_r = z.real() * std::cos(alpha) - z.imag() * std::sin(alpha);
						double x_i = z.real() * std::sin(alpha) + z.imag() * std::cos(alpha);
                        point.coords = std::make_pair(x_r, x_i);
                    	points.push_back(point);
					}
                }
            }
        }
    }

    // Scale and translate points to fit within the window
    std::vector<vorpt> scaled_points;
    float range_x = max_x - min_x;
    float range_y = max_y - min_y;
    for (auto& point : points) {
        double scaled_x = ((point.coords.first - min_x) / range_x) * window_width;
        double scaled_y = ((point.coords.second - min_y) / range_y) * window_height;
        vorpt sc_point;
        sc_point.coords = std::make_pair(scaled_x, scaled_y);
        scaled_points.push_back(sc_point);
    }

    points = scaled_points;
}

void generate_points_in_grid(std::vector<vorpt>& points, int width, int height, double rect_width, double rect_height) {
    int num_cols = static_cast<int>(width / rect_width);
    int num_rows = static_cast<int>(height / rect_height);

    // Random device and generator for shift calculations
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-50, 25);

    // Base offsets to center the grid in the window
    double offsetX = (width - (num_cols * rect_width)) / 2.0 + rect_width / 2.0;
    double offsetY = (height - (num_rows * rect_height)) / 2.0 + rect_height / 2.0;

    double currentShift = 0;

    for (int i = 0; i < num_rows; ++i) {
        currentShift += dis(gen);
        for (int j = 0; j < num_cols; ++j) {
            double x = j * rect_width + offsetX + currentShift;
            double y = i * rect_height + offsetY;
            vorpt point;
            point.coords = {x,y};
            points.emplace_back(point);
        }
    }
}

#endif