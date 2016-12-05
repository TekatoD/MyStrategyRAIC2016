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
//            if (mState == nullptr) return;
            mUpdated = true;
            auto& self = mState->self;
            auto radius = self.getRadius();
            const double w = W;
            mGraph = share<GridMapGraph<W, W>>(Point{self}, 2.0 * radius,
                                               Point{ -w * radius, -w * radius});

            mFilter->update(mState);
            this->applyUnits(mFilter->getWizards());
            this->applyUnits(mFilter->getTrees());
            this->applyUnits(mFilter->getBuildings());
            this->applyUnits(mFilter->getMinions());
        }
    }

    template <class T>
    void applyUnits(VectorProxy<T> proxy) const {
        auto& me = mState->self;
        for (auto&& item : proxy) {
            if (me.getId() == item->getId()) continue;
            auto radius = item->getRadius();
            if (radius / mGraph->getCellSize() <= 1.0) {
                mGraph->setCellValue({*item}, 0);
            } else {
                this->drawCircle({*item}, radius, 0);
            }
        }
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


