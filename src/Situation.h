/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <memory>
#include <ostream>
#include "State.h"
#include "Ranked.h"
#include "Refreshable.h"
#include "NonCopyable.h"
#include "Named.h"
#include "SelfDisabler.h"

class Situation: public SelfDisabler, public Named, public Ranked, public Refreshable, NonCopyable {
public:
    Situation(const std::string& name) : Named(name) {}

    friend std::ostream& operator<<(std::ostream& os, const Situation& situation);
};


