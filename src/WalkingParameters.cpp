/**
 * @author arssivka
 * @date 11/26/16
 */

#include "WalkingParameters.h"


WalkingParameters::WalkingParameters(double speedFactor, const Point& targetPoint, const Point& trackingPoint)
        : mSpeedFactor(speedFactor), mTargetPoint(targetPoint), mTrackingPoint(trackingPoint) {}


WalkingParameters::WalkingParameters() : mSpeedFactor(0.0), mTargetPoint(), mTrackingPoint() {}


double WalkingParameters::getSpeedFactor() const {
    return mSpeedFactor;
}


void WalkingParameters::setSpeedFactor(double speedFactor) {
    if (speedFactor < 0.0) {
        speedFactor = 0.0;
    } else if (speedFactor > 1.0) {
        speedFactor = 1.0;
    }
    mSpeedFactor = speedFactor;
}


const Point& WalkingParameters::getTargetPoint() const {
    return mTargetPoint;
}


void WalkingParameters::setTargetPoint(const Point& targetPoint) {
    mTargetPoint = targetPoint;
}


const Point& WalkingParameters::getTrackingPoint() const {
    return mTrackingPoint;
}


void WalkingParameters::setTrackingPoint(const Point& trackingPoint) {
    mTrackingPoint = trackingPoint;
}
