/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include <math.h>
#include "Behavior.h"
#include "BerserkTools.h"
#include "PathFinder.h"
#include "MagicSensors.h"

class BerserkBehavior : public Behavior {
public:
    BerserkBehavior(const std::string& name, const Point& position, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors,
                    Ptr<BerserkTools> berserkTools, double acceptedRadius, double factor = 1.0, bool berserkMode = false,
                    bool fullProbability = false)
            : Behavior(name), mUpdateNeeded(true), mPosition(position), mAcceptedRadius(acceptedRadius), mBerserkMode(berserkMode),
              mFullProbability(fullProbability), mFinder(std::move(finder)), mSensors(std::move(sensors)),
              mBerserkTools(std::move(berserkTools)) {
        this->setFactor(factor);
    }

    void turn(Ptr<State> state) override {
        CastAction& castAction = this->getCastAction();
        WalkingAction& walkAction = this->getWalkingAction();
        auto& self = state->self;
        Point pos = {self};
        if (!mWaypoint.inCircle(pos, mAcceptedRadius)) {
            mUpdateNeeded = true;
        }
        walkAction.setTargetPoint({self});
        if (!mBerserkMode) {
            if (!mPosition.inCircle({self}, mAcceptedRadius)) {
                if (mUpdateNeeded) {
                    mPath = mFinder->findPath({self}, {mPosition});
                    mUpdateNeeded = false;
                }

                while (pos.inCircle(mPath.current(), mAcceptedRadius)) mWaypoint = mPath.pop();
                Log(DEBUG) << mPath;
                if (mBerserkTools->isInStuffRange() && mBerserkTools->isStuffAvailable()) {
                    walkAction.setTargetPoint(pos);
                } else if (!mPath.isFinished()) {
                    auto target =  this->correctTarget(state, mPath.current());
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
        if (mBerserkTools->isInStuffRange() && mBerserkTools->isStuffAvailable()) {
            Log(DEBUG) << "SMASH!!!";
            castAction.kickStaff();
        }
        Behavior::turn(state);
    }

    void finalize() override {
        mUpdateNeeded = true;
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

private:
    void protectPosition(Ptr<State> state) {

    }

    Point correctTarget(Ptr<State> state, const Point& target) const {
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

private:
    bool mUpdateNeeded;
    Point mWaypoint;
    Path mPath;
    Point mPosition;
    double mAcceptedRadius;
    bool mBerserkMode;
    bool mFullProbability;
    Ptr<PathFinder> mFinder;
    Ptr<MagicSensors> mSensors;
    Ptr<BerserkTools> mBerserkTools;
};


