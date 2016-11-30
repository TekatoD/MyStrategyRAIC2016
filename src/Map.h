/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once

#include "Point.h"
#include <unordered_map>
#include <map>
#include <forward_list>
#include <memory>
#include <limits>
#include <algorithm>

class Map {
public:
    Map();

    void addPoint(const std::string& name, Point point);

    void addEdge(const std::string& start, const std::string& end);
private:

    template <class T>
    std::string toVertex(T point) {
        Point tmp{point};
        auto result =std::min_element(mWayPoints.begin(), mWayPoints.end(), [tmp](std::pair<std::string, std::shared_ptr<Point>>& p1,
                                                                                  std::pair<std::string, std::shared_ptr<Point>>& p2){
            return p1.second->getDistanceTo(tmp) < p2.second->getDistanceTo(tmp);
        });
        return result->first;
    }

    std::string toVertex(const std::string& point);


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


