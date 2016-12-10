/**
 * @author arssivka
 * @date 12/10/16
 */

#pragma once


#include "Teacher.h"

class SimpleTeacher : public Teacher {
public:
    SimpleTeacher();

    void update(Ptr<State> state) override;

    void feedBehavior(Ptr<Behavior> behavior) override;

    void feedSituation(Ptr<Situation> situation) override;

    void feedRelationship(Ptr<Relationship> relationship) override;

private:
    int mLevel;
};


