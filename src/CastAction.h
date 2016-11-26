/**
 * @author arssivka
 * @date 11/26/16
 */

#pragma once


#include <algorithm>
#include "Action.h"
#include "Point.h"

class CastAction : public Action {
public:
    CastAction();

    void kickStaff() noexcept;;

    void castMagicMissile(const Point& targetPoint, double maxCastDistance, double minCastDistance = 0.0) noexcept;

    void castFrostBolt(const Point& targetPoint, double maxCastDistance, double minCastDistance = 0.0) noexcept;

    void castFireball(const Point& targetPoint, double maxCastDistance, double minCastDistance = 0.0) noexcept;

    void castHaste(long long targetId = -1) noexcept;

    void castShield(long long targetId = -1) noexcept;

    void perform() override;

private:
    inline void castProjectileRequest(model::ActionType action, const Point& targetPoint,
                                      double maxCastDistance, double minCastDistance) noexcept;

    inline void castSpellRequest(model::ActionType action, long long targetId = -1) noexcept;

    inline void tryKickStaff(State::Ptr state) noexcept;
    
    inline void castProjectile(State::Ptr state) noexcept;

    inline void castSpell(State::Ptr state) noexcept;

    bool checkSkill(model::SkillType skill, State::Ptr state) noexcept;

private:
    model::ActionType mAction;
    Point mTargetPoint;
    double mMinCastDistance;
    double mMaxCastDistance;
    long long mTargetId;
};


