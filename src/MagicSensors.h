/**
 *  @autor arssivka
 *  @date 12/7/16
 */

#pragma once


#include <vector>
#include <algorithm>
#include <cmath>
#include <ostream>
#include "Point.h"
#include "Mechanism.h"
#include "WorldFilter.h"

class MagicSensors : public Mechanism {
public:
    MagicSensors(Ptr<WorldFilter> filter, size_t count, double radius);

    void update(Ptr<State> state) override;

    double correctDirection(double direction, double segmentSize) const;

    Point correctTarget(const Point& origin, double orientation, const Point& target) const {
        auto corrected = this->correctDirection(orientation, M_PI_2);
        Point result = {target};
        if (orientation != corrected) {
            Point local = target - origin;
            auto csin = std::sin(orientation - corrected);
            auto ccos = std::cos(orientation - corrected);
            result.setX(local.getX() * ccos - local.getY() * csin + origin.getX());
            result.setY(local.getX() * csin + local.getY() * ccos + origin.getY());
        }
        return result;
    }

    double getRadius() const;

    bool isTriggered() const;

    friend std::ostream& operator<<(std::ostream& os, const MagicSensors& sensors);

private:
    Ptr<WorldFilter> mFilter;
    std::vector<Point> mPositions;
    std::vector<bool> mSensors;
    double mRadius;
    bool mTriggered;
};


