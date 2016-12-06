/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include "Behavior.h"
#include "Walker.h"
#include "Weaponry.h"

template <size_t W>
class BerserkBehavior : public Behavior {
public:
    BerserkBehavior(const std::string& name, const Point& position, Ptr<Walker<W>> walker,
                    Ptr<Weaponry> weaponry, double sectorSize, double factor = 1.0, bool berserkMode = false,
                    bool holdPositionRequired = false, bool fullProbability = false)
            : Behavior(name), mPosition(position), mSectorSize(sectorSize), mBerserkMode(berserkMode),
              mHoldPositionRequired(holdPositionRequired), mWalker(std::move(walker)), mWeaponry(std::move(weaponry)),
              mFullProbability(fullProbability) {
        this->setFactor(factor);
    }

    void turn(Ptr<State> state) override {
        CastAction& castAction = this->getCastAction();
        WalkingAction& walkAction = this->getWalkingAction();
        auto& self = state->self;

        walkAction.setTargetPoint({self});
        if (!mBerserkMode) {
            if (mHoldPositionRequired || !mPosition.inCircle({self}, mSectorSize)) {
                Path path = mWalker->findPath({self}, {mPosition});
                if (!path.isFinished()) {
                    auto target = path.pop();
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
    double mSectorSize;
    bool mBerserkMode;
    bool mHoldPositionRequired;
    Ptr<Walker<W>> mWalker;
    Ptr<Weaponry> mWeaponry;
    bool mFullProbability;
};


