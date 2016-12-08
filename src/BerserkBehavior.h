/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include <math.h>
#include "Behavior.h"
#include "BerserkTools.h"
#include "PathFinder.h"
#include "MagicSensors.h"
#include "WalkingBehavior.h"

class BerserkBehavior : public WalkingBehavior {
public:
    BerserkBehavior(const std::string& name, const Point& position, double acceptedRadius, double factor,
                    bool berserkMode, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors,
                    Ptr<BerserkTools> berserkTools)
            : WalkingBehavior(name, position, acceptedRadius, finder, sensors),
              mBerserkMode(berserkMode), mBerserkTools(std::move(berserkTools)) {
        this->setFactor(factor);
    }

    void prepare(Ptr<State> state) override {
        CastAction& castAction = this->getCastAction();
        auto& self = state->self;
        this->updateWalkingAction(state);
        if (mBerserkTools->isInStuffRange() && mBerserkTools->isStuffAvailable()) {
            Log(DEBUG) << "SMASH!!!";
            castAction.kickStaff();
        }
    }

    void update(Ptr<State> state) override {
        this->updateProbabilityByDistance({state->self});
    }

private:
    bool isBerserkMode() const {
        return mBerserkMode;
    }

    void setBerserkMode(bool berserkMode) {
        mBerserkMode = berserkMode;
    }

    void protectPosition(Ptr<State> state) {

    }

private:
    bool mBerserkMode;
    Ptr<BerserkTools> mBerserkTools;
};


