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

class Situation: public Pointer<Situation>, public Ranked, public Refreshable, NonCopyable {};


