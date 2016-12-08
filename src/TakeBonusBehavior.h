/**
 * @author arssivka
 * @date 12/7/16
 */

#pragma once


#include "Behavior.h"
#include "MagicSensors.h"
#include "PathFinder.h"
#include "WalkingBehavior.h"

class TakeBonusBehavior : public WalkingBehavior {
public:
    TakeBonusBehavior(const std::string& name, const Point& targetPoint, double acceptedRadius,
                    const Ptr<PathFinder>& finder, const Ptr<MagicSensors>& sensors)
            : WalkingBehavior(name, targetPoint, acceptedRadius, finder, sensors) {

    }

    void prepare(Ptr<State> state) override {
        this->updateWalkingAction(state);
    }

    void update(Ptr<State> state) override {
        this->setProbability(1.0);
    }
};


