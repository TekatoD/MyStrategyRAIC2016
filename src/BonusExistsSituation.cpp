/**
 * @author arssivka
 * @date 12/4/16
 */

#include "BonusExistsSituation.h"
#include "Log.h"


BonusExistsSituation::BonusExistsSituation(const std::string& name, const Point& position)
        : Situation(name), mPosition(position) {}


void BonusExistsSituation::update(Ptr<State> state) {
    auto& world = state->world;
    auto& game = state->game;
    for (auto bonus : world.getBonuses()) {
        Log(DEBUG) << Point{bonus};
        if (mPosition.inCircle({bonus}, 30.0)) {
            this->setProbability(1.0);
            return;
        }
    }
    double probability = 0.0;
    int period = game.getBonusAppearanceIntervalTicks();
    int tick = world.getTickIndex();
    int count = tick / period;
    int rest = tick % period;
    auto temp = (rest - period / 2.0) / period;
    probability = (count > 0 || temp >= 0) ? temp * temp : 0.0;
    this->setProbability(probability);
}
