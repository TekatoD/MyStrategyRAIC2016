/**
 * @author arssivka
 * @date 12/10/16
 */

#pragma once


#include "Mechanism.h"
#include "Behavior.h"
#include "Situation.h"
#include "Relationship.h"

class Teacher : public Mechanism {
public:
    virtual void feedBehavior(Ptr<Behavior> behavior) = 0;

    virtual void feedSituation(Ptr<Situation> situation) = 0;

    virtual void feedRelationship(Ptr<Relationship> relationship) = 0;
};


