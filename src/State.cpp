/**
 * @author arssivka
 * @date 11/25/16
 */

#include "State.h"

State::State(const model::Wizard& self, const model::World& world, const model::Game& game, model::Move& move)
        : self(self), world(world), game(game), move(move) { }
