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
                    double factor, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors);

    void prepare(Ptr<State> state) override;

    void update(Ptr<State> state) override {
    }

private:
    std::vector<Point> mRetreatPoints;
    double mSafeRange;
};


