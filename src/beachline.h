#ifndef BEACHLINE_H
#define BEACHLINE_H

#include <iostream>
#include <vector>
#include "test.h"
#include "const.h"
#include "geometry.h"
#include "vorpt.h"

class Beachline {
public:
    // Node of the beachline
    struct node {
        node() {}
        node(point_double point, int index)
            : point(point), index(index), end(0), previous(nullptr), next(nullptr) {}

        point_double point;
        int index;
        int end;
        node* previous;
        node* next;
    };

    node* head;
    node* tail;
    double sweepline;

    Beachline() : sweepline(-1e20), head(nullptr), tail(nullptr) {}

    void insert(node* new_node, node* prev_node, bool after) {
        if (!prev_node) {
            if (head) {
                if (after) {
                    new_node->next = head;
                    head->previous = new_node;
                    head = new_node;
                } else {
                    new_node->next = head;
                    head->previous = new_node;
                    head = new_node;
                }
            } else {
                head = tail = new_node;
            }
            return;
        }

        if (after) {
            new_node->next = prev_node->next;
            new_node->previous = prev_node;
            if (prev_node->next) prev_node->next->previous = new_node;
            prev_node->next = new_node;
            if (prev_node == tail) tail = new_node;
        } else {
            new_node->previous = prev_node->previous;
            new_node->next = prev_node;
            if (prev_node->previous) prev_node->previous->next = new_node;
            prev_node->previous = new_node;
            if (prev_node == head) head = new_node;
        }
    }

    // Erase a node from the doubly linked list
    void erase(node* n) {
        if (!n) return;

        if (n->previous) n->previous->next = n->next;
        if (n->next) n->next->previous = n->previous;
        if (n == head) head = n->next;
        if (n == tail) tail = n->previous;

        n->previous = nullptr;
        n->next = nullptr;
    }

    bool get_event(node* current, double& next_sweep) {
        if (!current->previous || !current->next) return false;

        point_double u = point_double(current->point.second - current->previous->point.second, current->previous->point.first - current->point.first);
        point_double v = point_double(current->next->point.second - current->point.second, current->point.first - current->next->point.first);

        if ((u.first * v.second - u.second * v.first) < EPS) return false;

        point_double p = get_circumcenter(current->point, current->previous->point, current->next->point);
        next_sweep = p.second + size(p - current->point);
        return true;
    }

    node* find(double x) {
        node* current = head;
        while (current) {
            double left = current->previous ? parabola_intersect(current->previous->point, current->point, sweepline): -1e30;
            double right = current->next ? parabola_intersect(current->point, current->next->point, sweepline) : 1e30;
            if (left <= x && x <= right) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
};

using BeachNode = Beachline::node;

static BeachNode* arr;
static int sz;
static BeachNode* new_node(point_double point, int index) {
    arr[sz] = BeachNode(point, index);
    return arr + (sz++);
}

// Definition of event
struct event {
    event(double sweep, int index)
        : type(0), sweep(sweep), index(index) {}

    event(double sweep, BeachNode* current)
        : type(1), sweep(sweep),
          previous(current->previous ? current->previous->index : -1),
          current(current),
          next(current->next ? current->next->index : -1) {}

    int type;
    int index;
    int previous;
    int next;
    BeachNode* current;
    double sweep;

    bool operator>(const event& l) const {
        return sweep > l.sweep;
    }
};

#endif