/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#pragma once

#include <vector>
#include "Point.h"
#include "Pointer.h"

class Graph : public Pointer<Graph> {
public:
    Graph() = default;

//    virtual void addPoint(const Point& point) = 0;

//    virtual void addTransition()

    virtual std::vector<std::pair<Point, double>> getNeighbours(const Point& point) const = 0;

    virtual ~Graph() {}

};

