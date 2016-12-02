/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <memory>
#include "State.h"

class Refreshable {
public:
    virtual void update(Ptr<State> state) = 0;

    virtual ~Refreshable() {}
};