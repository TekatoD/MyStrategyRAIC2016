/**
 * @author arssivka
 * @date 12/4/16
 */

#include "CallbackBehavior.h"


CallbackBehavior::CallbackBehavior(const std::string& name,
                                   const std::function<void(WalkingAction&, CastAction&)>& mover,
                                   const std::function<double(Ptr<State>)>& probabilityGenerator)
        : Behavior(name), mMover(mover), mProbabilityGenerator(probabilityGenerator) {}


void CallbackBehavior::prepare(Ptr<State> state) {
    mMover(this->getWalkingAction(), this->getCastAction());
    Behavior::turn(Ptr<State>());
}

void CallbackBehavior::update(Ptr<State> state) {
    this->setProbability(mProbabilityGenerator(state));
}
