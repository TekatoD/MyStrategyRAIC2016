/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <set>
#include <list>
#include <unordered_set>
#include "NonCopyable.h"
#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"
#include "Mechanism.h"
#include "Log.h"
#include "Teacher.h"


class Controller: public Refreshable, NonCopyable {
public:
    Controller(Ptr<Teacher> teacher = nullptr);


    void addMechanism(Ptr<Mechanism> mechanism);

    void removeMechanism(Ptr<Mechanism> mechanism);

    void addRelationship(Ptr<Relationship> relationship);

    void removeRelationship(Ptr<Relationship> relationship);

    void update(Ptr<State> state) override;

    void turn();

    Ptr<Teacher> getTeacher() const;

    void setTeacher(Ptr<Teacher> teacher);


private:
    void removeBehavior(Ptr<Behavior> behavior);

    void removeSituation(Ptr<Situation> situation);


private:
    Ptr<Teacher> mTeacher;
    Ptr<State> mState;
    std::list<Ptr<Mechanism>> mMechanismsList;
    std::set<Ptr<Behavior>> mBehaviorsSet;
    std::set<Ptr<Situation>> mSituationsSet;
    std::set<Ptr<Relationship>> mRelationshipsSet;
    Ptr<Behavior> mActiveBehavior;
};


