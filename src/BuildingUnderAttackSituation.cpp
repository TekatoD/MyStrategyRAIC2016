/**
 * @author arssivka
 * @date 12/4/16
 */

#include <algorithm>
#include "BuildingUnderAttackSituation.h"


BuildingUnderAttackSituation::BuildingUnderAttackSituation(const std::string& name,
                                                     const Point& position, double radius, Ptr<Clusterer> clusterer)
        : Situation(name), mPosition(position), mRadius(radius), mClusterer(std::move(clusterer)) {}


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
    auto clusters = mClusterer->getEnemyClusters();
    if (std::find_if(clusters.cbegin(), clusters.cend(), [this](const MinionCluster& m) {
        return  mPosition.inCircle({m}, mRadius);
    }) == clusters.cend()) {
        this->setProbability(0.0);
    } else {
        this->setProbability(1.0);
    }
}
