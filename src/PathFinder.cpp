/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#include "PathFinder.h"


PathFinder::PathFinder(Ptr<Graph> graph) : mGraph(std::move(graph)) { }


Ptr<Graph> PathFinder::getGraph() const {
    return mGraph;
}


void PathFinder::setGraph(Ptr<Graph> graph) {
    mGraph = std::move(graph);
}


PathFinder::~PathFinder() {}
