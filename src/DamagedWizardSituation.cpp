/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#include <algorithm>
#include "DamagedWizardSituation.h"

DamagedWizardSituation::DamagedWizardSituation(const std::string& name, long long id, double threshold, double heals, double maxHeals)
        : Situation(name), mId(id), mMaxHP(maxHeals), mLevel(0) {
    this->setHeals(heals);
    this->setThreshold(threshold);
}

void DamagedWizardSituation::update(Ptr<State> state) {
    auto& world = state->world;
    auto& game = state->game;
    auto& wizards = world.getWizards();
    auto found = std::find_if(wizards.cbegin(), wizards.cend(), [this](const model::Wizard& wizard) {
        return wizard.getOwnerPlayerId() == mId;
    });

    if (found != wizards.cend()) {
        mHP = found->getLife();
        mLevel = found->getLevel();
        mMaxHP = found->getMaxLife();
    } else {
        auto regeneration =   game.getWizardBaseLifeRegeneration()
                            + mLevel * game.getWizardLifeRegenerationGrowthPerLevel();
        this->setHeals(mHP + regeneration);
    }

    if (mHP <= mThreshold) {
        this->setProbability(1.0);
    } else {
        auto factor = mThreshold / mMaxHP / 2.0;
        this->setProbability((mHP - mThreshold) * factor);
    }
}

double DamagedWizardSituation::getThreshold() const {
    return mThreshold;
}

long long double DamagedWizardSituation::getId() const {
    return mId;
}

void DamagedWizardSituation::setThreshold(double threshold) {
    if (threshold < 0) {
        threshold = 0;
    } else if (threshold > mMaxHP) {
        threshold = mMaxHP;
    }
    mThreshold = threshold;
}

void DamagedWizardSituation::setId(long long id) {
    mId = id;
}

double DamagedWizardSituation::getMaxHeals() const {
    return mMaxHP;
}

void DamagedWizardSituation::setMaxHeals(double maxHP) {
    if (maxHP < 0) maxHP = 0;
    if (maxHP < mThreshold) mThreshold = maxHP;
    if (maxHP < mHP) mHP = maxHP;
    mMaxHP = maxHP;
}

double DamagedWizardSituation::getHeals() const {
    return mHP;
}

void DamagedWizardSituation::setHeals(double HP) {
    if (HP < 0) {
        HP = 0;
    } else if (HP > mMaxHP) {
        HP = mMaxHP;
    }
    mHP = HP;
}


