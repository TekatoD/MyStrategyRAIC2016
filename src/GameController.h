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

    void update(Ptr<State> state) override {
        for (auto&& mechanism : mMechanismsList)
            mechanism->update(state);
        for (auto&& behavior : mBehaviorsSet) {
            behavior->update(state);
            mTeacher->feedBehavior(behavior);
        }
        for (auto&& situation : mSituationsSet) {
            situation->update(state);
            mTeacher->feedSituation(situation);
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
    Ptr<TeacherType> mTeacher;
    std::list<Ptr<Mechanism>> mMechanismsList;
    std::set<Ptr<Behavior>> mBehaviorsSet;
    std::set<Ptr<Situation>> mSituationsSet;
    std::set<Ptr<Relationship>> mRelationshipsSet;
};


