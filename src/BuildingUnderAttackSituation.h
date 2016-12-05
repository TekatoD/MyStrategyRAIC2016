/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "Situation.h"
#include "Point.h"

class BuildingUnderAttackSituation : public Situation {
public:
    BuildingUnderAttackSituation(const std::string& name, const Point& position, double radius = 50.0);

    void update(Ptr<State> state) override;

private:
    Point mPosition;
    double mRadius;
};


