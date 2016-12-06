/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#include <algorithm>
#include "DamagedBuildingSituation.h"

DamagedBuildingSituation::DamagedBuildingSituation(const std::string& name, const Point& position, int threshold)
        : Situation(name), mPosition(position), mThreshold(threshold) {}

const Point& DamagedBuildingSituation::getPosition() const {
    return mPosition;
}

void DamagedBuildingSituation::setPosition(const Point& position) {
    mPosition = position;
}

int DamagedBuildingSituation::getThreshold() const {
    return mThreshold;
}

void DamagedBuildingSituation::setThreshold(int threshold) {
    mThreshold = threshold;
}

void DamagedBuildingSituation::update(Ptr<State> state) {
    auto& self = state->self;
    auto& world = state->world;
    auto& game = state->game;
    auto& buildings = world.getBuildings();
    auto& minions = world.getMinions();
    auto& wizards = world.getWizards();
    auto found = std::find_if(buildings.cbegin(), buildings.cend(), [this](const model::Building& building) {
        return mPosition.inCircle({building}, 1.0);
    });

    if (found != buildings.cend()) {
        this->setProbability((found->getLife() < mThreshold) ? 1.0 : 0.0);
    } else {
        bool isMinionsSee = std::find_if(minions.cbegin(), minions.cend(), [this, &game, &self](const model::Minion& minion) {
            return    minion.getFaction() == self.getFaction()
                   && mPosition.inCircle({minion}, game.getMinionVisionRange());
        }) != minions.cend();

        bool isWizardsSee = std::find_if(wizards.cbegin(), wizards.cend(), [this, &game, &self](const model::Wizard& wizard) {
            return wizard.getFaction() == self.getFaction()
                   && mPosition.inCircle({wizard}, game.getWizardVisionRange());
        }) != wizards.cend();

        if (isMinionsSee || isWizardsSee) {
            this->disable();
        }
    }
}
