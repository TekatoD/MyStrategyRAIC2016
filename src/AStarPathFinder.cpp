/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#include "AStarPathFinder.h"
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>

AStarPathFinder::AStarPathFinder(Graph::Ptr graph) : PathFinder(std::move(graph)) {}

std::forward_list<Point> AStarPathFinder::findPath(const Point& start, const Point& end) {
    using Waypoint = std::pair<Point, double>;
    if (start == end) {
        return std::forward_list<Point>{start};
    }
    double dist = 0;
    std::list<Waypoint> q{{{start, dist}}};
    std::unordered_map<Point, double> waypoints;
    bool found = false;
    auto graph = this->getGraph();
    while (!q.empty()) {
        auto current = std::move(q.front());
        q.pop_front();
        auto& pose = std::get<0>(current); //TODO: check
        dist =  std::get<1>(current);
        auto contains = waypoints.find(pose);
        if (contains != waypoints.cend()) {
            waypoints.emplace(current);
        } else if (contains != waypoints.cend() && contains->second > dist) { //Segmantation fault was here
            contains->second = dist;
        }

        if (pose == end) {
            found = true;
            break;
        }
        auto neighbours = graph->getNeighbours(pose);
        for (auto&& neighbour : neighbours) {
            auto place = std::find_if(q.begin(), q.end(), [&neighbour, dist](const auto& waypoint) -> bool {
                return neighbour.second + dist < waypoint.second;
            });
            q.insert(place, {neighbour.first, neighbour.second + dist});
        }
    }

    if (!found) {
        return std::forward_list<Point>{start};
    }

    std::forward_list<Point> path;
    auto current = end;
    while (current != start) {
        path.push_front(current);
        const auto& neighbours = graph->getNeighbours(current);
        auto place = std::accumulate(neighbours.cbegin(), neighbours.cend(), current, [&waypoints, &dist](const Point& acc, const auto& pair) -> Point {
            double len = waypoints[pair.first];
            if (len < dist) {
                dist = len;
                return pair.first;
            } else {
                return acc;
            }
        });
        current = place;

    }
    path.push_front(start);
    return std::move(path);
}


