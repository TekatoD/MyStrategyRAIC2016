/**
 * @author arssivka
 * @date 11/25/16
 */


#include "Map.h"


Map::Map()
        : Graph() {}

void Map::addPoint(const Point& point) {
    mWaypoints.insert(point);
}

void Map::deletePoint(const Point& point) {
    mWaypoints.erase(point);
}

void Map::addEdge(Point start, Point end, bool findNearest) {
    if (mWaypoints.find(start) == mWaypoints.end()) {
        if (findNearest) {
            start = this->getNearestVertex(start);
        } else {
            mWaypoints.insert(start);
        }
    }
    if (mWaypoints.find(end) == mWaypoints.end()) {
        if (findNearest) {
            start = this->getNearestVertex(end);
        } else {
            mWaypoints.insert(end);
        }
    }
    mEdges.push_front({start, end, start.getDistanceTo(end)});
}

std::vector<std::pair<Point, double>> Map::getNeighbourVertexes(const Point& point) const {
    auto found = mWaypoints.find(point);
    if (found == mWaypoints.end()) {
        return std::vector<std::pair<Point, double>>{};
    }
    std::vector<std::pair<Point, double>> neighbours;
    for(auto&& edge : mEdges) {
        if(edge.start == *found) {
            neighbours.push_back({Point(*mWaypoints.find(edge.end)), edge.distance});
        }
        else if(edge.end == *found) {
            neighbours.push_back({Point(*mWaypoints.find(edge.start)), edge.distance});
        }
    }
    return std::move(neighbours);
}

Point Map::getNearestVertex(const Point& point) const {
    auto result = std::min_element(mWaypoints.begin(), mWaypoints.end(),
                                   [&point](const auto& p1, const auto& p2){
        return p1.getDistanceTo(point) < p2.getDistanceTo(point);
    });
    return *result;
}

bool Map::isContains(const Point& point) const {
    return mWaypoints.find(point) != mWaypoints.end();

}
