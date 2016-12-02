/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include <memory>
#include "State.h"
#include "Ranked.h"
#include "Refreshable.h"
#include "NonCopyable.h"
#include "Named.h"

class Situation: public Named, public Ranked, public Refreshable, NonCopyable {
public:
    Situation(const std::string& name) : Named(name) {}
};


