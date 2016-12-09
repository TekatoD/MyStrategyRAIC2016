/**
 * @author arssivka
 * @date 12/3/16
 */

#include "WorldFilter.h"


void WorldFilter::update(Ptr<State> state) {
    if (state != mState) {
        mState = state;
        mUpdated = false;
    }
}


double WorldFilter::getRadius() const {
    this->updateProxies();
    return mRadius;
}


const VectorProxy<model::Tree>& WorldFilter::getTrees() const {
    this->updateProxies();
    return mTrees;
}


const VectorProxy<model::Wizard>& WorldFilter::getWizards() const {
    this->updateProxies();
    return mWizards;
}


const VectorProxy<model::Building>& WorldFilter::getBuildings() const {
    this->updateProxies();
    return mBuildings;
}


const VectorProxy<model::Minion>& WorldFilter::getMinions() const {
    this->updateProxies();
    return mMinions;
}


const VectorProxy<model::Bonus>& WorldFilter::getBonuses() const {
    this->updateProxies();
    return mBonuses;
}


const VectorProxy<model::Projectile>& WorldFilter::getProjectiles() const {
    this->updateProxies();
    return mProjectiles;
}


void WorldFilter::setRadius(double radius) {
    mRadius = radius;
    if (mState != nullptr && mUpdated) {
        mUpdated = false;
        this->updateProxies();
    }
}


void WorldFilter::updateProxies() const {
    if (!mUpdated) {
        mUpdated = true;
        const auto& world = mState->world;
        this->updateProxy(world.getBonuses(), mBonuses);
        this->updateProxy(world.getBuildings(), mBuildings);
        this->updateProxy(world.getTrees(), mTrees);
        this->updateProxy(world.getMinions(), mMinions);
        this->updateProxy(world.getWizards(), mWizards);
    }
}


WorldFilter::WorldFilter(double radius)
        : mRadius(radius) {}
