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
        mUpdated = true;
        const auto& self = mState->self;
        const auto& game = mState->game;
        mStuffAvailable = this->checkTimeout(model::ACTION_STAFF);
        mMagicMissileAvailable = self.getMana() >= game.getMagicMissileManacost()
                                 && ((this->checkTimeout(model::ACTION_MAGIC_MISSILE)
                                     || this->checkSkill(self, model::SKILL_ADVANCED_MAGIC_MISSILE)));
        mFrostBoltAvailable = self.getMana() >= game.getFrostBoltManacost()
                              && this->checkTimeout(model::ACTION_FROST_BOLT)
                              && this->checkSkill(self, model::SKILL_FROST_BOLT);
        mFireballAvailable = self.getMana() >= game.getFireballManacost()
                             && this->checkTimeout(model::ACTION_FIREBALL)
                             && this->checkSkill(self, model::SKILL_FIREBALL);
        mHasteAvailable = self.getMana() >= game.getHasteManacost()
                          && this->checkTimeout(model::ACTION_HASTE)
                          && this->checkSkill(self, model::SKILL_HASTE);
        mShieldAvailable = self.getMana() >= game.getShieldManacost()
                           && this->checkTimeout(model::ACTION_SHIELD)
                           && this->checkSkill(self, model::SKILL_SHIELD);
        // Update mInStuffRange;
        const auto& minions = mFilter->getMinions();
        const auto& wizards = mFilter->getWizards();
        const auto& trees = mFilter->getTrees();

        double mCastRangeIncrement = 0.0;
        const auto& nearestWizards = mFilter->getWizards();
        for (auto&& w : nearestWizards) {
            if (w->getFaction() == self.getFaction() || w->getId() != self.getId()) {
                double increment = 0.0;
                if (checkSkill(*w, model::SKILL_RANGE_BONUS_AURA_2)) {
                    mCastRangeIncrement = game.getRangeBonusPerSkillLevel() * 2.0;
                    break;
                } else if (checkSkill(*w, model::SKILL_RANGE_BONUS_PASSIVE_2)) {
                    mCastRangeIncrement = game.getRangeBonusPerSkillLevel();
                }
            }
        }

        mInStuffRange = false;
        // TODO: Check accumulate auras and passive
        if (checkSkill(self, model::SKILL_RANGE_BONUS_PASSIVE_2)) {
            mCastRangeIncrement += game.getRangeBonusPerSkillLevel() * 2.0;
        } else if (checkSkill(self, model::SKILL_RANGE_BONUS_PASSIVE_1)) {
            mCastRangeIncrement += game.getRangeBonusPerSkillLevel();
        }

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

        mInCastRange = false;
        if (!mInCastRange) {
            mInCastRange = std::find_if(minions.cbegin(), minions.cend(), [this, &self, &game](const model::Minion* m) {
                return m->getFaction() != self.getFaction()
                       && self.getDistanceTo(*m) <= game.getWizardCastRange() + this->getCastRangeIncrement();
            }) != minions.cend();
        }

        if (!mInCastRange) {
            mInCastRange = std::find_if(wizards.cbegin(), wizards.cend(), [this, &self, &game](const model::Wizard* w) {
                return w->getFaction() != self.getFaction()
                       && self.getDistanceTo(*w) <= game.getWizardCastRange() + this->getCastRangeIncrement();
            }) != wizards.cend();
        }
    }
}

double BerserkTools::getCastRangeIncrement() const {
    this->updateTools();
    return mCastRangeIncrement;
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

bool BerserkTools::checkSkill(const model::Wizard& wizard, model::SkillType skill) const {
    auto& skills = wizard.getSkills();
    return std::find(skills.cbegin(), skills.cend(), skill) != skills.cend();
}

bool BerserkTools::checkTimeout(model::ActionType actionType) const {
    auto& self = mState->self;
    auto& actionsCooldown = self.getRemainingCooldownTicksByAction();
    auto cooldown = self.getRemainingActionCooldownTicks();
    return cooldown == 0 && actionsCooldown[actionType] == 0;
}

bool BerserkTools::isInStuffRange() const {
    this->updateTools();
    return mInStuffRange;
}

bool BerserkTools::isInCastRange() const {
    this->updateTools();
    return mInCastRange;
}
