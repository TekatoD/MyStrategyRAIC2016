/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <set>
#include <forward_list>
#include "NonCopyable.h"
#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"
#include "Mechanism.h"

class GameController: public Refreshable, NonCopyable {
public:
    GameController();

    void addMechanism(Mechanism::Ptr mechanism);

    void removeMechanism(Mechanism::Ptr mechanism);

    void addRelationship(Relationship::Ptr relationship);

    void removeRelationship(Relationship::Ptr relationship);

    void update(State::Ptr state) override;

    void turn();


private:
    std::forward_list<Mechanism::Ptr> mMechanismsList;
    std::set<Behavior::Ptr> mBehaviorsSet;
    std::set<Situation::Ptr> mSituationsSet;
    std::set<Relationship::Ptr> mRelationshipsSet;
};


