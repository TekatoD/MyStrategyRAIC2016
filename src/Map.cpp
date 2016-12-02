/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Map.h"

void Map::addPoint(const std::string &name, Point point) {
    auto pointPtr = std::make_shared<Point>(point);
    mWayPoints.insert({name, pointPtr});

}

void Map::addEdge(const std::string &start, const std::string &end) {
    auto startPoint = mWayPoints.find(start);
    auto endPoint = mWayPoints.find(end);
    if(startPoint != mWayPoints.end() && endPoint != mWayPoints.end()) {
        mEdges.push_front(Edge(start, end,
                               startPoint->second->getDistanceTo(endPoint->second->getX(), endPoint->second->getY())));
    }
}

Map::Map() : Graph() {
    this->addPoint("corner_top",       Point(400, 400));
    this->addPoint("corner_bot",       Point(3600, 3600));
    this->addPoint("bonus_bot",        Point(1200.0, 1200.0));
    this->addPoint("bonus_top",        Point(2800.0, 2800.0));
    this->addPoint("base_top",         Point(3600.0 - (74.25 + 70.71), 400.0 + (74.25 + 70.71)));
    this->addPoint("base_bot",         Point(400.0 - (74.25 + 70.71), 3600.0 - (74.25 + 70.71)));
    this->addPoint("tower_left_1",     Point(50.0 + (52 + 50), 2693.2577778083373));
    this->addPoint("tower_left_2",     Point(350.0 - (52 + 50), 1656.7486446626867));
    this->addPoint("tower_mid_bot_2",  Point(1929.2893218813454 - (35.35 + 74.25), 2400.0 - (35.35 + 74.25)));
    this->addPoint("tower_mid_bot_1",  Point(902.6130586671778 + (35.35 + 74.25), 2768.0976194514765 + (35.35 + 74.25)));
    this->addPoint("tower_bot_1",      Point(1370.6603203516029, 3650.0 + (52 + 50)));
    this->addPoint("tower_bot_2",      Point(2312.1259974228437, 3950.0 - (52 + 50)));
    this->addPoint("tower_right_1",    Point(3950.0 - (52 + 50), 1306.7422221916627));
    this->addPoint("tower_right_2",    Point(3650.0 + (52 + 50), 2343.2513553373133));
    this->addPoint("tower_mid_top_1",  Point(3097.386941332822 - (35.35 + 74.25), 1231.9023805485235 - (35.35 + 74.25)));
    this->addPoint("tower_mid_top_2",  Point(2070.710678118655 + (35.35 + 74.25), 1600.0 + (35.35 + 74.25)));
    this->addPoint("tower_top_1",      Point(2629.339679648397, 350.0 - (52 + 50)));
    this->addPoint("tower_top_2",      Point(1687.8740025771563, 50.0 + (52 + 50)));
    this->addPoint("center",           Point(2000.0, 2000.0));

    this->addEdge("base_bot", "tower_left_1");
    this->addEdge("tower_left_1", "tower_left_2");
    this->addEdge("tower_left_2", "corner_top");
    this->addEdge("base_bot", "tower_mid_bot_1");
    this->addEdge("tower_mid_bot_1", "tower_mid_bot_2");
    this->addEdge("tower_mid_bot_2", "center");
    this->addEdge("base_bot", "tower_bot_1");
    this->addEdge("tower_bot_1", "tower_bot_2");
    this->addEdge("tower_bot_2", "corner_bot");
    this->addEdge("base_top", "tower_top_1");
    this->addEdge("tower_top_1", "tower_top_2");
    this->addEdge("tower_top_2", "corner_top");
    this->addEdge("base_top", "tower_mid_top_1");
    this->addEdge("tower_mid_top_1", "tower_mid_top_2");
    this->addEdge("tower_mid_top_2", "center");
    this->addEdge("base_top", "tower_right_1");
    this->addEdge("tower_right_1", "tower_right_2");
    this->addEdge("tower_right_2", "corner_bot");
    this->addEdge("corner_bot", "bonus_bot");
    this->addEdge("bonus_bot", "center");
    this->addEdge("corner_top", "bonus_top");
    this->addEdge("bonus_top", "center");

}

std::vector<std::pair<Point, double>> Map::getNeighbourVertexes(const Point& point) const {
    auto result = std::find_if(mWayPoints.begin(), mWayPoints.end(), [point](const auto& pair){
        return *pair.second == point;
    });
    if (result == mWayPoints.end()) {
        return std::vector<std::pair<Point, double>>{};
    }
    std::vector<std::pair<Point, double>> neighbours;
    for(auto&& edge : mEdges) {
        if(edge.start == result->first) {
            neighbours.push_back({Point(*mWayPoints.find(edge.end)->second), edge.distance});
        }
        else if(edge.end == result->first) {
            neighbours.push_back({Point(*mWayPoints.find(edge.start)->second), edge.distance});
        }
    }
    return std::move(neighbours);
}

Point Map::getNearestVertex(const Point& point) const {
    auto result =std::min_element(mWayPoints.begin(), mWayPoints.end(), [point](const auto& p1,
                                                                              const auto& p2){
        return p1.second->getDistanceTo(point) < p2.second->getDistanceTo(point);
    });
    return *result->second;
}

Point Map::getPointByName(const std::string &point) const {
    auto result = mWayPoints.find(point);
    if (result == mWayPoints.end()) {
        return Point();
    }
    return Point(*result->second);
}
