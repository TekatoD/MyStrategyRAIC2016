/**
 * @author arssivka
 * @date 11/26/16
 */

#include "WalkingAction.h"


WalkingAction::WalkingAction() {}


void WalkingAction::perform() {
    auto& targetPoint = mTargetPoint;
    auto& trackingPoint = mTargetPoint;
    double speedFactor = mSpeedFactor;
    // Create aliases
    auto& self = mState->self;
    auto& game = mState->game;
    auto& move = mState->move;
    // Update angles
    auto angleToTarget = self.getAngleTo(targetPoint.getX(), targetPoint.getY());
    auto angleToLook = self.getAngleTo(trackingPoint.getX(), trackingPoint.getY());
    double dir = (angleToLook > 0) ? 1.0 : -1.0;
    bool go_forward = std::abs(angleToLook) <= M_PI_2;
    // Update speed limits
    double max_speed = (go_forward)
                       ? game.getWizardForwardSpeed()
                       : game.getWizardStrafeSpeed();
    double rest = targetPoint.getDistanceTo(self);
    double turnLimit = dir * std::min(std::abs(angleToLook), game.getWizardMaxTurnAngle());
    double speedLimit = std::min(rest, max_speed);
    double strafeLimit = std::min(rest, game.getWizardStrafeSpeed());
    // Update speed
    double turn = turnLimit * speedFactor;
    double speed = cos(angleToTarget) * speedLimit * speedFactor;
    double strafe = sin(angleToTarget) * strafeLimit * speedFactor;
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
