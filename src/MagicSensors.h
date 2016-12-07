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

    double correctDirection(double direction, double segmentSize);

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


