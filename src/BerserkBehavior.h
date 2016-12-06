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
                    Ptr<Weaponry> weaponry, double sectorSize, bool berserkMode = false,
                    bool holdPositionRequired = false, double factor = 1.0)
            : Behavior(name), mPosition(position), mSectorSize(sectorSize), mBerserkMode(berserkMode),
              mHoldPositionRequired(holdPositionRequired), mWalker(std::move(walker)), mWeaponry(std::move(weaponry)) {
        this->setFactor(factor);
    }

    void update(Ptr<State> state) override {
        auto& castAction = this->getCastAction();
        auto& walkAction = this->getWalkingAction();
        auto& self = state->self;
        if (!mBerserkMode) {
            if (mHoldPositionRequired || !mPosition.inCircle({self}, mSectorSize)) {
                mWalker->findPath({self}, {mPosition});
            } else {
                this->protectPosition(state);
            }
        } else {
            // TODO Check nearest enemies
        }
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

};


