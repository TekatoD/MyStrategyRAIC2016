#include <iomanip>
#include "MyStrategy.h"
#include "MapGraph.h"
#include "WorldFilter.h"
#include "ObstaclesGridMaker.h"
#include "Log.h"
#include "Walker.h"
#include "AStarPathFinder.h"
#include "BonusExistsSituation.h"
#include "Weaponry.h"
#include "BerserkBehavior.h"
#include "Clusterer.h"


MyStrategy::MyStrategy() : mInitialized(false) {
#ifdef __DEBUG__
    Log::setGlobalLevel(DEBUG);
#endif
}

void MyStrategy::move(const model::Wizard& self,
                      const model::World& world,
                      const model::Game& game,
                      model::Move& move) {
    Log(INFO) << "#Tick" << world.getTickIndex();
    auto startTimePoint = std::chrono::steady_clock::now();
    // Create new state object
    auto state = share<State>(std::ref(self), std::ref(world), std::ref(game), std::ref(move));
    // Init controller
    if (!mInitialized) this->initialize(state);
    // Update world
    Log(DEBUG) << "Updating controller...";
    mGameController.update(state);
    // Move!
    Log(DEBUG) << "Performing turn...";
    mGameController.turn();
    auto endTimePoint = std::chrono::steady_clock::now();
    Log(DEBUG) << "Tick" << world.getTickIndex() << "completed for"
               << std::fixed << std::setprecision(9)
               << 1.0 / (endTimePoint - startTimePoint).count();
}


