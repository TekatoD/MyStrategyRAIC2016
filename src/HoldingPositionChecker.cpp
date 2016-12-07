/**
 * @author arssivka
 * @date 12/7/16
 */

#include "HoldingPositionChecker.h"


HoldingPositionChecker::HoldingPositionChecker(int interval, double radiusThreshold)
        : mInterval(interval), mRadiusThreshold(radiusThreshold) {}


int HoldingPositionChecker::getInterval() const {
    return mInterval;
}


void HoldingPositionChecker::setInterval(int interval) {
    mInterval = interval;
}


double HoldingPositionChecker::getRadiusThreshold() const {
    return mRadiusThreshold;
}


void HoldingPositionChecker::setRadiusThreshold(double radiusThreshold) {
    mRadiusThreshold = radiusThreshold;
}


bool HoldingPositionChecker::isHoldingPosition() const {
    return mIsHolding;
}


void HoldingPositionChecker::update(Ptr<State> state) {
    Point self = {state->self};
    auto& world = state->world;
    if (!mPosition.inCircle(self, mRadiusThreshold)) {
        mTick = world.getTickIndex();
        mPosition = self;
    }
    mIsHolding = world.getTickIndex() - mTick > mInterval;
}
