/**
 * @author arssivka
 * @date 12/7/16
 */

#pragma once


#include "Mechanism.h"
#include "Point.h"

class HoldingPositionChecker : public Mechanism {
public:
    HoldingPositionChecker(int interval, double radiusThreshold);

    int getInterval() const;

    void setInterval(int interval);

    double getRadiusThreshold() const;

    void setRadiusThreshold(double radiusThreshold);

    bool isHoldingPosition() const;

private:
    void update(Ptr<State> state) override;

private:
    int mInterval;
    double mRadiusThreshold;
    Point mPosition;
    int mTick;
    bool mIsHolding;
};


