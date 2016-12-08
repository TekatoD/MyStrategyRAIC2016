/**
 * @author arssivka
 * @date 12/7/16
 */

#include "TakeBonusBehavior.h"
#include <cmath>


TakeBonusBehavior::TakeBonusBehavior(const std::string& name, const Ptr<PathFinder>& finder,
                                     const Ptr<MagicSensors>& sensors, double factor)
        : Behavior(name), mFinder(finder), mSensors(sensors) {
    this->setFactor(factor);
}


void TakeBonusBehavior::turn(Ptr<State> state) {
    auto& walk = this->getWalkingAction();
    walk.setTargetPoint(mTarget);
    walk.setTrackingPoint(mTarget);
    walk.setSpeedFactor(1.0);
    Behavior::turn(state);
}


void TakeBonusBehavior::update(Ptr<State> state) {
    const auto& world = state->world;
    const auto& self = state->self;
    const auto& bonuses = world.getBonuses();
    // TODO: Wizards check
    if (bonuses.empty()) {
        this->setProbability(0.0);
        return;
    }
    this->setProbability(1.0);
    Point position = {*std::min_element(bonuses.cbegin(), bonuses.end(), [&self](const auto& b1, const auto& b2) {
        return self.getDistanceTo(b1) < self.getDistanceTo(b2);
    })};
    Path path = mFinder->findPath({self}, position);
    if (!path.isFinished()) path.pop();
    if (path.countVertex() <= 1) {
        mTarget = this->correctTarget(state, position);
    } else {
        mTarget = this->correctTarget(state, path.current());
    }
}


Point TakeBonusBehavior::correctTarget(Ptr<State> state, const Point& target) const {
    const auto& self = state->self;
    auto angle = self.getAngle() + self.getAngleTo(target.getX(), target.getY());
    auto corrected = mSensors->correctDirection(angle, M_PI_2);
    Point result = {target};
    if (angle != corrected) {
        Point local = target - Point{self};
        auto csin = sin(angle - corrected);
        auto ccos = cos(angle - corrected);
        result.setX(local.getX() * ccos - local.getY() * csin + self.getX());
        result.setY(local.getX() * csin + local.getY() * ccos + self.getY());
    }
    return result;
}
