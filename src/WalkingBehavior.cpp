/**
 *  @autor arssivka
 *  @date 12/8/16
 */

#include "WalkingBehavior.h"

WalkingBehavior::WalkingBehavior(const std::string& name, const Point& targetPoint, double acceptedRadius,
                                 const Ptr<PathFinder>& finder, const Ptr<MagicSensors>& sensors)
        : Behavior(name), mTargetPoint(targetPoint), mTrackingTargetPoint(true), mAcceptedRadius(acceptedRadius),
          mFinder(finder), mSensors(sensors) {}

void WalkingBehavior::updateWalkingAction(Ptr<State> state) {
    WalkingAction& walkAction = this->getWalkingAction();
    const auto& self = state->self;
    const Point pos = {self};
    // If wizard transmitted
    if (!mWaypoint.inCircle(pos, mAcceptedRadius)) mUpdateNeeded = true;
    // Update path
    if (mUpdateNeeded) {
        mPath = mFinder->findPath(pos, mTargetPoint);
        mUpdateNeeded = false;
    }

    Point target;
    while (!mPath.isFinished() && pos.inCircle(mPath.current(), mAcceptedRadius)) {
        mWaypoint = mPath.pop();
    }

    if (!mPath.isFinished()) {
        Log(DEBUG) << mPath;
        target = mPath.current();
    } else {
        target = mTargetPoint;
    }

    const double orientation = self.getAngle() + self.getAngleTo(target.getX(), target.getY());
    target =  mSensors->correctTarget(pos, orientation, target);
    walkAction.setSpeedFactor(1.0);
    walkAction.setTargetPoint(target);
    walkAction.setTrackingPoint((mTrackingTargetPoint) ? target : mTrackingPoint);
}
