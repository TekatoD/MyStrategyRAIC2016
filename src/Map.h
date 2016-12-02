/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once

#include "Point.h"
#include "Graph.h"
#include <unordered_map>
#include <map>
#include <forward_list>
#include <memory>
#include <limits>
#include <algorithm>

class Map : public Graph {
public:

    Map();

    void addPoint(const std::string& name, Point point);

    void addEdge(const std::string& start, const std::string& end);

    std::vector<std::pair<Point, double>> getNeighbourVertexes(const Point& point) const override ;

    Point getNearestVertex(const Point& point) const override;

    Point getPointByName(const std::string& point) const;
private:



    struct Edge {
        Edge(const std::string& start, const std::string& end, double distance) :
                start(start), end(end), distance(distance) {}

        std::string start;
        std::string end;
        double distance;
    };
    std::unordered_map<std::string, std::shared_ptr<Point>> mWayPoints;
    std::forward_list<Edge> mEdges;
};


