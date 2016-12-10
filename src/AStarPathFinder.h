/**
 *  @autor arssivka
 *  @date 11/30/16
 */
#pragma once


#include "PathFinder.h"

class AStarPathFinder : public PathFinder {
public:
    static const size_t DEFAULT_QUEUE_SIZE;

    AStarPathFinder();

    AStarPathFinder(Ptr<Graph> graph, size_t initialQueueSize = DEFAULT_QUEUE_SIZE);

    Path findPath(const Point& start, const Point& end) override;

    size_t getInitialQueueSize() const;

    void setInitialQueueSize(size_t initialQueueSize);

private:
    size_t mInitialQueueSize;
};

