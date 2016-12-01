/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#pragma once


#include "PathFinder.h"

class AStarPathFinder : public PathFinder {
public:
    AStarPathFinder(Graph::Ptr graph);

    std::forward_list<Point> findPath(const Point& start, const Point& end) override;

};

