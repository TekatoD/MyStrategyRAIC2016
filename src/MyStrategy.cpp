#include <iomanip>
#include "MyStrategy.h"
#include "MapGraph.h"
#include "WorldFilter.h"
#include "ObstaclesGridMaker.h"
#include "Log.h"
#include "AStarPathFinder.h"
#include "BonusExistsSituation.h"
#include "BerserkTools.h"
#include "BerserkBehavior.h"
#include "Clusterer.h"
#include "HoldingPositionChecker.h"
#include "TakeBonusBehavior.h"


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
    mGameController.update(state);
    // Move!
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
    const double baseOffset = 240;
    const auto wpCornerTop = Point(400, 400);
    const auto wpCornerBot = Point(3600, 3600);
    const auto wpBonusBot = Point(1200.0, 1200.0);
    const auto wpBonusTop = Point(2800.0, 2800.0);
    const auto wpBaseTopTop = posBaseTop + Point{-baseOffset, -baseOffset};
    const auto wpBaseTopMid = posBaseTop + Point{-baseOffset, baseOffset};
    const auto wpBaseTopRight = posBaseTop + Point{-baseOffset, baseOffset};
    const auto wpBaseBotLeft = posBaseBot + Point{-baseOffset, -baseOffset};
    const auto wpBaseBotMid = posBaseBot + Point{baseOffset, -baseOffset};
    const auto wpBaseBotBot = posBaseBot + Point{baseOffset, baseOffset};
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
    std::vector<std::pair<const Point*, const Point*>> edges = {{
             {&wpBaseBotLeft,  &wpTowerLeft1},
             {&wpTowerLeft2,   &wpCornerTop},
             {&wpBaseBotMid,   &wpTowerMidBot1},
             {&wpTowerMidBot1, &wpTowerMidBot2},
             {&wpTowerMidBot2, &wpCenter},
             {&wpBaseBotBot,   &wpTowerBot1},
             {&wpTowerBot1,    &wpTowerBot2},
             {&wpTowerBot2,    &wpCornerBot},
             {&wpBaseTopTop,   &wpTowerTop1},
             {&wpTowerTop1,    &wpTowerTop2},
             {&wpTowerTop2,    &wpCornerTop},
             {&wpBaseTopMid,   &wpTowerMidTop1},
             {&wpTowerMidTop1, &wpTowerMidTop2},
             {&wpTowerMidTop2, &wpCenter},
             {&wpBaseTopRight, &wpTowerRight1},
             {&wpTowerRight1,  &wpTowerRight2},
             {&wpTowerRight2,  &wpCornerBot},
             {&wpCornerBot,    &wpBonusBot},
             {&wpBonusBot,     &wpCenter},
             {&wpCornerTop,    &wpBonusBot},
             {&wpBonusTop,     &wpCenter},
             {&wpBaseBotLeft,   &wpBaseBotMid},
             {&wpBaseBotBot,   &wpBaseBotMid}
// TODO: Rounded corners
//             {&wpBaseTopTop,   &wpBaseTopMid},
//             {&wpBaseTopRight, &wpBaseTopMid}
    }};

    const int tesselationLevel = 2;
    const double tesselationThreshold = 400.0;
    for (const auto& edge : edges) {
        if (edge.second->inCircle(*edge.second, tesselationThreshold)) {
            const Point diff = (*edge.second - *edge.first) / tesselationLevel;
            Point current = *edge.first;
            for (int i = 0; i < tesselationLevel - 1; ++i) {
                map->addEdge(current, current + diff);
                current += diff;
            }
            map->addEdge(current, *edge.second);
        } else {
            map->addEdge(*edge.first, *edge.second);
        }
    }

    // Define constants
    const double wizardSize = game.getWizardRadius();
    const size_t sectorSize = 10 * wizardSize;
    const double filterRadius = std::sqrt(sectorSize * sectorSize * 2);
    // Initialize mechanisms
    auto filter = share<WorldFilter>(filterRadius);
    auto bersertTools = share<BerserkTools>(filter);
    auto sensors = share<MagicSensors>(filter, 64, wizardSize * 1.5);
    auto finder = share<AStarPathFinder>(map);
    auto holdingChecker = share<HoldingPositionChecker>(32, 5.0);
    auto clusterer = share<Clusterer>(wizardSize * 8, true, wizardSize * 3);

    mGameController.addMechanism(bersertTools);
    mGameController.addMechanism(filter);
    mGameController.addMechanism(sensors);
    mGameController.addMechanism(holdingChecker);
    mGameController.addMechanism(clusterer);

    auto situationTopBonusExists = share<BonusExistsSituation>("top_bonus_exists", posBonusTop);
    auto situationBotBonusExists = share<BonusExistsSituation>("bot_bonus_exists", posBonusBot);

    auto behaviorGoToTopBonus = share<BerserkBehavior>("go_to_top_bonus", posBonusTop, finder, sensors, bersertTools,
                                                       sectorSize, 3, false, true);
    auto behaviorGoToBotBonus = share<BerserkBehavior>("go_to_bot_bonus", posBonusBot, finder, sensors, bersertTools,
                                                       sectorSize, 3, false, true);
    auto behaviorBonusGetter = share<TakeBonusBehavior>("get_bonus", finder, sensors, 7);

    mGameController.addRelationship(share<Relationship>("go_to_top_bonus", situationTopBonusExists, behaviorGoToTopBonus));
    mGameController.addRelationship(share<Relationship>("go_to_bot_bonus", situationBotBonusExists, behaviorGoToBotBonus));
    mGameController.addRelationship(share<Relationship>("get_top_bonus", situationBotBonusExists, behaviorBonusGetter));
    mGameController.addRelationship(share<Relationship>("get_bot_bonus", situationTopBonusExists, behaviorBonusGetter));

    // DO NOT EDIT WHAT'S BELOW!
    mInitialized = true;
    Log(DEBUG) << "Initialization finished";
}
