/**
 * @author arssivka
 * @date 11/26/16
 */

#pragma once


#include <functional>
#include <math.h>
#include "Action.h"
#include "Point.h"
#include "WalkingParameters.h"

class WalkingAction : public Action {
public:
    WalkingAction();

    const Point& getTargetPoint() const;

    void setTargetPoint(const Point& targetPoint);

    const Point& getTrackingPoint() const;

    void setTrackingPoint(const Point& trackingPoint);

    double getSpeedFactor() const;

    void setSpeedFactor(double speedFactor);

    void perform() override;

private:
    Point mTargetPoint;
    Point mTrackingPoint;
    double mSpeedFactor;
};


