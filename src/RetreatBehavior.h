/**
 *  @autor arssivka
 *  @date 12/8/16
 */

#pragma once


#include "Behavior.h"
#include "PathFinder.h"
#include "MagicSensors.h"
#include "WalkingBehavior.h"
#include "Clusterer.h"
#include "model/Unit.h"

class RetreatBehavior : public WalkingBehavior {
public:
    RetreatBehavior(const std::string& name, std::vector<Point> retreatPoints, double acceptedRadius, double safeRange,
                    double factor, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors)
            : WalkingBehavior(name, retreatPoints[0], acceptedRadius, std::move(finder), std::move(sensors)),
              mRetreatPoints(std::move(retreatPoints)),
              mSafeRange(safeRange) {
        this->setFactor(factor);
        this->setProbability(1.0);
    }

    void prepare(Ptr<State> state) override {
        const auto& self = state->self;
        const auto& world = state->world;
        const auto& wizards = world.getWizards();
        const auto& minions = world.getMinions();
        std::unique_ptr<model::Unit> danger;
        bool found = false;
        Point position = {self};
        for (auto&& w : wizards) {
            if (w.getFaction() == self.getFaction()) continue;
            if (position.inCircle({w}, mSafeRange + w.getRadius())) {
                found = true;
                danger = std::make_unique<model::Unit>(w);
                break;
            }
        }
        if (!found) {
            for (auto&& m : minions) {
                if (   m.getFaction() == self.getFaction()
                    || m.getFaction() == model::FACTION_NEUTRAL) continue;
                if (position.inCircle({m}, mSafeRange + m.getRadius())) {
                    found = true;
                    danger = std::make_unique<model::Unit>(m);
                    break;
                }
            }
        }
        if (found) {
            auto nearest = std::min_element(mRetreatPoints.begin(), mRetreatPoints.end(), [&danger](auto point1, auto point2){
                return abs(danger->getAngleTo(point1.getX(), point1.getY())) < abs(danger->getAngleTo(point2.getX(), point2.getY()));
            });
            this->setTargetPoint(*nearest);
            this->updateWalkingAction(state);
        } else {
            this->stop(state);
        }
    }

    void update(Ptr<State> state) override {
    }

private:
    std::vector<Point> mRetreatPoints;
    double mSafeRange;
};


