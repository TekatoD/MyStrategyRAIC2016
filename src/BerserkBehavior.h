/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include <math.h>
#include <assert.h>
#include "Behavior.h"
#include "BerserkTools.h"
#include "PathFinder.h"
#include "MagicSensors.h"
#include "WalkingBehavior.h"

class BerserkBehavior : public WalkingBehavior {
public:
    BerserkBehavior(const std::string& name, const Point& position, double acceptedRadius, double factor,
                    bool berserkMode, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors,
                    Ptr<BerserkTools> berserkTools, Ptr<WorldFilter> filter)
            : WalkingBehavior(name, position, acceptedRadius, finder, sensors), mTargetPoint(position),
              mBerserkMode(berserkMode), mBerserkTools(std::move(berserkTools)), mFilter(std::move(filter)) {
        this->setFactor(factor);
    }

    void prepare(Ptr<State> state) override {
        CastAction& castAction = this->getCastAction();
        const auto& self = state->self;
        const auto& world = state->world;
        const auto& game = state->game;
        std::vector<const model::LivingUnit*> enemies;
        const auto& minions = mFilter->getMinions();
        const auto& wizards = mFilter->getWizards();
        enemies.reserve(minions.size());
        bool rampage = false;
         for (auto&& m : minions)
            if (m->getFaction() != self.getFaction() && m->getFaction() != model::FACTION_NEUTRAL)
                enemies.push_back(m);
        for (auto&& w : wizards)
            if (w->getFaction() != self.getFaction())
                enemies.push_back(w);

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

    void update(Ptr<State> state) override {
        this->updateProbabilityByDistance({state->self});
    }

private:
    bool isBerserkMode() const {
        return mBerserkMode;
    }

    void setBerserkMode(bool berserkMode) {
        mBerserkMode = berserkMode;
    }

    void protectPosition(Ptr<State> state) {

    }

private:
    Point mTargetPoint;
    bool mBerserkMode;
    Ptr<BerserkTools> mBerserkTools;
    Ptr<WorldFilter> mFilter;
};


