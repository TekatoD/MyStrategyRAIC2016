/**
 * @author arssivka
 * @date 11/25/16
 */

#pragma once


#include "model/Wizard.h"
#include "model/World.h"
#include "model/Game.h"
#include "model/Move.h"
#include "NonCopyable.h"
#include "Pointer.h"

struct State: public Pointer<State>, NonCopyable {
public:
    State(const model::Wizard& self, const model::World& world, const model::Game& game, model::Move& move);

    const model::Wizard& self;
    const model::World& world;
    const model::Game& game;
    model::Move& move;
};


