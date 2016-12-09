#include "Runner.h"

#include <vector>
#include <thread>

#include "src/MyStrategy.h"
#include "src/GridMap.h"
#include <iostream>
#include <time.h>

using namespace model;
using namespace std;

int main(int argc, char* argv[]) {
    std::this_thread::sleep_for(3s);
//    GridMap<int, 64, 64> gridMap{Point{0, 0}, 10};
//    clock_t tStart = clock();
//    gridMap.drawFilledCircle(Point{300, 300}, 400, 4);
//    printf("Time taken: %.6fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
////    gridMap.drawCircle(Point{300, 300}, 137, 4);
////    gridMap.drawLine(Point{0, 0}, Point{100, 50}, 6);
//    std::cout << gridMap.getGrid();

    if (argc == 4) {
        Runner runner(argv[1], argv[2], argv[3]);
        runner.run();
    } else {
        Runner runner("127.0.0.1", "31001", "0000000000000000");
        runner.run();
    }

    return 0;
}

Runner::Runner(const char* host, const char* port, const char* token)
    : remoteProcessClient(host, atoi(port)), token(token) {
}

void Runner::run() {
    remoteProcessClient.writeTokenMessage(token);
    remoteProcessClient.writeProtocolVersionMessage();
    size_t teamSize = remoteProcessClient.readTeamSizeMessage();
    Game game = remoteProcessClient.readGameContextMessage();

    vector<Strategy*> strategies;

    for (size_t strategyIndex = 0; strategyIndex < teamSize; ++strategyIndex) {
        Strategy* strategy = new MyStrategy;
        strategies.push_back(strategy);
    }

    PlayerContext* playerContext;

    while ((playerContext = remoteProcessClient.readPlayerContextMessage()) != nullptr) {
        vector<Wizard> playerWizards = playerContext->getWizards();
        if (playerWizards.size() != teamSize) {
            break;
        }

        vector<Move> moves;

        for (size_t wizardIndex = 0; wizardIndex < teamSize; ++wizardIndex) {
            Wizard playerWizard = playerWizards[wizardIndex];

            Move move;
            strategies[wizardIndex]->move(playerWizard, playerContext->getWorld(), game, move);
            moves.push_back(move);
        }

        remoteProcessClient.writeMovesMessage(moves);

        delete playerContext;
    }

    for (size_t strategyIndex = 0; strategyIndex < teamSize; ++strategyIndex) {
        delete strategies[strategyIndex];
    }
}
