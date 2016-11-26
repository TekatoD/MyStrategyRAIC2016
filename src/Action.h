/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "Refreshable.h"

class Action : public Pointer<Action>, public Refreshable, NonCopyable {
public:
    void update(State::Ptr state) override {
        mState = state;
    }

    virtual void perform() = 0;

protected:
    State::Ptr mState;
};