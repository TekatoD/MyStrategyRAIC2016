/**
 *  @autor arssivka
 *  @date 12/9/16
 */

#pragma once


#include "Point.h"
#include "Situation.h"
#include "Clusterer.h"

class AttackBuildingSituation : public Situation {
public:
    AttackBuildingSituation(const std::string& name, const Point& position, double radius, double factor, Ptr<Clusterer> clusterer);

    virtual void update(Ptr<State> state) override;

private:
    Ptr<Clusterer> mClusterer;
    Point mPosition;
    double mRadius;
    double mFactor;
};


