/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#include "Weaponry.h"

void Weaponry::update(Ptr<State> state) {
    auto& self = state->self;
    auto& game = state->game;
    mStuffAvailable = this->checkTimeout(model::ACTION_STAFF, state);
    mMagicMissileAvailable = self.getMana() >= game.getMagicMissileManacost()
                             && this->checkTimeout(model::ACTION_MAGIC_MISSILE, state);
    mFrostBoltAvailable = self.getMana() >= game.getFrostBoltManacost()
                          && this->checkTimeout(model::ACTION_FROST_BOLT, state)
                          && this->checkSkill(model::SKILL_FROST_BOLT, state);
    mFireballAvailable = self.getMana() >= game.getFireballManacost()
                         && this->checkTimeout(model::ACTION_FIREBALL, state)
                         && this->checkSkill(model::SKILL_FIREBALL, state);
    mHasteAvailable = self.getMana() >= game.getHasteManacost()
                      && this->checkTimeout(model::ACTION_HASTE, state)
                      && this->checkSkill(model::SKILL_HASTE, state);
    mShieldAvailable = self.getMana() >= game.getShieldManacost()
                       && this->checkTimeout(model::ACTION_SHIELD, state)
                       && this->checkSkill(model::SKILL_SHIELD, state);
}

bool Weaponry::isStuffAvailable() const {
    return mStuffAvailable;
}

bool Weaponry::isMagicMissileAvailable() const {
    return mMagicMissileAvailable;
}

bool Weaponry::isFrostBoltAvailable() const {
    return mFrostBoltAvailable;
}

bool Weaponry::isFireballAvailable() const {
    return mFireballAvailable;
}

bool Weaponry::isHasteAvailable() const {
    return mHasteAvailable;
}

bool Weaponry::isShieldAvailable() const {
    return mShieldAvailable;
}

bool Weaponry::checkSkill(model::SkillType skill, Ptr<State> state) const {
    auto& self = state->self;
    auto& skills = self.getSkills();
    return std::find(skills.cbegin(), skills.cend(), skill) != skills.cend();
}

bool Weaponry::checkTimeout(model::ActionType actionType, Ptr<State> state) {
    auto& self = state->self;
    auto& actionsCooldown = self.getRemainingCooldownTicksByAction();
    auto cooldown = self.getRemainingActionCooldownTicks();
    return cooldown == 0 && actionsCooldown[actionType] == 0;
}
