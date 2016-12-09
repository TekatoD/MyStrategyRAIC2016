/**
 * @author arssivka
 * @date 11/26/16
 */

#include "WalkingAction.h"


WalkingAction::WalkingAction() {}


void WalkingAction::perform() {
    const auto& targetPoint = mTargetPoint;
    const auto& trackingPoint = mTrackingPoint;
    const double speedFactor = mSpeedFactor;
    // Create aliases
    const auto& self = mState->self;
    const auto& game = mState->game;
    auto& move = mState->move;
    // Update angles
    const auto angleToTarget = self.getAngleTo(targetPoint.getX(), targetPoint.getY());
    const auto angleToLook = self.getAngleTo(trackingPoint.getX(), trackingPoint.getY());
    const double dir = (angleToLook > 0) ? 1.0 : -1.0;
    const bool goForward = std::abs(angleToTarget) <= M_PI_2;
    // Update speed limits
    double maxSpeed = (goForward)
                       ? game.getWizardForwardSpeed()
                       : game.getWizardStrafeSpeed();
    const double rest = targetPoint.getDistanceTo(self);
    const double turnLimit = dir * std::min(std::abs(angleToLook), game.getWizardMaxTurnAngle());
    const double speedLimit = std::min(rest, maxSpeed);
    const double strafeLimit = std::min(rest, game.getWizardStrafeSpeed());
    // Update speed
    const double turn = turnLimit * speedFactor;
    const double speed = cos(angleToTarget) * speedLimit * speedFactor;
    const double strafe = sin(angleToTarget) * strafeLimit * speedFactor;
    // Apply data
    move.setTurn(turn);
    move.setSpeed(speed);
    move.setStrafeSpeed(strafe);
}


const Point& WalkingAction::getTargetPoint() const {
    return mTargetPoint;
}


void WalkingAction::setTargetPoint(const Point& targetPoint) {
    mTargetPoint = targetPoint;
}


const Point& WalkingAction::getTrackingPoint() const {
    return mTrackingPoint;
}


void WalkingAction::setTrackingPoint(const Point& trackingPoint) {
    mTrackingPoint = trackingPoint;
}


double WalkingAction::getSpeedFactor() const {
    return mSpeedFactor;
}


void WalkingAction::setSpeedFactor(double speedFactor) {
    if (speedFactor < 0.0) {
        speedFactor = 0.0;
    } else if (speedFactor > 1.0) {
        speedFactor = 1.0;
    }
    mSpeedFactor = speedFactor;
}
