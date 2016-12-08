/**
 * @author arssivka
 * @date 11/25/16
 */

#include "Behavior.h"


void Behavior::turn(Ptr<State> state) {
    mWalkingAction.update(state);
    mCastAction.update(state);
    this->prepare(std::move(state));
    mWalkingAction.perform();
    mCastAction.perform();
}


const WalkingAction& Behavior::getWalkingAction() const {
    return mWalkingAction;
}


const CastAction& Behavior::getCastAction() const {
    return mCastAction;
}

WalkingAction& Behavior::getWalkingAction() {
    return mWalkingAction;
}


CastAction& Behavior::getCastAction() {
    return mCastAction;
}

void Behavior::finalize() {}
