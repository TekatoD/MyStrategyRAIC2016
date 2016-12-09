/**
 * @author arssivka
 * @date 12/7/16
 */

#include "TakeBonusBehavior.h"
#include <cmath>

void TakeBonusBehavior::prepare(Ptr<State> state) {
    this->updateWalkingAction(state);
}

void TakeBonusBehavior::update(Ptr<State> state) {
    this->setProbability(1.0);
}
