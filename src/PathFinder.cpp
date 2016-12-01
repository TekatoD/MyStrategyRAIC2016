/**
 *  @autor tekatod
 *  @date 11/30/16
 */
#include "PathFinder.h"


PathFinder::PathFinder(Graph::Ptr graph) : mGraph(std::move(graph)) { }


Graph::Ptr PathFinder::getGraph() const {
    return mGraph;
}


void PathFinder::setGraph(Graph::Ptr graph) {
    mGraph = std::move(graph);
}


PathFinder::~PathFinder() {}
