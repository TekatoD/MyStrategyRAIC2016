/**
 *  @autor tekatod
 *  @date 12/6/16
 */

#include "Clusterer.h"

Clusterer::Clusterer(double clusterRadius, bool mergeNeeded, double mergeDist) : mClusterRadius(clusterRadius),
                                                                                 mMergeNeeded(mergeNeeded),
                                                                                 mMergeDist(mergeDist){ }


void Clusterer::update(Ptr<State> state) {
    if (mState != state) {
        mState = state;
        mFriendlyClusters.clear();
        mEnemyClusters.clear();
        for(auto&& minion : state->world.getMinions()) {
            model::Faction minionFaction = minion.getFaction();
            if(minionFaction == state->self.getFaction()) {
                this->updateVector(minion, mFriendlyClusters);
            }
            else if(minionFaction != state->self.getFaction() && minionFaction != model::Faction::FACTION_NEUTRAL &&
                    minionFaction != model::Faction::FACTION_OTHER) {
                this->updateVector(minion, mEnemyClusters);
            }
        }
        if (mMergeNeeded) {
            this->mergeClusters(mFriendlyClusters);
            this->mergeClusters(mEnemyClusters);
        }
    }
}


void Clusterer::updateVector(const model::Minion& minion, std::vector<MinionCluster>& clusterVector) {
    bool clusterNotNeeded = false;
    std::find_if(clusterVector.begin(), clusterVector.end(), [&clusterNotNeeded, &minion](auto cluster) {
        clusterNotNeeded = cluster.accamulateXY(minion);
        return clusterNotNeeded;
    });
    if(!clusterNotNeeded) {
        clusterVector.push_back(MinionCluster{mClusterRadius, minion});
    }
}


void Clusterer::mergeClusters(std::vector<MinionCluster> &clusterVector) {
    for(auto&& cluster : clusterVector) {
        double distAlias = mMergeDist;
        auto toDelete = std::find_if(clusterVector.begin(), clusterVector.end(), [&cluster, &distAlias](const auto& cluster2){
            return cluster.mergeWithCluster(cluster2, distAlias) && !(cluster==cluster2);
        });
        if(toDelete != clusterVector.end()) {
            clusterVector.erase(toDelete);
        }
    }
}


double Clusterer::getClusterRadius() const {
    return mClusterRadius;
}


void Clusterer::setClusterRadius(double clusterRadius) {
    mClusterRadius = clusterRadius;
}


bool Clusterer::isMergeNeeded() const {
    return mMergeNeeded;
}


void Clusterer::setMergeNeeded(bool mergeNeeded) {
    mMergeNeeded = mergeNeeded;
}


double Clusterer::getMergeDist() const {
    return mMergeDist;
}


void Clusterer::setMergeDist(double mergeDist) {
    mMergeDist = mergeDist;
}

