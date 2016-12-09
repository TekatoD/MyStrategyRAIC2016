/**
 *  @autor arssivka
 *  @date 12/8/16
 */

#pragma once


#include "Behavior.h"
#include "PathFinder.h"
#include "MagicSensors.h"
#include "Log.h"

class WalkingBehavior : public Behavior {
protected:
    WalkingBehavior(const std::string& name, const Point& targetPoint, double acceptedRadius,
                    const Ptr<PathFinder>& finder, const Ptr<MagicSensors>& sensors);
    void finalize() override {
        mUpdateNeeded = true;
    }

protected:
    void updateWalkingAction(Ptr<State> state);

    void stop(Ptr<State> state) {
        WalkingAction& walkAction = this->getWalkingAction();
        walkAction.setTargetPoint({state->self});
    }

    const Point& getTargetPoint() const {
        return mTargetPoint;
    }

    void setTargetPoint(const Point& targetPoint) {
        mUpdateNeeded = true;
        mTargetPoint = targetPoint;
    }

    const Point& getTrackingPoint() const {
        return mTrackingPoint;
    }

    void setTrackingPoint(const Point& trackingPoint) {
        mTrackingTargetPoint = false;
        mTrackingPoint = trackingPoint;
    }

    void enableTrackingTargetPoint() {
        mTrackingTargetPoint = true;
    }


    Ptr<PathFinder> getFinder() const {
        return mFinder;
    }

    Ptr<MagicSensors> getSensors() const {
        return mSensors;
    }

    double getAcceptedRadius() const {
        return mAcceptedRadius;
    }

    void setAcceptedRadius(double acceptedRadius) {
        mUpdateNeeded = true;
        mAcceptedRadius = acceptedRadius;
    }

    void updateProbabilityByDistance(Point origin) {
        this->setProbability(1.0 / mTargetPoint.getDistanceTo(origin));
    }

private:
    bool mTrackingTargetPoint;
    bool mUpdateNeeded;
    Point mWaypoint;
    Path mPath;
    Point mTargetPoint;
    Point mTrackingPoint;
    double mAcceptedRadius;
    Ptr<PathFinder> mFinder;
    Ptr<MagicSensors> mSensors;
};


