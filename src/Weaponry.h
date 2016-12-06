/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include "CastAction.h"
#include "Mechanism.h"

class Weaponry : public Mechanism {
public:
    void update(Ptr<State> state) override;

    bool isStuffAvailable() const;

    bool isMagicMissileAvailable() const;

    bool isFrostBoltAvailable() const;

    bool isFireballAvailable() const;

    bool isHasteAvailable() const;

    bool isShieldAvailable() const;

private:
    bool checkSkill(model::SkillType skill, Ptr<State> state) const;

    bool checkTimeout(model::ActionType actionType, Ptr<State> state);

private:
    bool mStuffAvailable;
    bool mMagicMissileAvailable;
    bool mFrostBoltAvailable;
    bool mFireballAvailable;
    bool mHasteAvailable;
    bool mShieldAvailable;
};


