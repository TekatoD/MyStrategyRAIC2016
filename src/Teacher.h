/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#pragma once


#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"
#include "Mechanism.h"

class Teacher : public Mechanism {
public:
    void feedBehavior(Ptr<Behavior>);

    void feedSituation(Ptr<Situation>);

    void feedRelationship(Ptr<Relationship>);

    void update(Ptr<State> state) override;

private:


};


