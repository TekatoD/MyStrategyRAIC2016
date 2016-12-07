/**
 *  @autor tekatod
 *  @date 12/6/16
 */
#include "MinionCluster.h"

MinionCluster::MinionCluster(double clusterRadius, double x, double y) :mClusterRadius(clusterRadius),
                                                                        mCenter(x, y), mCount(1),
                                                                        mXRange{x, x}, mYRange{y, y} { }


MinionCluster::MinionCluster(double clusterRadius, const model::Minion& minion) :mClusterRadius(clusterRadius),
                                                                        mCenter(minion.getX(), minion.getY()),
                                                                                 mCount(1) {
    mXRange.first = minion.getX();
    mXRange.second = minion.getX();
    mYRange.first = minion.getY();
    mYRange.second = minion.getY();
}


bool MinionCluster::accamulateXY(double x, double y) {
    if (mCenter.inCircle(Point(x, y), mClusterRadius)) {
        mCount += 1;
        if (x < mXRange.first) {
            mXRange.first = x;
        }
        else if(x > mXRange.second) {
            mXRange.second = x;
        }
        if (y < mYRange.first) {
            mYRange.first = y;
        }
        else if(y > mYRange.second) {
            mYRange.second = y;
        }
        return true;
    }
    return false;
}


bool MinionCluster::accamulateXY(const model::Minion& minion) {
    return this->accamulateXY(minion.getX(), minion.getY());
}


int MinionCluster::getCount() const {
    return mCount;
}


double MinionCluster::getX() const {
    return mCenter.getX();
}


double MinionCluster::getY() const {
    return mCenter.getY();
}


Point MinionCluster::getCenter() const {
    return mCenter;
}

std::pair<double, double> MinionCluster::getXrange() const {
    return mXRange;
}


std::pair<double, double> MinionCluster::getYRange() const {
    return mYRange;
}


bool MinionCluster::mergeWithCluster(const MinionCluster& other, double mergeDist) {
    if(other == *this) {
        return false;
    }
    if(mCenter.inCircle(other.mCenter, (mClusterRadius + other.mClusterRadius + mergeDist))) {
        mCenter.setX((mCenter.getX() + other.mCenter.getX()) / 2);
        mCenter.setY((mCenter.getY() + other.mCenter.getY()) / 2);
        if (other.mXRange.first < mXRange.first) {
            mXRange.first = other.mXRange.first;
        }
        if (other.mXRange.second > mXRange.second) {
            mXRange.second = other.mXRange.second;
        }
        if (other.mYRange.first < mYRange.first) {
            mYRange.first = other.mYRange.first;
        }
        if (other.mYRange.second > mYRange.second) {
            mYRange.second = other.mYRange.second;
        }
        mCount += other.mCount;
        return true;
    }
    return false;
}


bool MinionCluster::operator==(const MinionCluster& rhs) const {
    return mClusterRadius == rhs.mClusterRadius &&
           mCenter == rhs.mCenter &&
           mCount == rhs.mCount &&
           mXRange == rhs.mXRange &&
           mYRange == rhs.mYRange;
}


bool MinionCluster::operator!=(const MinionCluster& rhs) const {
    return !(rhs == *this);
}


std::ostream& operator<<(std::ostream& os, const MinionCluster& minionCluster) {
    os << "Center(" << minionCluster.mCenter << ")" << " XRange(" << minionCluster.mXRange.first
                                                    << "," << minionCluster.mXRange.second << ")"
                                                    << " YRange(" << minionCluster.mYRange.first
                                                    << "," << minionCluster.mYRange.second << ")"
                                                    << " count: " << minionCluster.mCount;
    return os;;
}
