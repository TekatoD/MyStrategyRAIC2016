/**
 * @author arssivka
 * @date 12/10/16
 */

#include "Controller.h"

Controller::Controller(Ptr <Teacher> teacher)
        : mTeacher(teacher),
          mState(nullptr), mActiveBehavior(nullptr) {
    if (mTeacher != nullptr)
        this->addMechanism(mTeacher);
}


void Controller::addMechanism(Ptr <Mechanism> mechanism) {
    mMechanismsList.push_back(std::move(mechanism));
}


void Controller::removeMechanism(Ptr <Mechanism> mechanism) {
    mMechanismsList.remove(mechanism);
}


void Controller::addRelationship(Ptr<Relationship> relationship) {
    Log(DEBUG) << "Add relationship between"
               << relationship->getBehavior()->getName()
               << "and"
               << relationship->getSituation()->getName();
    mBehaviorsSet.insert(relationship->getBehavior());
    mSituationsSet.insert(relationship->getSituation());
    mRelationshipsSet.insert(std::move(relationship));
}


void Controller::removeRelationship(Ptr<Relationship> relationship) {
    auto found = mRelationshipsSet.find(relationship);
    if (found != mRelationshipsSet.end()) {
        long count;
        // Try to delete behavior
        auto behavior = (*found)->getBehavior();
        count = std::count_if(mRelationshipsSet.cbegin(), mRelationshipsSet.cend(),
                              [behavior](const Ptr<Relationship>& r) {
                                  return r->getBehavior() == behavior;
                              });
        if (count == 1) mBehaviorsSet.erase(behavior);
        // Try to delete situation
        auto situation = (*found)->getSituation();
        count = std::count_if(mRelationshipsSet.cbegin(), mRelationshipsSet.cend(),
                              [situation](const Ptr<Relationship>& r) {
                                  return r->getSituation() == situation;
                              });
        if (count == 1) mSituationsSet.erase(situation);
        // Delete relationship
        mRelationshipsSet.erase(found);
    }
}


void Controller::update(Ptr<State> state) {
    Log(DEBUG) << "Updating controller...";
    mState = state;
    for (auto&& mechanism : mMechanismsList)
        mechanism->update(state);

    for (auto&& situation : mSituationsSet) {
        if (!situation->isDisabled()) {
            situation->update(state);
            Log(DEBUG) << " -Situation" << situation->getName()
                       << "has factor" << situation->getFactor()
                       << "and probability" << situation->getProbability();
            if (mTeacher != nullptr) mTeacher->feedSituation(situation);
        }
    }

    for (auto&& behavior : mBehaviorsSet) {
        if (!behavior->isDisabled()) {
            behavior->update(state);
            Log(DEBUG) << " -Behavior" << behavior->getName()
                       << "has factor" << behavior->getFactor()
                       << "and probability" << behavior->getProbability();
            if (mTeacher != nullptr) mTeacher->feedBehavior(behavior);
        }
    }
}


void Controller::turn() {
    Ptr<Behavior> behavior = nullptr;
    double maxFactor = 0.0;
    for (auto&& relationship : mRelationshipsSet) {
        if (mTeacher != nullptr) mTeacher->feedRelationship(relationship);
        auto factor = relationship->getFactor();
        if (factor > maxFactor) {
            maxFactor = factor;
            behavior = relationship->getBehavior();
        }
    }

    if (behavior != nullptr) {
        Log(DEBUG) << "Performing" << behavior->getName() << "behavior";
        if (mActiveBehavior != nullptr && mActiveBehavior != behavior) {
            mActiveBehavior->finalize();
            mActiveBehavior = behavior;
        }
        behavior->turn(mState);
    } else {
        Log(WARN) << "Behavior wasn't selected!";
    }
}


Ptr<Teacher> Controller::getTeacher() const {
    return mTeacher;
}


void Controller::setTeacher(Ptr<Teacher> teacher) {
    if (mTeacher != nullptr)
        this->removeMechanism(mTeacher);
    mTeacher = std::move(teacher);
    this->addMechanism(mTeacher);
}


void Controller::removeBehavior(Ptr<Behavior> behavior) {
    auto toRemove = std::find_if(mRelationshipsSet.begin(), mRelationshipsSet.end(),
                                 [behavior](const Ptr<Relationship>& relationship) {
                                     return behavior == relationship->getBehavior();
                                 });
    for (auto& it = toRemove; it != mRelationshipsSet.end(); ++it) {
        this->removeRelationship(*it);
    }
}


void Controller::removeSituation(Ptr<Situation> situation) {
    auto toRemove = std::find_if(mRelationshipsSet.begin(), mRelationshipsSet.end(),
                                 [situation](const Ptr<Relationship>& relationship) {
                                     return situation == relationship->getSituation();
                                 });
    for (auto& it = toRemove; it != mRelationshipsSet.end(); ++it) {
        this->removeRelationship(*it);
    }
}
