/**
 * @author arssivka
 * @date 12/4/16
 */

#include "BonusExistsSituation.h"
#include "Log.h"


BonusExistsSituation::BonusExistsSituation(const std::string& name, const Point& position)
        : Situation(name), mPosition(position), mExists(false) {}


void BonusExistsSituation::update(Ptr<State> state) {
    auto& world = state->world;
    auto& game = state->game;
    auto& self = state->self;
    for (auto bonus : world.getBonuses()) {
        Log(DEBUG) << Point{bonus};
        if (mPosition.inCircle({bonus}, 30.0)) {
            this->setProbability(1.0);
            mExists = true;
            return;
        }
    }
    int period = game.getBonusAppearanceIntervalTicks();
    int tick = world.getTickIndex();
    int count = tick / period;
    int rest = tick % period;
    auto temp = (rest - period / 2.0) / period;
    if (temp > 0) {
        mExists = true;
    }
    if (mPosition.inCircle({self}, self.getVisionRange()) && temp < 0) {
        this->setProbability(0.0);
        mExists = false;
    } else if (mExists && (temp >= 0 || count > 0)) {
        this->setProbability(temp * temp);
    } else {
        this->setProbability(0.0);
        mExists = false;
    }
}
