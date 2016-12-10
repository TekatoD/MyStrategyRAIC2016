/**
 *  @autor arssivka
 *  @date 12/6/16
 */
#pragma once


#include "Mechanism.h"
#include "MinionCluster.h"
#include <algorithm>

class Clusterer : public Mechanism {
public:
    Clusterer(double clusterRadius, bool mergeNeeded, double mergeDistance);

    double getClusterRadius() const;

    void setClusterRadius(double radius);

    bool isAutoMergeEnabled() const;

    void setAutoMerge(bool autoMerge);

    double getMergeDistance() const;

    void setMergeDistance(double distance);

    void update(Ptr<State> state) override;

    const std::vector<MinionCluster>& getFriendlyClusters() const;

    const std::vector<MinionCluster>& getEnemyClusters() const;

private:
    void updateVector(const model::Minion& minion, std::vector<MinionCluster>& clusterVector) const;

    void mergeClusters(std::vector<MinionCluster> &clusterVector) const;

    void updateClusterer() const;

private:
    Ptr<State> mState;
    double mClusterRadius;
    mutable bool mUpdated;
    mutable std::vector<MinionCluster> mFriendlyClusters;
    mutable std::vector<MinionCluster> mEnemyClusters;
    bool mAutoMerge;
    double mMergeDistance;


};

