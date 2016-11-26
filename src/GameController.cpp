/**
 * @author arssivka
 * @date 11/25/16
 */

#include "GameController.h"


GameController::GameController() {}


void GameController::addMechanism(Mechanism::Ptr mechanism) {
    mMechanismsList.push_front(std::move(mechanism));
}


void GameController::removeMechanism(Mechanism::Ptr mechanism) {
    mMechanismsList.remove(mechanism);
}


void GameController::addRelationship(Relationship::Ptr relationship) {
    mBehaviorsSet.insert(relationship->getBehavior());
    mSituationsSet.insert(relationship->getSituation());
    mRelationshipsSet.insert(std::move(relationship));
}


void GameController::removeRelationship(Relationship::Ptr relationship) {
    auto found = mRelationshipsSet.find(relationship);
    if (found != mRelationshipsSet.end()) {
        int count;
        // Try to delete behavior
        count = 0;
        auto behavior = (*found)->getBehavior();
        for (auto&& r : mRelationshipsSet)
            count += (r->getBehavior() == behavior) ? 1 : 0;
        if (count == 1) mBehaviorsSet.erase(behavior);
        // Try to delete situation
        count = 0;
        auto situation = (*found)->getSituation();
        for (auto&& r : mRelationshipsSet)
            count += (r->getSituation() == situation) ? 1 : 0;
        if (count == 1) mSituationsSet.erase(situation);
        // Delete relationship
        mRelationshipsSet.erase(found);
    }
}


void GameController::update(State::Ptr state) {
    for (auto&& mechanism : mMechanismsList)
        mechanism->update(state);
    for (auto&& behavior : mBehaviorsSet)
        behavior->update(state);
    for (auto&& behavior : mSituationsSet)
        behavior->update(state);
}


void GameController::turn() {
    Behavior::Ptr behavior = nullptr;
    double maxFactor = 0.0;
    for (auto&& relationship : mRelationshipsSet) {
        auto factor = relationship->getFactor();
        if (factor > maxFactor) {
            maxFactor = factor;
            behavior = relationship->getBehavior();
        }
    }

    if (behavior != nullptr) {
        behavior->turn();
    }
}
