/**
 * @author arssivka
 * @date 11/26/16
 */

#pragma once


#include "Point.h"

class WalkingParameters {
public:
    WalkingParameters();

    WalkingParameters(WalkingParameters&&) = default;

    WalkingParameters(const WalkingParameters&) = default;

    WalkingParameters(double speedFactor, const Point& targetPoint, const Point& trackingPoint);

    double getSpeedFactor() const;

    void setSpeedFactor(double speedFactor);

    const Point& getTargetPoint() const;

    void setTargetPoint(const Point& targetPoint);

    const Point& getTrackingPoint() const;

    void setTrackingPoint(const Point& trackingPoint);

    WalkingParameters& operator=(WalkingParameters&& rhs) = default;

    WalkingParameters& operator=(const WalkingParameters& rhs) = default;

private:
    double mSpeedFactor;
    Point mTargetPoint;
    Point mTrackingPoint;
};


