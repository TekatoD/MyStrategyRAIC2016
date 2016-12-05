/**
 * @author arssivka
 * @date 12/4/16
 */

#include "CallbackBehavior.h"


CallbackBehavior::CallbackBehavior(const std::string& name,
                                   const std::function<void(WalkingAction&, CastAction&)>& mover,
                                   const std::function<double(Ptr<State>)>& probabilityGenerator)
        : Behavior(name), mMover(mover), mProbabilityGenerator(probabilityGenerator) {}


void CallbackBehavior::turn() {
    mMover(this->getWalkingAction(), this->getCastAction());
    Behavior::turn();
}

void CallbackBehavior::update(Ptr<State> state) {
    this->setProbability(mProbabilityGenerator(state));
}
