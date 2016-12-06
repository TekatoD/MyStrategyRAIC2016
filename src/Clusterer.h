/**
 *  @autor tekatod
 *  @date 12/6/16
 */
#pragma once


#include "Mechanism.h"
#include "MinionCluster.h"
#include <algorithm>

class Clusterer : public Mechanism {
public:
    Clusterer(double clusterRadius, bool mergeNeeded, double mergeDist);

    double getClusterRadius() const;

    void setClusterRadius(double clusterRadius);

    bool isMergeNeeded() const;

    void setMergeNeeded(bool mMergeNeeded);

    double getMergeDist() const;

    void setMergeDist(double mMergeDist);

    void update(Ptr<State> state) override;

private:
    void updateVector(const model::Minion& minion, std::vector<MinionCluster>& clusterVector);

    void mergeClusters(std::vector<MinionCluster> &clusterVector);

    Ptr<State> mState;
    double mClusterRadius;
    std::vector<MinionCluster> mFriendlyClusters;
    std::vector<MinionCluster> mEnemyClusters;
    bool mMergeNeeded;
    double mMergeDist;
};

