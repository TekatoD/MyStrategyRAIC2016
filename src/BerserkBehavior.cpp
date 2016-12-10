/**
 *  @autor arssivka
 *  @date 12/9/16
 */

#include <assert.h>
#include "BerserkBehavior.h"


static constexpr size_t MAP_SIZE = BerserkBehavior::MAP_SIZE;
static constexpr int WINDOW_SIZE = 4;


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
    for (auto&& m : minions) {
        bool wokeUp = m->getFaction() == model::FACTION_NEUTRAL && m->getSpeedX() != 0 && m->getSpeedY() != 0;
        if ((m->getFaction() != self.getFaction() && m->getFaction() != model::FACTION_NEUTRAL) || wokeUp)
            enemies.push_back(m);
    }
    for (auto&& w : wizards)
        if (w->getFaction() != self.getFaction())
            enemies.push_back(w);
    if (enemies.empty()) {
        for (auto&& b : buildings)
            if (b->getFaction() != self.getFaction())
                enemies.push_back(b);
    }


    if (!enemies.empty()) {
        model::LivingUnit const* target = nullptr;
        // Hit staff
        if (mBerserkTools->isInStuffRange() && mBerserkTools->isStuffAvailable()) {
            Log(DEBUG) << "SMASH!!!";
            castAction.kickStaff();
            rampage = true;
        } else {
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
                if (std::abs(self.getAngleTo(*target)) <= 2 * PI / 180) {
                    if (mBerserkTools->isFireballAvailable()) {
                        Log(DEBUG) << "BURN!!!";
                        castAction.castFireball({*target}, game.getWizardCastRange(), self.getDistanceTo(*target));
                    } else if (mBerserkTools->isFrostBoltAvailable() && dynamic_cast<const model::Wizard*>(target) != nullptr) {
                        Log(DEBUG) << "Happy new ear!";
                        castAction.castFrostBolt({*target}, game.getWizardCastRange(), self.getDistanceTo(*target));
                    } else if (mBerserkTools->isMagicMissileAvailable()) {
                        Log(DEBUG) << "Cast magic missile!";
                        castAction.castMagicMissile({*target}, game.getWizardCastRange(), self.getDistanceTo(*target));
                    }
                }
                rampage = true;
            }
        }

        if (rampage) {
            Log(DEBUG) << "RAMPAGE!";
            const auto damageMap = this->getDamageMap(state, true, true, false, true);
            const auto attackMap = this->getAttackMap(state);
            int damage = std::numeric_limits<int>::max();
            const Point center = Point{self};
            Point pos;
            const auto defaultPosition = Point{};
            const auto halfWindow = WINDOW_SIZE / 2;
            bool aimed = false;
            for (int x = 0; x < MAP_SIZE; ++x) {
                for (int y = 0; y < MAP_SIZE; ++y) {
                    Point current = damageMap->getGlobalPosition(x, y);
                    int d = 0;
                    for (int i = -halfWindow; i < halfWindow; ++i) {
                        for (int j = -halfWindow; j < halfWindow; ++j) {
                            auto xi = x + i;
                            auto yi = y + j;
                            if (damageMap->isValidIndex(xi, yi)) {
                                d += damageMap->getCellValueByIndex(xi, yi);
                            } else {
                                break;
                            }
                        }
                    }
                    if (d <= damage && attackMap->getCellValueByIndex(x, y) >= 1) {
                        bool captured = mBerserkTools->isMagicMissileAvailable() && target != nullptr
                                        && current.inCircle({*target}, self.getCastRange() - target->getRadius());
                        if (captured) aimed = true;
                        if (d == damage && center.getDistanceTo(current) > center.getDistanceTo(pos))
                            continue;
                        if (aimed && captured || !aimed) {
                            damage = d;
                            pos = current;
                        }
                    }
                }
            }
            this->setTargetPoint(pos);
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


Ptr<GridMap<int, MAP_SIZE, MAP_SIZE>> BerserkBehavior::getDamageMap(Ptr<State> state, bool drawMinions, bool drawWizards,
                                                               bool drawBuildings, bool drawProjectiles) {
    const auto &self = state->self;
    const auto &projectiles = mFilter->getProjectiles();
    const auto &minions = mFilter->getMinions();
    const auto &wizards = mFilter->getWizards();
    const auto &buildings = mFilter->getBuildings();
    const auto &game = state->game;
    const model::Faction faction = self.getFaction();
    const double mapSize = MAP_SIZE;
    const auto cellSize = self.getRadius() / WINDOW_SIZE;
    const auto offset = Point{-mapSize * cellSize / 2, -mapSize / 2 * cellSize};
    GridMap<int, MAP_SIZE, MAP_SIZE> gridMapMinion{Point{self}, cellSize, offset};
    GridMap<int, MAP_SIZE, MAP_SIZE> gridMapWizards{Point{self}, cellSize, offset};
    GridMap<int, MAP_SIZE, MAP_SIZE> gridMapBuildings{Point{self}, cellSize, offset};
    GridMap<int, MAP_SIZE, MAP_SIZE> gridMapProjectiles{Point{self}, cellSize, offset};
    auto resultGridMap = share<GridMap<int, MAP_SIZE, MAP_SIZE>>(Point{self}, cellSize, offset);
    if (drawMinions)
        for (auto&& m : minions) {
            if (m->getFaction() != faction && m->getFaction() != model::Faction::FACTION_NEUTRAL
                && m->getFaction() != model::Faction::FACTION_OTHER) {
                if (m->getType() == model::MinionType::MINION_ORC_WOODCUTTER) {
                    gridMapMinion.drawFilledCircle(Point{*m}, m->getRadius() + game.getOrcWoodcutterAttackRange(),
                                                   game.getOrcWoodcutterDamage());
                } else {
                    gridMapMinion.drawFilledCircle(Point{*m}, m->getRadius() + game.getFetishBlowdartAttackRange(),
                                                   game.getDartDirectDamage());
                }
            }
        }
    if (drawWizards)
        for (auto&& wizard : wizards) {
            if (wizard->getFaction() != faction) {
                gridMapWizards.drawFilledCircle(Point{*wizard}, wizard->getCastRange(),
                                                game.getMagicMissileDirectDamage());
            }
        }
    if (drawBuildings)
        for (auto&& building : buildings) {
            if (building->getFaction() != faction) {
                if (building->getType() == model::BUILDING_GUARDIAN_TOWER)
                    gridMapBuildings.drawFilledCircle(Point{*building}, building->getAttackRange(),
                                                      game.getGuardianTowerDamage());
                else
                    gridMapBuildings.drawFilledCircle(Point{*building}, building->getAttackRange(), game.getFactionBaseDamage());
            }
        }
    if (drawProjectiles)
        for (auto&& projectile : projectiles) {
            if(projectile->getFaction() != faction) {
                double dist = projectile->getDistanceTo(self) * 2;
                Point endPoint(projectile->getX() + std::cos(projectile->getAngle()) * dist,
                               projectile->getY() + std::sin(projectile->getAngle()) * dist);
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
                gridMapProjectiles.drawLine(Point{*projectile}, endPoint, projectileDamage);

            }
        }
    *resultGridMap += gridMapMinion += gridMapWizards += gridMapBuildings += gridMapProjectiles;
    return std::move(resultGridMap);
}


Ptr<GridMap<int, MAP_SIZE, MAP_SIZE>> BerserkBehavior::getAttackMap(Ptr<State> state) {
    const auto &self = state->self;
    const auto &projectiles = mFilter->getProjectiles();
    const auto &minions = mFilter->getMinions();
    const auto &wizards = mFilter->getWizards();
    const auto &buildings = mFilter->getBuildings();
    const auto &game = state->game;
    std::vector<const model::LivingUnit*> enemies;
    std::move(std::find_if(wizards.cbegin(), wizards.cend(), [&self](const model::Wizard* wizard) {
        return wizard->getFaction() != self.getFaction();
    }), wizards.end(), std::inserter(enemies, enemies.end()));

    std::move(std::find_if(buildings.cbegin(), buildings.cend(), [&self](const model::Building* building) {
        return building->getFaction() != self.getFaction();
    }), buildings.end(), std::inserter(enemies, enemies.end()));

    std::move(std::find_if(minions.cbegin(), minions.cend(), [&self](const model::Minion* minion) {
        bool wokeUp = minion->getFaction() == model::FACTION_NEUTRAL && (minion->getSpeedX() != 0 || minion->getSpeedY() != 0);
        return (minion->getFaction() != self.getFaction() && (minion->getFaction() != model::FACTION_NEUTRAL) || wokeUp);
    }), minions.end(), std::inserter(enemies, enemies.end()));

    const model::Faction faction = self.getFaction();
    const double mapSize = MAP_SIZE;
    const auto cellSize = self.getRadius() / WINDOW_SIZE;
    const auto offset = Point{-mapSize * cellSize / 2, -mapSize * cellSize / 2};
    auto gridMap = share<GridMap<int, MAP_SIZE, MAP_SIZE>>(Point{self}, cellSize, offset);
    for (int x = 0; x < MAP_SIZE; ++x) {
        for (int y = 0; y < MAP_SIZE; ++y) {
            Point position = gridMap->getGlobalPosition(x, y);
            int count = 0;
            for (auto&& enemy : enemies) {
                if (position.inCircle({*enemy}, game.getWizardCastRange() - enemy->getRadius())) {
                    ++count;
                }
            }
            gridMap->setCellValue(x, y, count);
        }
    }
    return std::move(gridMap);
}