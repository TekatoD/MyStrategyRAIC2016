/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "NonCopyable.h"
#include "Situation.h"
#include "Behavior.h"

class Relationship: public Named, NonCopyable {
public:
    Relationship(const std::string& name, Ptr<Situation> situation, Ptr<Behavior> behavior);

    double getFactor() const;

    Ptr<Situation> getSituation() const;

    Ptr<Behavior> getBehavior() const;

private:
    Ptr<Situation> mSituation;
    Ptr<Behavior> mBehavior;
};


