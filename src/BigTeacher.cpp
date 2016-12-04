/**
 *  @autor arssivka
 *  @date 12/2/16
 */

#include "BigTeacher.h"

void BigTeacher::feedBehavior(Ptr<Behavior> behavior) {

}

void BigTeacher::feedSituation(Ptr<Situation> situation) {
    if (mLastSituation != situation->getName()) {
        mLastSituation = situation->getName();
        mSituationUpdated = true;
    }
    else {
        mSituationUpdated = false;
    }
}

void BigTeacher::feedRelationship(Ptr<Relationship>) {

}

void BigTeacher::update(Ptr<State> state) {
    if (mSituationUpdated) {
        //mLastScore = state-> TODO: Get Score from player
        mLastTick = state->world.getTickCount();
    }
    else {
        //TODO: Check if score changed. If yes update everything
    }

}

void BigTeacher::Being::updateScore(double score, int ticks, Ptr<State> state) {
    mScore = score * (state->self.getLife() / state->self.getMaxLife()) * (1 / ticks);
    mChanged = true;
}

BigTeacher::Being BigTeacher::Being::makeChild(const Being& partner) {
    Being child;
    child.mFactors.reserve(mFactors.size());
    auto it = mFactors.cbegin();
    int halfSize = mFactors.size() / 2;
    std::advance(it, halfSize);
    child.mFactors.insert(mFactors.cbegin(), it);
    it = partner.mFactors.cbegin();
    std::advance(it, halfSize);
    child.mFactors.insert(it, partner.mFactors.cend());
    return std::move(child);
}

BigTeacher::Incubator::Incubator(size_t generationSize) {
    mGeneration.reserve(generationSize);
}

void BigTeacher::Incubator::init() {
    //TODO:: Initialize initial generation here!
}

void BigTeacher::Incubator::update(double score, int ticks, Ptr<State> state,
                                   const std::set<Ptr<Behavior>>& behaviorSet) {
    bool allUpdated = false;
    bool neededUpdated = false;
    for(auto&& being : mGeneration) {
        if(!neededUpdated) {
            bool found = true;
            for(auto&& behavior : behaviorSet) {
                auto factor = being.mFactors.find(behavior->getName());
                if(factor->second != behavior->getFactor()) {
                    found = false;
                    break;
                }
            }
            if(found) {
                neededUpdated = true;
                being.updateScore(score, ticks, state);
            }
        }
        allUpdated &= being.mChanged;
    }
    if (allUpdated) {
        std::sort(mGeneration.begin(), mGeneration.end(),[](const Being& being1, const Being& being2){
            return being1.mScore > being2.mScore;
        });
        size_t b = mGeneration.size() / 2;
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<size_t> distr(0, b);
        auto it = mGeneration.begin();
        std::advance(it, mGeneration.size() / 2);
        for(; b < mGeneration.size(); ++b) {
            size_t father = distr(eng);
            size_t mother = distr(eng);
            mGeneration[b] = mGeneration[mother].makeChild(mGeneration[father]);
        }
        for(auto&& behavior : behaviorSet) {
            auto factor = mGeneration[0].mFactors.find(behavior->getName());
            behavior->setFactor(factor->second);
        }
    }
    else {
        auto toCheck = std::find_if(mGeneration.begin(), mGeneration.end(), [](const Being& being){
            return !being.mChanged;
        });
        for(auto&& behavior : behaviorSet) {
            auto factor = toCheck->mFactors.find(behavior->getName());
            behavior->setFactor(factor->second);
        }
    }
}

void BigTeacher::Incubator::setNeededFactors(const std::set<Ptr<Behavior>>& behaviorSet) {
    auto toCheck = std::find_if(mGeneration.begin(), mGeneration.end(), [](const Being& being){
        return !being.mChanged;
    });
    if (toCheck != mGeneration.end()) {
        for(auto&& behavior : behaviorSet) {
            auto factor = toCheck->mFactors.find(behavior->getName());
            behavior->setFactor(factor->second);
        }
    }
    else {
        for(auto&& behavior : behaviorSet) {
            auto factor = mGeneration[0].mFactors.find(behavior->getName());
            behavior->setFactor(factor->second);
        }
    }
}