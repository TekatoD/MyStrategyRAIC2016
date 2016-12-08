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

class RetreatBehavior : public WalkingBehavior {
public:
    RetreatBehavior(const std::string& name, const Point& targetPoint, double acceptedRadius, double safeRange,
                    double factor, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors)
            : WalkingBehavior(name, targetPoint, acceptedRadius, std::move(finder), std::move(sensors)),
              mSafeRange(safeRange) {
        this->setFactor(factor);
    }

    void prepare(Ptr<State> state) override {
        const auto& self = state->self;
        const auto& world = state->world;
        const auto& wizards = world.getWizards();
        const auto& minions = world.getMinions();
        bool found = false;
        Point position = {self};
        for (auto&& w : wizards) {
            if (w.getFaction() == self.getFaction()) continue;
            if (position.inCircle({w}, mSafeRange + w.getRadius())) {
                found = true;
                break;
            }
        }
        if (!found) {
            for (auto&& m : minions) {
                if (   m.getFaction() == self.getFaction()
                    || m.getFaction() == model::FACTION_NEUTRAL) continue;
                if (position.inCircle({m}, mSafeRange + m.getRadius())) {
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            this->updateWalkingAction(state);
        } else {
            this->stop(state);
        }
    }

    void update(Ptr<State> state) override {
        this->setProbability(1.0);
    }

private:
    double mSafeRange;
};


