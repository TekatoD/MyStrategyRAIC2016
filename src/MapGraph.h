/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once

#include "Point.h"
#include "Graph.h"
#include <unordered_set>
#include <map>
#include <forward_list>
#include <memory>
#include <limits>
#include <algorithm>

class MapGraph : public Graph {
public:

    MapGraph();

    void addPoint(const Point& point);

    void deletePoint(const Point& point);

    void addEdge(Point start, Point end, bool findNearest = false);

    std::vector<std::pair<Point, double>> getNeighbourVertexes(const Point& point) const override ;

    Point getNearestVertex(const Point& point) const override;

    bool isContains(const Point& point) const;


private:
    struct Edge {
        Edge(const Point& start, const Point& end, double distance) :
                start(start), end(end), distance(distance) {}

        Point start;
        Point end;
        double distance;
    };
    std::unordered_set<Point> mWaypoints;
    std::forward_list<Edge> mEdges;
};


