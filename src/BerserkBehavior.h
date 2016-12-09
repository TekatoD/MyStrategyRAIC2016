/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include <math.h>
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
        std::vector<const model::LivingUnit*> enemies;
        const auto& minions = mFilter->getMinions();
        const auto& wizards = mFilter->getWizards();
        enemies.reserve(minions.size());
        bool rampage = false;
        for (auto&& m : minions)
            if (m->getFaction() != self.getFaction())
                enemies.push_back(m);
        for (auto&& w : minions)
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
                for (auto&& e : enemies) {
                    if (self.getCastRange() + mBerserkTools->getCastRangeIncrement() <= self.getDistanceTo(*e)) {
                        if (target == nullptr || e->getLife() < target->getLife()) {
                            target = e;
                        }
                    }
                }
                if (target != nullptr) {
                    this->setTrackingPoint({*target});
                    if (std::abs(self.getAngleTo(*target)) <= 1e-7 && mBerserkTools->isMagicMissileAvailable()) {
                        castAction.castMagicMissile({*target},
                                                    self.getCastRange() + mBerserkTools->getCastRangeIncrement(),
                                                    self.getDistanceTo(*target) - target->getRadius());
                    }
//                    rampage = true; //TODO: PIKABU
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
        const double radius = state->self.getRadius();
        auto gm = this->getDamageMap<600>(state);
        //TODO: PIAKBE
        if(mTemp){
            Log(DEBUG) << gm.getGrid();
        }
        //TODOD: PIKNAJU
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

    template <size_t S>
    GridMap<int, S, S> getDamageMap(Ptr<State> state) {
        const auto &self = state->self;
        const auto &projectiles = mFilter->getProjectiles();
        const auto &minions = mFilter->getMinions();
        const auto &wizards = mFilter->getWizards();
        const auto &buildings = mFilter->getBuildings();
        const auto &game = state->game;
        const model::Faction &faction = self.getFaction();
        mTemp = false;
        GridMap<int, S, S> gridMapMinion{Point{self}, self.getRadius() / 4, Point{-(double)S / 2, -(double)S / 2}};
        GridMap<int, S, S> gridMapWizards{Point{self}, self.getRadius() / 4, Point{-(double)S / 2, -(double)S / 2}};
        GridMap<int, S, S> gridMapBuildings{Point{self}, self.getRadius() / 4, Point{-(double)S / 2, -(double)S / 2}};
        GridMap<int, S, S> gridMapProjectiles{Point{self}, self.getRadius() / 4, Point{-(double)S / 2, -(double)S / 2}};
        GridMap<int, S, S> resultGridMap{Point{self}, self.getRadius() / 4, Point{-(double)S / 2, -(double)S / 2}};
        for (auto&& minion : minions) {
            if (minion->getFaction() != faction && minion->getFaction() != model::Faction::FACTION_NEUTRAL
                && minion->getFaction() != model::Faction::FACTION_OTHER) {
                if (minion->getType() == model::MinionType::MINION_ORC_WOODCUTTER) {
                    gridMapMinion.drawFilledCircle(Point{*minion}, game.getOrcWoodcutterAttackRange(), 5);
                } else {
                    gridMapMinion.drawFilledCircle(Point{*minion}, game.getFetishBlowdartAttackRange(), 4);
                }
            }
        }
        for (auto&& wizard : wizards) {
            if (wizard->getFaction() != faction) {
                gridMapWizards.drawFilledCircle(Point{*wizard}, wizard->getCastRange(), 3);
            }
        }
        for (auto&& building : buildings) {
            if (building->getFaction() != faction && building->getFaction() != model::Faction::FACTION_NEUTRAL &&
                building->getFaction() != model::Faction::FACTION_OTHER) {
                gridMapBuildings.drawFilledCircle(Point{*building}, building->getAttackRange(), 6);
            }
        }
        for (auto&& projectile : projectiles) {
            if(projectile->getFaction() != faction) {
                double dist = projectile->getDistanceTo(self) + self.getRadius() * 2;
                Point endPoint(projectile->getX() + (cos(projectile->getAngle())) * dist,
                               projectile->getY() + (sin(projectile->getAngle())) * dist);
                int projectileDamage = 0;
                switch(projectile->getType()) {
                    case model::ProjectileType::PROJECTILE_DART:
                        projectileDamage = game.getDartDirectDamage();
                        break;
                    case model::ProjectileType::PROJECTILE_MAGIC_MISSILE:
                        projectileDamage = game.getMagicMissileDirectDamage();
                        break;
                    case model::ProjectileType::PROJECTILE_FIREBALL:
                        projectileDamage = game.getFireballExplosionMaxDamage();
                        break;
                    case model::ProjectileType::PROJECTILE_FROST_BOLT:
                        projectileDamage = game.getFrostBoltDirectDamage();
                        break;
                    default:
                        projectileDamage = 1;
                }
                mTemp = true; //TODO: Pikabu
                gridMapProjectiles.drawLine(Point{*projectile}, endPoint, projectileDamage);

            }
        }
        resultGridMap += (gridMapMinion + gridMapWizards + gridMapBuildings + gridMapProjectiles);
        return std::move(resultGridMap);
    }

private:
    bool mTemp; //TODO: PIKABU
    Point mTargetPoint;
    bool mBerserkMode;
    Ptr<BerserkTools> mBerserkTools;
    Ptr<WorldFilter> mFilter;
};


