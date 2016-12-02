/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "Ranked.h"
#include "Refreshable.h"
#include "CastAction.h"
#include "WalkingAction.h"
#include "MessagingAction.h"
#include "LearningAction.h"
#include "Named.h"

class Behavior : public Named, public virtual Ranked, public virtual Refreshable, NonCopyable {
public:
    void turn();

protected:
    Behavior(const std::string& name) : Named(name) {}

protected:
    MessagingAction mMessagingAction;
    LearningAction mLearningAction;
    WalkingAction mWalkingAction;
    CastAction mCastAction;

};


