/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "Refreshable.h"

class Action: public Refreshable, NonCopyable {
public:
    void update(Ptr<State> state) override {
        mState = state;
    }

    virtual void perform() = 0;

protected:
    Ptr<State> mState;
};