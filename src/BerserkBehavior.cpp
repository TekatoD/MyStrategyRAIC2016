/**
 *  @autor arssivka
 *  @date 12/9/16
 */

#include "BerserkBehavior.h"


void BerserkBehavior::prepare(Ptr<State> state) {
    CastAction& castAction = this->getCastAction();
    const auto& self = state->self;
    const auto& world = state->world;
    const auto& game = state->game;
    std::vector<const model::LivingUnit*> enemies;
    const auto& minions = mFilter->getMinions();
    const auto& wizards = mFilter->getWizards();
    const auto& buildings = mFilter->getBuildings();
    enemies.reserve(minions.size());
    bool rampage = false;
    for (auto&& m : minions)
        if (m->getFaction() != self.getFaction() && m->getFaction() != model::FACTION_NEUTRAL)
            enemies.push_back(m);
    for (auto&& w : wizards)
        if (w->getFaction() != self.getFaction())
            enemies.push_back(w);
    for (auto&& b : buildings)
        if (b->getFaction() != self.getFaction())
            enemies.push_back(b);

    if (!enemies.empty()) {
        // Hit staff
        if (mBerserkTools->isInStuffRange() && mBerserkTools->isStuffAvailable()) {
            Log(DEBUG) << "SMASH!!!";
            castAction.kickStaff();
            rampage = true;
        } else {
            model::LivingUnit const* target = nullptr;
            int inRange = 0;
            for (auto&& e : enemies) {
                if (self.getDistanceTo(*e) < game.getWizardCastRange()) {
                    ++inRange;
                    if (target == nullptr || e->getLife() < target->getLife()
                        || e->getLife() == target->getLife()
                           && e->getDistanceTo(self) < target->getDistanceTo(self)) {
                        target = e;
                    }
                }
            }
            if (target != nullptr) {
                this->setTrackingPoint({*target});
                if (std::abs(self.getAngleTo(*target)) == 0 && mBerserkTools->isMagicMissileAvailable()) {
                    Log(DEBUG) << "Cast magic missile!";
                    castAction.castMagicMissile({*target},
                                                game.getWizardCastRange(),
                                                self.getDistanceTo(*target) - target->getRadius());
                }
                rampage = true;
            }
        }

        if (rampage) {
            Log(DEBUG) << "RAMPAGE!";
            this->setTargetPoint({self});
        }
    }
    if (!rampage && this->getTargetPoint() != mTargetPoint) {
        this->setTargetPoint(mTargetPoint);
        this->enableTrackingTargetPoint();
    }

    this->updateWalkingAction(state);
}

void BerserkBehavior::update(Ptr<State> state) {
    this->updateProbabilityByDistance({state->self});
}

bool BerserkBehavior::isBerserkMode() const {
    return mBerserkMode;
}

void BerserkBehavior::setBerserkMode(bool berserkMode) {
    mBerserkMode = berserkMode;
}

void BerserkBehavior::protectPosition(Ptr<State> state) {

}


