/**
 *  @autor tekatod
 *  @date 12/6/16
 */

#include "Clusterer.h"
#include "Log.h"

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
    if(mFriendlyClusters.size() > 0 || mEnemyClusters.size() > 0) {
        Log(DEBUG) << "friendle clusters!!!";
        for (auto &&cluster : mFriendlyClusters) {
            Log(DEBUG) << cluster;
        }
        Log(DEBUG) << "evil clusters!!!";
        for (auto &&cluster : mEnemyClusters) {
            Log(DEBUG) << cluster;
        }
        Log(DEBUG) << "clusters!!!";
    }
}


void Clusterer::updateVector(const model::Minion& minion, std::vector<MinionCluster>& clusterVector) {
    bool clusterNotNeeded = false;
    auto res = std::find_if(clusterVector.begin(), clusterVector.end(), [&clusterNotNeeded, &minion](MinionCluster& cluster) {
        clusterNotNeeded = cluster.accamulateXY(minion);
        return clusterNotNeeded;
    });
//    if(!clusterNotNeeded) {
    if(res == clusterVector.end()) {
        clusterVector.push_back(MinionCluster{mClusterRadius, minion});
    }
}


void Clusterer::mergeClusters(std::vector<MinionCluster> &clusterVector) {
    double distAlias = mMergeDist;
    for(int i = 0; i < clusterVector.size(); ++i) {
        auto toErase = std::remove_if(clusterVector.begin(), clusterVector.end(), [&clusterVector, i, distAlias](const auto& cluster2){
            return clusterVector[i].mergeWithCluster(cluster2, distAlias);
        });
        if(toErase != clusterVector.end()) {
            clusterVector.erase(toErase, clusterVector.end());
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

