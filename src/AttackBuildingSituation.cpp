/**
 *  @autor arssivka
 *  @date 12/9/16
 */

#include "AttackBuildingSituation.h"


AttackBuildingSituation::AttackBuildingSituation(const std::string& name,
                                                 const Point& position, double radius, double factor,
                                                 Ptr<Clusterer> clusterer)
        : Situation(name), mFactor(factor), mPosition(position), mRadius(radius), mClusterer(std::move(clusterer)) {}


void AttackBuildingSituation::update(Ptr<State> state) {
    if (this->isDisabled()) return;
    const auto& world = state->world;
    const auto& buildings = world.getBuildings();
    const auto& self = state->self;
    const auto& game = state->game;
    if (mPosition.inCircle({self}, self.getVisionRange())
        && std::find_if(buildings.cbegin(), buildings.cend(), [this](const auto& b) {
                return mPosition.inCircle({b}, 1.0);
            }) == buildings.cend()) {
        this->disable();
        return;
    }


    auto clusters = mClusterer->getFriendlyClusters();
    if (clusters.empty()) {
        this->setProbability(0.0);
        return;
    }

    const auto& cluster = std::min_element(clusters.cbegin(), clusters.cend(), [this](const MinionCluster& m1,
                                                                                      const MinionCluster& m2) {
            return mPosition.getDistanceTo(Point{m1}) < mPosition.getDistanceTo(Point{m2});
    });
    if (!mPosition.inCircle({*cluster}, mRadius)) {
        this->setProbability(1.0 / mPosition.getDistanceTo(Point{*cluster}));
    } else {
        const auto& wizards = world.getWizards();
        const auto& self = state->self;
        long count = std::count_if(wizards.cbegin(), wizards.cend(), [this, &self](const model::Wizard& wizard) {
            return wizard.getId() != self.getId() && mPosition.inCircle({wizard}, mRadius);
        });
        this->setFactor(mFactor + 4.0 - count);
        this->setProbability(1.0);
    }
}