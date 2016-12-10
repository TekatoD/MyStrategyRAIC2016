#pragma once


#include <mutex>
#include "Strategy.h"
#include "Controller.h"
#include "BigTeacher.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();

    void move(const model::Wizard& self, const model::World& world,
              const model::Game& game, model::Move& move) override;

private:
    void initialize(Ptr<State>);

private:
    bool mInitialized;
    Controller mGameController;
};
