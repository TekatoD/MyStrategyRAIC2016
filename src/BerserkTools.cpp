/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#include "BerserkTools.h"

void BerserkTools::update(Ptr<State> state) {
    if (mState != state) {
        mState = state;
        mUpdated = false;
        mFilter->update(state);
    }
}

void BerserkTools::updateTools() const {
    if (!mUpdated) {
        auto& self = mState->self;
        auto& game = mState->game;
        mStuffAvailable = checkTimeout(model::ACTION_STAFF, mState);
        mMagicMissileAvailable = self.getMana() >= game.getMagicMissileManacost()
                                 && checkTimeout(model::ACTION_MAGIC_MISSILE, mState);
        mFrostBoltAvailable = self.getMana() >= game.getFrostBoltManacost()
                              && checkTimeout(model::ACTION_FROST_BOLT, mState)
                              && checkSkill(model::SKILL_FROST_BOLT, mState);
        mFireballAvailable = self.getMana() >= game.getFireballManacost()
                             && checkTimeout(model::ACTION_FIREBALL, mState)
                             && checkSkill(model::SKILL_FIREBALL, mState);
        mHasteAvailable = self.getMana() >= game.getHasteManacost()
                          && checkTimeout(model::ACTION_HASTE, mState)
                          && checkSkill(model::SKILL_HASTE, mState);
        mShieldAvailable = self.getMana() >= game.getShieldManacost()
                           && checkTimeout(model::ACTION_SHIELD, mState)
                           && checkSkill(model::SKILL_SHIELD, mState);
        // Update mInStuffRange;
        const auto& minions = mFilter->getMinions();
        const auto& wizards = mFilter->getWizards();
        const auto& trees = mFilter->getTrees();
        mInStuffRange = false;
        if (!mInStuffRange) {
            mInStuffRange = std::find_if(minions.cbegin(), minions.cend(), [&self, &game](const model::Minion* m) {
                return m->getFaction() != self.getFaction()
                       && std::abs(self.getAngleTo(*m)) <= game.getStaffSector() / 2.0
                       && self.getDistanceTo(*m) <= game.getStaffRange() + m->getRadius();
            }) != minions.cend();
        }
        if (!mInStuffRange) {
            mInStuffRange = std::find_if(wizards.cbegin(), wizards.cend(), [&self, &game](const model::Wizard* w) {
                return w->getFaction() != self.getFaction()
                       && std::abs(self.getAngleTo(*w)) <= game.getStaffSector() / 2.0
                       && self.getDistanceTo(*w) <= game.getStaffRange() + w->getRadius();
            }) != wizards.cend();
        }
        if (!mInStuffRange) {
            mInStuffRange = std::find_if(trees.cbegin(), trees.cend(), [&self, &game](const model::Tree* t) {
                return std::abs(self.getAngleTo(*t)) <= game.getStaffSector() / 2.0
                       && self.getDistanceTo(*t) <= game.getStaffRange() + t->getRadius();
            }) != trees.cend();
        }

        mUpdated = true;
    }
}

bool BerserkTools::isStuffAvailable() const {
    this->updateTools();
    return mStuffAvailable;
}

bool BerserkTools::isMagicMissileAvailable() const {
    this->updateTools();
    return mMagicMissileAvailable;
}

bool BerserkTools::isFrostBoltAvailable() const {
    this->updateTools();
    return mFrostBoltAvailable;
}

bool BerserkTools::isFireballAvailable() const {
    this->updateTools();
    return mFireballAvailable;
}

bool BerserkTools::isHasteAvailable() const {
    return mHasteAvailable;
}

bool BerserkTools::isShieldAvailable() const {
    this->updateTools();
    return mShieldAvailable;
}

bool BerserkTools::checkSkill(model::SkillType skill, Ptr<State> state) const {
    auto& self = state->self;
    auto& skills = self.getSkills();
    return std::find(skills.cbegin(), skills.cend(), skill) != skills.cend();
}

bool BerserkTools::checkTimeout(model::ActionType actionType, Ptr<State> state) const {
    auto& self = state->self;
    auto& actionsCooldown = self.getRemainingCooldownTicksByAction();
    auto cooldown = self.getRemainingActionCooldownTicks();
    return cooldown == 0 && actionsCooldown[actionType] == 0;
}

bool BerserkTools::isInStuffRange() const {
    this->updateTools();
    return mInStuffRange;
}
