/**
 * @author arssivka
 * @date 12/3/16
 */

#pragma once


#include "Mechanism.h"
#include "GridMapGraph.h"
#include "WorldFilter.h"

template <size_t W>
class ObstaclesGridMaker : public Mechanism {
public:
    void update(Ptr<State> state) override {
        if (mState != state) {
            mState = state;
            mUpdated = false;
        }
    }

    Ptr<GridMapGraph<W, W>> getGraph() const {
        this->updateGraph();
        return mGraph;
    }

    ObstaclesGridMaker(const Ptr<WorldFilter>& filter)
            : mFilter(filter), mUpdated(false) {}

private:
    void updateGraph() const {
        if (!mUpdated) {
            mUpdated = true;
            auto& me = mState->self;
            mGraph = share<GridMapGraph<W, W>>(Point{me}, 2.0 * me.getRadius(), Point{-W / 2, -W / 2});

            mFilter->update(mState);
            this->applyUnits(mFilter->getWizards());
            this->applyUnits(mFilter->getTrees());
            this->applyUnits(mFilter->getBuildings());
            this->applyUnits(mFilter->getMinions());
        }
    }

    template <class T>
    void applyUnits(VectorProxy<T> proxy) const {
        size_t size = proxy.getSize();
        for (size_t i = 0; i < size; ++i) this->drawCircle({proxy[i]}, proxy[i].getRadius(), 0);
    }

    void drawCircle(const Point& center, double radius, int value) const {
        double cx = center.getX();
        double cy = center.getY();
        double x = radius;
        double y = 0;
        double err = 0;
        const double step = mGraph->getCellSize();
        while (x >= y) {
            mGraph->setCellValue({x + cx, y + cy}, value);
            mGraph->setCellValue({x + cx, y + cy}, value);
            mGraph->setCellValue({y + cx, x + cy}, value);
            mGraph->setCellValue({-x + cx, y + cy}, value);
            mGraph->setCellValue({-y + cx, x + cy}, value);
            mGraph->setCellValue({-x + cx, -y + cy}, value);
            mGraph->setCellValue({-y + cx, -x + cy}, value);
            mGraph->setCellValue({x + cx, -y + cy}, value);
            mGraph->setCellValue({y + cx, -x + cy}, value);

            y += step;
            if (2.0 * (err - x) + step > 0)
            {
                x -= step;
                err += step - 2 * x;
            }
        }
    }

private:
    Ptr<WorldFilter> mFilter;
    Ptr<State> mState;
    mutable Ptr<GridMapGraph<W, W>> mGraph;
    mutable bool mUpdated;
};


