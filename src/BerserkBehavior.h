/**
 *  @autor arssivka
 *  @date 12/6/16
 */

#pragma once


#include <cmath>
#include <math.h>
#include "Behavior.h"
#include "BerserkTools.h"
#include "PathFinder.h"
#include "MagicSensors.h"
#include "WalkingBehavior.h"
#include "GridMap.h"

class BerserkBehavior : public WalkingBehavior {
public:
    static constexpr size_t MAP_SIZE = 100;

    BerserkBehavior(const std::string& name, const Point& position, double acceptedRadius, double factor,
                    bool berserkMode, Ptr<PathFinder> finder, Ptr<MagicSensors> sensors,
                    Ptr<BerserkTools> berserkTools, Ptr<WorldFilter> filter)
            : WalkingBehavior(name, position, acceptedRadius, finder, sensors), mTargetPoint(position),
              mBerserkMode(berserkMode), mBerserkTools(std::move(berserkTools)), mFilter(std::move(filter)) {
        this->setFactor(factor);
    }

    void prepare(Ptr<State> state) override;

    void update(Ptr<State> state) override;

private:
    bool isBerserkMode() const;

    void setBerserkMode(bool berserkMode);

    Ptr<GridMap<int, MAP_SIZE, MAP_SIZE>> getDamageMap(Ptr<State> state, bool drawMinions, bool drawWizards,
                                                        bool drawBuildings, bool drawProjectiles);

    Ptr<GridMap<int, MAP_SIZE, MAP_SIZE>> getAttackMap(Ptr<State> state);;


private:
    Point mTargetPoint;
    bool mBerserkMode;
    Ptr<BerserkTools> mBerserkTools;
    Ptr<WorldFilter> mFilter;
};
