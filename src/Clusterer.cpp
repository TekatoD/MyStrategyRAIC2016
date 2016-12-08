/**
 *  @autor tekatod
 *  @date 12/6/16
 */

#include "Clusterer.h"
#include "Log.h"

Clusterer::Clusterer(double clusterRadius, bool mergeNeeded, double mergeDistance)
        : mClusterRadius(clusterRadius), mAutoMerge(mergeNeeded), mMergeDistance(mergeDistance){ }


void Clusterer::update(Ptr<State> state) {
    if (mState != state) {
        mState = state;
        mUpdated = false;
    }
}

void Clusterer::updateClusterer() const {
    if (!mUpdated && mState != nullptr) {
        mUpdated = false;
        mFriendlyClusters.clear();
        mEnemyClusters.clear();
        for (auto&& minion : mState->world.getMinions()) {
            model::Faction minionFaction = minion.getFaction();
            if (minionFaction == mState->self.getFaction()) {
                this->updateVector(minion, mFriendlyClusters);
            } else if (minionFaction != mState->self.getFaction() &&
                       minionFaction != model::Faction::FACTION_NEUTRAL &&
                       minionFaction != model::Faction::FACTION_OTHER) {
                this->updateVector(minion, mEnemyClusters);
            }
        }
        if (mAutoMerge) {
            this->mergeClusters(mFriendlyClusters);
            this->mergeClusters(mEnemyClusters);
        }
//        if (this->mFriendlyClusters.size() > 0 || this->mEnemyClusters.size() > 0) {
//            Log(DEBUG) << "friendle clusters!!!";
//            for (auto&& cluster : this->mFriendlyClusters) {
//                Log(DEBUG) << cluster;
//            }
//            Log(DEBUG) << "evil clusters!!!";
//            for (auto&& cluster : this->mEnemyClusters) {
//                Log(DEBUG) << cluster;
//            }
//            Log(DEBUG) << "clusters!!!";
//        }
    }
}


void Clusterer::updateVector(const model::Minion& minion, std::vector<MinionCluster>& clusterVector) const {
    bool clusterNotNeeded = false;
    auto res = std::find_if(clusterVector.begin(), clusterVector.end(), [&clusterNotNeeded, &minion](MinionCluster& cluster) {
        clusterNotNeeded = cluster.accamulateXY(minion);
        return clusterNotNeeded;
    });
    if(res == clusterVector.end()) {
        clusterVector.push_back(MinionCluster{mClusterRadius, minion});
    }
}


void Clusterer::mergeClusters(std::vector<MinionCluster> &clusterVector) const {
    for(int i = 0; i < clusterVector.size(); ++i) {
        auto toErase = std::remove_if(clusterVector.begin(), clusterVector.end(), [this, &clusterVector, i](const auto& cluster2){
            return clusterVector[i].mergeWithCluster(cluster2, mMergeDistance);
        });
        clusterVector.erase(toErase, clusterVector.end());
    }
}


double Clusterer::getClusterRadius() const {
    return mClusterRadius;
}


void Clusterer::setClusterRadius(double radius) {
    mClusterRadius = radius;
}


bool Clusterer::isAutoMergeEnabled() const {
    return mAutoMerge;
}


void Clusterer::setAutoMerge(bool autoMerge) {
    mAutoMerge = autoMerge;
}


double Clusterer::getMergeDistance() const {
    return mMergeDistance;
}


void Clusterer::setMergeDistance(double distance) {
    mMergeDistance = distance;
}

const std::vector<MinionCluster>& Clusterer::getFriendlyClusters() const {
    this->updateClusterer();
    return mFriendlyClusters;
}

const std::vector<MinionCluster>& Clusterer::getEnemyClusters() const {
    this->updateClusterer();
    return mEnemyClusters;
}

