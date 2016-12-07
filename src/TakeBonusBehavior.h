/**
 * @author arssivka
 * @date 12/7/16
 */

#pragma once


#include "Behavior.h"
#include "MagicSensors.h"
#include "PathFinder.h"

class TakeBonusBehavior : public Behavior {
public:
    TakeBonusBehavior(const std::string& name, const Ptr<PathFinder>& finder, const Ptr<MagicSensors>& sensors,
                      double factor);

    void turn(Ptr<State> state) override;

    void update(Ptr<State> state) override;

private:
    Point correctTarget(Ptr<State> state, const Point& target) const;

private:
    Ptr<PathFinder> mFinder;
    Ptr<MagicSensors> mSensors;
    Point mTarget;
};


