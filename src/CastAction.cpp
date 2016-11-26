/**
 * @author arssivka
 * @date 11/26/16
 */

#include "CastAction.h"


CastAction::CastAction() : mAction(model::ActionType::ACTION_NONE) { }


void CastAction::kickStaff() noexcept {
    mAction = model::ACTION_STAFF;
}


void CastAction::castMagicMissile(const Point& targetPoint, double maxCastDistance, double minCastDistance) noexcept {
    this->castProjectileRequest(model::ACTION_MAGIC_MISSILE, targetPoint, maxCastDistance, minCastDistance);
}


void CastAction::castFrostBolt(const Point& targetPoint, double maxCastDistance, double minCastDistance) noexcept {
    this->castProjectileRequest(model::ACTION_FROST_BOLT, targetPoint, maxCastDistance, minCastDistance);
}


void CastAction::castFireball(const Point& targetPoint, double maxCastDistance, double minCastDistance) noexcept {
    this->castProjectileRequest(model::ACTION_FIREBALL, targetPoint, maxCastDistance, minCastDistance);
}


void CastAction::castHaste(long long int targetId) noexcept {
    this->castSpellRequest(model::ACTION_HASTE, targetId);
}


void CastAction::castShield(long long int targetId) noexcept {
    this->castSpellRequest(model::ACTION_SHIELD, targetId);
}


void CastAction::perform() {
    switch (mAction) {
        case model::ACTION_STAFF:
            this->tryKickStaff(mState);
            break;
        case model::ACTION_MAGIC_MISSILE:
        case model::ACTION_FROST_BOLT:
        case model::ACTION_FIREBALL:
            this->castProjectile(std::move(mState));
            break;
        case model::ACTION_HASTE:
        case model::ACTION_SHIELD:
            this->castSpell(std::move(mState));
            break;
        default:
            break;
    }
}


void CastAction::castProjectileRequest(model::ActionType action, const Point& targetPoint, double maxCastDistance,
                                       double minCastDistance) noexcept {
    mAction = action;
    mTargetPoint = targetPoint;
    mMinCastDistance = std::max(minCastDistance, 0.0);
    mMaxCastDistance = maxCastDistance;
}


void CastAction::castSpellRequest(model::ActionType action, long long int targetId) noexcept {
    mAction = action;
    mTargetId = targetId;
}


void CastAction::tryKickStaff(State::Ptr state) noexcept {
    auto& self = state->self;
    auto& move = state->move;
    int remainingTicks = self.getRemainingActionCooldownTicks();
    int actionRemainingTicks = self.getRemainingCooldownTicksByAction()[mAction];
    if (remainingTicks == 0 && actionRemainingTicks == 0) {
        move.setAction(model::ACTION_STAFF);
        mAction = model::ACTION_NONE;
    }
}


void CastAction::castProjectile(State::Ptr state) noexcept {
    auto& self = state->self;
    auto& move = state->move;
    auto& game = state->game;
    switch (mAction) {
        case model::ACTION_MAGIC_MISSILE:
            if (self.getMana() < game.getMagicMissileManacost()) return;
            break;
        case model::ACTION_FROST_BOLT:
            if (!this->checkSkill(model::SKILL_FROST_BOLT, state) ||
                self.getMana() < game.getFrostBoltManacost()) return;
            break;
        case model::ACTION_FIREBALL:
            if (!this->checkSkill(model::SKILL_FIREBALL, state) ||
                self.getMana() < game.getFireballManacost()) return;
            break;
        default:
            break;
    }
    int remainingTicks = self.getRemainingActionCooldownTicks();
    int actionRemainingTicks = self.getRemainingCooldownTicksByAction()[mAction];
    if (remainingTicks == 0 && actionRemainingTicks == 0) {
        mMaxCastDistance = std::min(mMaxCastDistance, game.getWizardCastRange());
        double castAngle = self.getAngleTo(mTargetPoint.getX(),
                                           mTargetPoint.getX());
        move.setAction(mAction);
        move.setCastAngle(castAngle);
        move.setMinCastDistance(mMinCastDistance);
        move.setMaxCastDistance(mMaxCastDistance);
        mAction = model::ACTION_NONE;
    }
}


void CastAction::castSpell(State::Ptr state) noexcept {
    auto& self = state->self;
    auto& move = state->move;
    auto& game = state->game;
    switch (mAction) {
        case model::ACTION_HASTE:
            if (!this->checkSkill(model::SKILL_HASTE, state) ||
                self.getMana() < game.getHasteManacost()) return;
            break;
        case model::ACTION_SHIELD:
            if (!this->checkSkill(model::SKILL_SHIELD, state) ||
                self.getMana() < game.getHasteManacost()) return;
            break;
        default:
            break;
    }

    move.setAction(mAction);
    move.setStatusTargetId(mTargetId);
    mAction = model::ACTION_NONE;
}


bool CastAction::checkSkill(model::SkillType skill, State::Ptr state) noexcept {
    auto& self = state->self;
    auto& skills = self.getSkills();
    bool found = std::find(skills.cbegin(), skills.cend(), skill) == skills.cend();
    if (!found) {
        mAction = model::ACTION_NONE;
        return false;
    }
    return true;
}
