/**
 * @author arssivka
 * @date 12/4/16
 */

#pragma once


#include "Behavior.h"

class CallbackBehavior : public Behavior {
public:
    CallbackBehavior(const std::string& name, const std::function<void(WalkingAction&, CastAction&)>& mover,
                     const std::function<double(Ptr<State>)>& probabilityGenerator);

    void turn() override;

    void update(Ptr<State> state) override;

private:
    std::function<void(WalkingAction&, CastAction&)> mMover;
    std::function<double(Ptr<State>)> mProbabilityGenerator;
};


