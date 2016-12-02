#pragma once


#include <mutex>
#include "Strategy.h"
#include "GameController.h"
#include "Teacher.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();

    void move(const model::Wizard& self, const model::World& world, const model::Game& game, model::Move& move) override;

    void initialize();
private:
    bool mInitialized;
    GameController<Teacher> mGameController;
};
