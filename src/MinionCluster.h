/**
 *  @autor tekatod
 *  @date 12/6/16
 */
#pragma once


#include <utility>
#include "model/Minion.h"
#include "Point.h"

class MinionCluster {
public:
    MinionCluster(double clusterRadius, double x, double y);

    MinionCluster(double clusterRadius, const model::Minion& minion);

    bool accamulateXY(double x, double y);

    bool accamulateXY(const model::Minion& minion);

    int getCount() const;

    double getX() const;

    double getY() const;

    Point getCenter() const;

    std::pair<double, double> getXrange() const;

    std::pair<double, double> getYRange() const;

    bool mergeWithCluster(const MinionCluster& other, double mergeDist);

    bool operator==(const MinionCluster& rhs) const;

    bool operator!=(const MinionCluster& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const MinionCluster& minionCluster);

private:

    double mClusterRadius;
    Point mCenter;
    int mCount;
    std::pair<double, double> mXRange;
    std::pair<double, double> mYRange;
};

