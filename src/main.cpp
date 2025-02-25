#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <random>
#include <map>
#include "point.h"
#include "tribonacci.h"
#include "beachline.h"
#include "voronoi.h"

void draw_full_voronoi(sf::RenderWindow& window, const std::vector<vorpt>& points, const std::vector<Edge>& edges, const std::vector<Vertex>& vertices) {
    // Draw points
    for (const auto& point : points) {
        sf::CircleShape shape(3); 
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(point.coords.first - shape.getRadius(), point.coords.second - shape.getRadius());
        window.draw(shape);
    }

    // Draw Voronoi edges
    for (const auto& e : edges) {
        if (e.original_edge.first != -1 && e.original_edge.second != -1) { 
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(vertices[e.original_edge.first].center.first, vertices[e.original_edge.first].center.second), sf::Color::Black),
                sf::Vertex(sf::Vector2f(vertices[e.original_edge.second].center.first, vertices[e.original_edge.second].center.second), sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    // Draw vertices
    for (const auto& vertex : vertices) {
        sf::CircleShape shape(2); 
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(vertex.center.first - shape.getRadius(), vertex.center.second - shape.getRadius());
        window.draw(shape);
    }
}


void draw_region(sf::RenderWindow& window, const vorpt& region, const std::vector<Vertex>& vertices) {
    sf::VertexArray lines(sf::Lines);

    // Draw Voronoi edges
    for (const auto& edge : region.edges) {
        if (edge.original_edge.first != -1 && edge.original_edge.second != -1) { 
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(vertices[edge.original_edge.first].center.first, vertices[edge.original_edge.first].center.second), sf::Color::Black),
                sf::Vertex(sf::Vector2f(vertices[edge.original_edge.second].center.first, vertices[edge.original_edge.second].center.second), sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    for (const auto& vertex : region.vertices) {
        sf::CircleShape vertex_shape(3);
        vertex_shape.setFillColor(sf::Color::Green);
        vertex_shape.setPosition(vertex.first - vertex_shape.getRadius(), vertex.second - vertex_shape.getRadius());
        window.draw(vertex_shape);
    }

    sf::CircleShape site_shape(5); 
    site_shape.setFillColor(sf::Color::Blue);
    site_shape.setPosition(region.coords.first - site_shape.getRadius(), region.coords.second - site_shape.getRadius());
    window.draw(site_shape);
}


int main() {
    const int windowWidth = 800;
    const int windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Voronoi Diagrams");

    std::vector<vorpt> points;

    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<point_double> wrongs;

    generate_tribonacci_points(points, windowWidth, windowHeight, -25);
    VoronoiDiagram(points, vertices, edges, wrongs);
    find_new_edges(vertices, edges, wrongs);

    auto unique_shapes = get_unique_shapes(points, vertices);

    int currentRegionIndex = -1;
    bool needsRedraw = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Right) {
                    currentRegionIndex++;
                    if (currentRegionIndex > static_cast<int>(unique_shapes.size())) {
                        currentRegionIndex = -1; 
                    }
                    needsRedraw = true;
                } else if (event.key.code == sf::Keyboard::Left) {
                    currentRegionIndex--;
                    if (currentRegionIndex < -1) {
                        currentRegionIndex = static_cast<int>(unique_shapes.size());
                    }
                    needsRedraw = true;
                }
            }
        }

        if (needsRedraw) {
            window.clear(sf::Color::White);

            if (currentRegionIndex == -1) {
                draw_full_voronoi(window, points, edges, vertices);
            } else {
                const auto& currentRegion = unique_shapes[currentRegionIndex];
                draw_region(window, currentRegion, vertices);
            }

            window.display();
            needsRedraw = false; 
        }
    }

    return 0;
}
