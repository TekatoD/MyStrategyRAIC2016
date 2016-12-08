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
#include "SelfDisabler.h"

class Behavior : public SelfDisabler, public Named, public virtual Ranked, public virtual Refreshable, NonCopyable {
public:
    void turn(Ptr<State> state);

    virtual void prepare(Ptr<State> state) = 0;

    virtual void finalize();

protected:
    Behavior(const std::string& name) : Named(name) {}

    const WalkingAction& getWalkingAction() const;

    const CastAction& getCastAction() const;

    WalkingAction& getWalkingAction();

    CastAction& getCastAction();

private:
    WalkingAction mWalkingAction;
    CastAction mCastAction;

};


