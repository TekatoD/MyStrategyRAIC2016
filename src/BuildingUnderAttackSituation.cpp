/**
 * @author arssivka
 * @date 12/4/16
 */

#include <algorithm>
#include "BuildingUnderAttackSituation.h"


BuildingUnderAttackSituation::BuildingUnderAttackSituation(const std::string& name,
                                                     const Point& position, double radius)
        : Situation(name), mPosition(position), mRadius(radius) {}


void BuildingUnderAttackSituation::update(Ptr<State> state) {
    if (this->isDisabled()) return;
    auto& world = state->world;

    auto& buildings = world.getBuildings();
    if (std::find_if(buildings.cbegin(), buildings.cend(), [this](const auto& b) {
        return mPosition.inCircle({b}, 1.0);
    }) == buildings.cend()) {
        this->disable();
        this->setProbability(0.0);
        return;
    }

    auto& self = state->self;
    auto faction = self.getFaction();
    auto& minions = world.getMinions();
    if (std::find_if(minions.cbegin(), minions.cend(), [this, faction](const auto& m) {
        return  m.getFaction() != faction && mPosition.inCircle({m}, mRadius);
    }) == minions.cend()) {
        this->setProbability(0.0);
    } else {
        this->setProbability(1.0);
    }
}
