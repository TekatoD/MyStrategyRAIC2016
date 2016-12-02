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

    template <class Func>
    WalkingAction(Func&& func) : mPathPlanner(std::forward<Func>(func)) {}


    template <class Func>
    void setPathPlanner(Func&& func) {
        mPathPlanner = std::forward<Func>(func);
    }

    void perform() override;

private:
    std::function<WalkingParameters(Ptr<State>)> mPathPlanner;
};


