/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "Situation.h"
#include "Point.h"
#include "Clusterer.h"

class BuildingUnderAttackSituation : public Situation {
public:
    BuildingUnderAttackSituation(const std::string& name, const Point& position, double radius, Ptr<Clusterer> clusterer);

    virtual void update(Ptr<State> state) override;

private:
    Ptr<Clusterer> mClusterer;
    Point mPosition;
    double mRadius;
};


