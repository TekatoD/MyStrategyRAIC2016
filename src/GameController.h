/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <set>
#include <list>
#include "NonCopyable.h"
#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"
#include "Mechanism.h"

template <class TeacherType>
class GameController: public Refreshable, NonCopyable {
public:
    GameController(Ptr<TeacherType> teacher = nullptr)
            : mTeacher((teacher == nullptr ) ? share<TeacherType>() : teacher) {
        this->addMechanism(mTeacher);
    }


    void addMechanism(Ptr<Mechanism> mechanism) {
        mMechanismsList.push_back(std::move(mechanism));
    }

    void removeMechanism(Ptr<Mechanism> mechanism) {
        mMechanismsList.remove(mechanism);
    }

    void addRelationship(Ptr<Relationship> relationship) {
        mBehaviorsSet.insert(relationship->getBehavior());
        mSituationsSet.insert(relationship->getSituation());
        mRelationshipsSet.insert(std::move(relationship));
    }

    void removeRelationship(Ptr<Relationship> relationship) {
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

    void update(Ptr<State> state) override {
        for (auto&& mechanism : mMechanismsList)
            mechanism->update(state);

        for (auto&& behavior : mBehaviorsSet) {
            behavior->update(state);
            if (behavior->isDisabled()) {
                this->removeBehavior(behavior);
            } else {
                mTeacher->feedBehavior(behavior);
            }
        }
        for (auto&& situation : mSituationsSet) {
            situation->update(state);
            if (situation->isDisabled()) {
                this->removeSituation(situation);
            } else {
                mTeacher->feedBehavior(situation);
            }
        }
    }

    void turn() {
        Ptr<Behavior> behavior = nullptr;
        double maxFactor = 0.0;
        for (auto&& relationship : mRelationshipsSet) {
            mTeacher->feedRelationship(relationship);
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


private:
    void removeBehavior(Ptr<Behavior> behavior) {
        auto toRemove = std::find_if(mRelationshipsSet.begin(), mRelationshipsSet.end(),
                                       [behavior](const Ptr<Relationship>& relationship) {
                                           return behavior == relationship->getBehavior();
                                       });
        for (auto& it = toRemove; it != mRelationshipsSet.end(); ++it) {
            this->removeRelationship(*it);
        }
    }

    void removeSituation(Ptr<Situation> situation) {
        auto toRemove = std::find_if(mRelationshipsSet.begin(), mRelationshipsSet.end(),
                                     [situation](const Ptr<Relationship>& relationship) {
                                         return situation == relationship->getSituation();
                                     });
        for (auto& it = toRemove; it != mRelationshipsSet.end(); ++it) {
            this->removeRelationship(*it);
        }
    }


private:
    Ptr<TeacherType> mTeacher;
    std::list<Ptr<Mechanism>> mMechanismsList;
    std::set<Ptr<Behavior>> mBehaviorsSet;
    std::set<Ptr<Situation>> mSituationsSet;
    std::set<Ptr<Relationship>> mRelationshipsSet;
};


