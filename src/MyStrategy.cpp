#include "MyStrategy.h"

MyStrategy::MyStrategy() : mInitialized(false) {}

void MyStrategy::move(const model::Wizard& self,
                      const model::World& world,
                      const model::Game& game,
                      model::Move& move) {
    // Create new state object
    auto state = share<State>(std::ref(self), std::ref(world), std::ref(game), std::ref(move));
    // Init controller
    if (!mInitialized) this->initialize();
    // Update world
    mGameController.update(state);
    // Move!
    mGameController.turn();
}

void MyStrategy::initialize() {

    // DO NOT EDIT WHAT'S BELOW!
    mInitialized = true;
}
