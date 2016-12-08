/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include "CastAction.h"
#include "Mechanism.h"
#include "Clusterer.h"
#include "WorldFilter.h"

class BerserkTools : public Mechanism {
public:
    BerserkTools(Ptr<WorldFilter> filter)
            : mUpdated(false), mFilter(filter), mState(nullptr) {}

    void update(Ptr<State> state) override;

    bool isStuffAvailable() const;

    bool isMagicMissileAvailable() const;

    bool isFrostBoltAvailable() const;

    bool isFireballAvailable() const;

    bool isHasteAvailable() const;

    bool isShieldAvailable() const;

    bool isInStuffRange() const;

private:
    bool checkSkill(model::SkillType skill, Ptr<State> state) const;

    bool checkTimeout(model::ActionType actionType, Ptr<State> state) const;

    void updateTools() const;
private:
    mutable  bool mUpdated;
    Ptr<State> mState;
    mutable Ptr<WorldFilter> mFilter;
    mutable bool mStuffAvailable;
    mutable bool mMagicMissileAvailable;
    mutable bool mFrostBoltAvailable;
    mutable bool mFireballAvailable;
    mutable bool mHasteAvailable;
    mutable bool mShieldAvailable;
    mutable bool mInStuffRange;
};


