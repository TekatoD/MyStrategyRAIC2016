/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#pragma once


#include <forward_list>
#include "Graph.h"
#include "Path.h"

class PathFinder {
public:
    PathFinder(Ptr<Graph> graph);

    Ptr<Graph> getGraph() const;

    void setGraph(Ptr<Graph> graph);

    virtual Path findPath(const Point& start, const Point& end) = 0;

    virtual ~PathFinder();;

private:
    Ptr<Graph> mGraph;

};

