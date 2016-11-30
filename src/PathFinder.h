/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#pragma once


#include <forward_list>
#include "Graph.h"

class PathFinder : public Pointer<PathFinder> {
public:
    PathFinder(Graph::Ptr graph) : mGraph(std::move(graph)) { }

    Graph::Ptr getGraph() const {
        return mGraph;
    }

    void setGraph(Graph::Ptr graph) {
        mGraph = std::move(graph);
    }

    virtual std::forward_list<Point> findPath(const Point& start, const Point& end) = 0;

    virtual ~PathFinder() {};

private:
    Graph::Ptr mGraph;

};