void MyStrategy::initialize(Ptr<State> state) {
    Log(DEBUG) << "Initialization started";
    // Create aliases
    auto& self = state->self;
    auto& game = state->game;
    auto& move = state->move;
    auto& world = state->world;
    // Key positions
    const auto posBonusBot =      Point(1200.0, 1200.0);
    const auto posBonusTop =      Point(2800.0, 2800.0);
    const auto posBaseTop =       Point{3600.0, 400.0};
    const auto posBaseBot =       Point{400.0, 3600.0};
    const auto posTowerLeft1 =    Point{50.0, 2693.2577778083373};
    const auto posTowerLeft2 =    Point{350.0, 1656.7486446626867};
    const auto posTowerMidBot2 =  Point{1929.2893218813454, 2400.0};
    const auto posTowerMidBot1 =  Point{902.6130586671778, 2768.0976194514765};
    const auto posTowerBot1 =     Point{1370.6603203516029, 3650.0};
    const auto posTowerBot2 =     Point{2312.1259974228437, 3950.0};
    const auto posTowerRight1 =   Point{3950.0, 1306.7422221916627};
    const auto posTowerRight2 =   Point{3650.0, 2343.2513553373133};
    const auto posTowerMidTop1 =  Point{3097.386941332822, 1231.9023805485235};
    const auto posTowerMidTop2 =  Point{2070.710678118655, 1600.0};
    const auto posTowerTop1 =     Point{2629.339679648397, 350.0};
    const auto posTowerTop2 =     Point{1687.8740025771563, 50.0};
    // Define waypoints
    const auto wpCornerTop = Point(400, 400);
    const auto wpCornerBot = Point(3600, 3600);
    const auto wpBonusBot = Point(1200.0, 1200.0);
    const auto wpBonusTop = Point(2800.0, 2800.0);
    const auto wpBaseTop = Point(3600.0 - (74.25 + 70.71), 400.0 + (74.25 + 70.71));
    const auto wpBaseBot = Point(400.0 - (74.25 + 70.71), 3600.0 - (74.25 + 70.71));
    const auto wpTowerLeft1 = Point(50.0 + (52 + 50), 2693.2577778083373);
    const auto wpTowerLeft2 = Point(350.0 - (52 + 50), 1656.7486446626867);
    const auto wpTowerMidBot2 = Point(1929.2893218813454 - (35.35 + 74.25), 2400.0 - (35.35 + 74.25));
    const auto wpTowerMidBot1 = Point(902.6130586671778 + (35.35 + 74.25), 2768.0976194514765 + (35.35 + 74.25));
    const auto wpTowerBot1 = Point(1370.6603203516029, 3650.0 + (52 + 50));
    const auto wpTowerBot2 = Point(2312.1259974228437, 3950.0 - (52 + 50));
    const auto wpTowerRight1 = Point(3950.0 - (52 + 50), 1306.7422221916627);
    const auto wpTowerRight2 = Point(3650.0 + (52 + 50), 2343.2513553373133);
    const auto wpTowerMidTop1 = Point(3097.386941332822 - (35.35 + 74.25), 1231.9023805485235 - (35.35 + 74.25));
    const auto wpTowerMidTop2 = Point(2070.710678118655 + (35.35 + 74.25), 1600.0 + (35.35 + 74.25));
    const auto wpTowerTop1 = Point(2629.339679648397, 350.0 - (52 + 50));
    const auto wpTowerTop2 = Point(1687.8740025771563, 50.0 + (52 + 50));
    const auto wpCenter = Point(2000.0, 2000.0);
    // Initialize map
    Ptr<MapGraph> map = share<MapGraph>();
    map->addEdge(wpBaseBot, wpTowerLeft1);
    map->addEdge(wpTowerLeft1, wpTowerLeft2);
    map->addEdge(wpTowerLeft2, wpCornerTop);
    map->addEdge(wpBaseBot, wpTowerMidBot1);
    map->addEdge(wpTowerMidBot1, wpTowerMidBot2);
    map->addEdge(wpTowerMidBot2, wpCenter);
    map->addEdge(wpBaseBot, wpTowerBot1);
    map->addEdge(wpTowerBot1, wpTowerBot2);
    map->addEdge(wpTowerBot2, wpCornerBot);
    map->addEdge(wpBaseTop, wpTowerTop1);
    map->addEdge(wpTowerTop1, wpTowerTop2);
    map->addEdge(wpTowerTop2, wpCornerTop);
    map->addEdge(wpBaseTop, wpTowerMidTop1);
    map->addEdge(wpTowerMidTop1, wpTowerMidTop2);
    map->addEdge(wpTowerMidTop2, wpCenter);
    map->addEdge(wpBaseTop, wpTowerRight1);
    map->addEdge(wpTowerRight1, wpTowerRight2);
    map->addEdge(wpTowerRight2, wpCornerBot);
    map->addEdge(wpCornerBot, wpBonusBot);
    map->addEdge(wpBonusBot, wpCenter);
    map->addEdge(wpCornerTop, wpBonusBot);
    map->addEdge(wpBonusTop, wpCenter);
    // Define constants
    const double wizardSize = game.getWizardRadius();
    const size_t sectorSize = 15;
    const double filterRadius = std::sqrt(sectorSize * sectorSize * wizardSize * wizardSize * 2);
    // Initialize mechanisms
    auto weaponry = share<Weaponry>();
    auto worldFilter = share<WorldFilter>(filterRadius);
    auto obstaclesGridMaker = share<ObstaclesGridMaker<sectorSize>>(worldFilter);
    auto walker = share<Walker<sectorSize>>(
            map,
            obstaclesGridMaker,
            share<AStarPathFinder>(),
            share<AStarPathFinder>()
    );
    auto clusterer = share<Clusterer>(wizardSize * 8, true, 4 * wizardSize);

    mGameController.addMechanism(weaponry);
    mGameController.addMechanism(worldFilter);
    mGameController.addMechanism(obstaclesGridMaker);
    mGameController.addMechanism(walker);
    mGameController.addMechanism(clusterer);

    auto situationTopBonusExists = share<BonusExistsSituation>("top_bonus_exists", posBonusTop);
    auto situationBotBonusExists = share<BonusExistsSituation>("bot_bonus_exists", posBonusBot);

    auto behaviorGoToTopBonus = share<BerserkBehavior<sectorSize>>("go_to_top_bonus", posBonusTop, walker,
                                                       weaponry, sectorSize, 3, false, true, true);
    auto behaviorGoToBotBonus = share<BerserkBehavior<sectorSize>>("go_to_bot_bonus", posBonusTop, walker,
                                                       weaponry, sectorSize, 3, false, true, true);

    mGameController.addRelationship(share<Relationship>(std::string("top_bonus"), situationTopBonusExists, behaviorGoToTopBonus));
    mGameController.addRelationship(share<Relationship>(std::string("bot_bonus"), situationBotBonusExists, behaviorGoToBotBonus));

    // DO NOT EDIT WHAT'S BELOW!
    mInitialized = true;
    Log(DEBUG) << "Initialization finished";
}
