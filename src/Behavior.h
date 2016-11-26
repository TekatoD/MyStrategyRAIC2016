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

class Behavior : public Pointer<Behavior>, public Ranked, public Refreshable, NonCopyable {
public:
    void turn();

protected:
    Behavior() = default;

protected:
    MessagingAction mMessagingAction;
    LearningAction mLearningAction;
    WalkingAction mWalkingAction;
    CastAction mCastAction;

};


