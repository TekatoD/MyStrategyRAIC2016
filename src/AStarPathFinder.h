/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#pragma once


#include "PathFinder.h"

class AStarPathFinder : public PathFinder {
public:
    AStarPathFinder(size_t initialQueueSize = 32);

    AStarPathFinder(Ptr<Graph> graph, size_t initialQueueSize = 32);

    Path findPath(const Point& start, const Point& end) override;

    size_t getInitialQueueSize() const;

    void setInitialQueueSize(size_t initialQueueSize);

private:
    size_t mInitialQueueSize;
};

