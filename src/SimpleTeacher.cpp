/**
 * @author arssivka
 * @date 12/10/16
 */

#include "SimpleTeacher.h"


void SimpleTeacher::update(Ptr<State> state) {
    const auto& self = state->self;
    const auto& game = state->game;
    if (game.isSkillsEnabled()) {
        auto& move = state->move;
        while (self.getLevel() != mLevel) {
            mLevel += 1;
            model::SkillType skill = model::_SKILL_UNKNOWN_;
            switch (mLevel) {
                case (1):
                    skill = model::SKILL_MAGICAL_DAMAGE_BONUS_PASSIVE_1;
                    break;
                case (2):
                    skill = model::SKILL_STAFF_DAMAGE_BONUS_PASSIVE_1;
                    break;
                case (3):
                    skill = model::SKILL_STAFF_DAMAGE_BONUS_AURA_1;
                    break;
                case (4):
                    skill = model::SKILL_STAFF_DAMAGE_BONUS_PASSIVE_2;
                    break;
                case (5):
                    skill = model::SKILL_STAFF_DAMAGE_BONUS_AURA_2;
                    break;
                case (6):
                    skill = model::SKILL_FIREBALL;
                    break;
                case (7):
                    skill = model::SKILL_MAGICAL_DAMAGE_BONUS_AURA_1;
                    break;
                case(8):
                    skill = model::SKILL_MAGICAL_DAMAGE_BONUS_PASSIVE_2;
                    break;
                case(9):
                    skill = model::SKILL_MAGICAL_DAMAGE_BONUS_AURA_2;
                    break;
                case(10):
                    skill = model::SKILL_FROST_BOLT;
                    break;
                default:
                    break;
            };
            if (skill != model::_SKILL_UNKNOWN_)
                move.setSkillToLearn(skill);
        }
    }
}


void SimpleTeacher::feedBehavior(Ptr<Behavior> behavior) {

}


void SimpleTeacher::feedSituation(Ptr<Situation> situation) {

}


void SimpleTeacher::feedRelationship(Ptr<Relationship> relationship) {

}


SimpleTeacher::SimpleTeacher()
        : mLevel(0) {}
