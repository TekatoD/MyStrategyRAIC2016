/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "MapGraph.h"
#include "GridMapGraph.h"
#include "PathFinder.h"
#include "Mechanism.h"
#include "ObstaclesGridMaker.h"


template <size_t W>
class Walker : public Mechanism {
public:
    Walker(Ptr<MapGraph> map, Ptr<ObstaclesGridMaker<W>> maker,
           Ptr<PathFinder> globalFinder, Ptr<PathFinder> localFinder)
            : mGridMaker(std::move(maker)),
              mGlobalFinder(std::move(globalFinder)),
              mLocalFinder(std::move(localFinder)) {
        mGlobalFinder->setGraph(map);
    }

    Ptr<PathFinder> getGlobalFinder() const {
        return mGlobalFinder;
    }

    Ptr<PathFinder> getLocalFinder() const {
        return mLocalFinder;
    }

    void setLocalFinder(Ptr<PathFinder> pathFinder) {
        mLocalFinder = std::move(pathFinder);
    }

    void setGlobalFinder(Ptr<PathFinder> pathFinder) {
        mGlobalFinder = std::move(pathFinder);
    }

    Path findPath(Point from, Point to) {
        auto grid = mGridMaker->getGraph();
        mLocalFinder->setGraph(grid);
        Path path;
        if (!grid->isContainsPoint(to)) {
            path = mGlobalFinder->findPath(from, to);
            if (path.isFinished()) return std::move(path);
            to = grid->getNearestPoint(path.pop());
        }
        return mLocalFinder->findPath(from, to);
    }

    void update(Ptr<State> state) override {
        mGridMaker->update(state);
    }


private:
    Ptr<PathFinder> mGlobalFinder;
    Ptr<PathFinder> mLocalFinder;
    Ptr<ObstaclesGridMaker<W>> mGridMaker;
};


