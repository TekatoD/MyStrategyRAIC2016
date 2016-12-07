/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include "Behavior.h"
#include "Weaponry.h"
#include "PathFinder.h"
#include "MagicSensors.h"

class BerserkBehavior : public Behavior {
public:
    BerserkBehavior(const std::string& name, const Point& position, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors,
                    Ptr<Weaponry> weaponry, double acceptedRadius, double factor = 1.0, bool berserkMode = false,
                    bool holdPositionRequired = false, bool fullProbability = false)
            : Behavior(name), mPosition(position), mAcceptedRadius(acceptedRadius), mBerserkMode(berserkMode),
              mHoldPositionRequired(holdPositionRequired), mFullProbability(fullProbability),
              mFinder(std::move(finder)), mSensors(std::move(sensors)), mWeaponry(std::move(weaponry)) {
        this->setFactor(factor);
    }

    void turn(Ptr<State> state) override {
        CastAction& castAction = this->getCastAction();
        WalkingAction& walkAction = this->getWalkingAction();
        auto& self = state->self;

        walkAction.setTargetPoint({self});
        if (!mBerserkMode) {
            if (mHoldPositionRequired || !mPosition.inCircle({self}, mAcceptedRadius)) {
                Path path = mFinder->findPath({self}, {mPosition});
                Log(DEBUG) << path;
                Log(DEBUG) << *mSensors;
                if (!path.isFinished()) {
                    auto target = path.pop();
                    auto angle = self.getAngle() + self.getAngleTo(target.getX(), target.getY());
                    auto corrected = mSensors->correctDirection(angle, M_PI_2);
                    if (angle != corrected) {
                        Point local = target - Point{self};
                        auto csin = sin(angle - corrected);
                        auto ccos = cos(angle - corrected);
                        target.setX(local.getX() * ccos - local.getY() * csin + self.getX());
                        target.setY(local.getX() * csin + local.getY() * ccos + self.getY());
                    }
                    walkAction.setSpeedFactor(1.0);
                    walkAction.setTargetPoint(target);
                    walkAction.setTrackingPoint(target);
                }
            } else {
                this->protectPosition(state);
            }
        } else {
            // TODO Check nearest enemies
        }

        Behavior::turn(state);
    }

    void update(Ptr<State> state) override {
        auto& self = state->self;
        if (mFullProbability) {
            this->setProbability(1.0);
        } else {
            this->setProbability(1.0 / mPosition.getDistanceTo(self));
        }
    }

    bool isFullProbability() const {
        return mFullProbability;
    }

    void setFullProbability(bool fullProbability) {
        mFullProbability = fullProbability;
    }

    const Point& getPosition() const {
        return mPosition;
    }

    void setPosition(const Point& position) {
        mPosition = position;
    }

    bool isBerserkMode() const {
        return mBerserkMode;
    }

    void setBerserkMode(bool berserkMode) {
        mBerserkMode = berserkMode;
    }

    bool isHoldPositionRequired() const {
        return mHoldPositionRequired;
    }

    void setHoldPositionRequired(bool holdPositionRequired) {
        mHoldPositionRequired = holdPositionRequired;
    }


private:
    void protectPosition(Ptr<State> state) {

    }

private:
    Point mPosition;
    double mAcceptedRadius;
    bool mBerserkMode;
    bool mHoldPositionRequired;
    bool mFullProbability;
    Ptr<PathFinder> mFinder;
    Ptr<MagicSensors> mSensors;
    Ptr<Weaponry> mWeaponry;
};


