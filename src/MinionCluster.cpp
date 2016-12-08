/**
 *  @autor tekatod
 *  @date 12/6/16
 */
#include "MinionCluster.h"

MinionCluster::MinionCluster(double clusterRadius, double x, double y) :mClusterRadius(clusterRadius),
                                                                        mCenter(x, y), mCount(1),
                                                                        mXEdges{x, x}, mYEdges{y, y} { }


MinionCluster::MinionCluster(double clusterRadius, const model::Minion& minion) :mClusterRadius(clusterRadius),
                                                                        mCenter(minion.getX(), minion.getY()),
                                                                                 mCount(1) {
    mXEdges.first = minion.getX();
    mXEdges.second = minion.getX();
    mYEdges.first = minion.getY();
    mYEdges.second = minion.getY();
}


bool MinionCluster::accamulateXY(double x, double y) {
    if (mCenter.inCircle(Point(x, y), mClusterRadius)) {
        mCount += 1;
        if (x < mXEdges.first) {
            mXEdges.first = x;
        }
        else if(x > mXEdges.second) {
            mXEdges.second = x;
        }
        if (y < mYEdges.first) {
            mYEdges.first = y;
        }
        else if(y > mYEdges.second) {
            mYEdges.second = y;
        }
        mCenter.setX((mCenter.getX() + x) / 2);
        mCenter.setY((mCenter.getY() + y) / 2);
        mXRange = std::abs(mXEdges.first - mXEdges.second);
        mYRange = std::abs(mYEdges.first - mYEdges.second);
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

double MinionCluster::getXrange() const {
    return mXRange;
}


double MinionCluster::getYRange() const {
    return mYRange;
}


bool MinionCluster::mergeWithCluster(const MinionCluster& other, double mergeDist) {
    if(other == *this) {
        return false;
    }
    if(mCenter.inCircle(other.mCenter, (mClusterRadius + other.mClusterRadius + mergeDist))) {
        mCenter.setX((mCenter.getX() + other.mCenter.getX()) / 2);
        mCenter.setY((mCenter.getY() + other.mCenter.getY()) / 2);
        if (other.mXEdges.first < mXEdges.first) {
            mXEdges.first = other.mXEdges.first;
        }
        if (other.mXEdges.second > mXEdges.second) {
            mXEdges.second = other.mXEdges.second;
        }
        if (other.mYEdges.first < mYEdges.first) {
            mYEdges.first = other.mYEdges.first;
        }
        if (other.mYEdges.second > mYEdges.second) {
            mYEdges.second = other.mYEdges.second;
        }
        mCount += other.mCount;
        mXRange = std::abs(mXEdges.first - mXEdges.second);
        mYRange = std::abs(mYEdges.first - mYEdges.second);
        return true;
    }
    return false;
}


bool MinionCluster::operator==(const MinionCluster& rhs) const {
    return mClusterRadius == rhs.mClusterRadius &&
           mCenter == rhs.mCenter &&
           mCount == rhs.mCount &&
           mXEdges == rhs.mXEdges &&
           mYEdges == rhs.mYEdges;
}


bool MinionCluster::operator!=(const MinionCluster& rhs) const {
    return !(rhs == *this);
}


std::ostream& operator<<(std::ostream& os, const MinionCluster& minionCluster) {
    os << "Center(" << minionCluster.mCenter << ")" << " XRange: " << minionCluster.mXRange
                                                    << " YRange: " << minionCluster.mYRange
                                                    << " count: " << minionCluster.mCount;
    return os;;
}
