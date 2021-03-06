#include "Runner.h"

#include <vector>
#include <stdlib.h>
#include <thread>

#include "src/MyStrategy.h"
#include "src/genetic_reactor.h"
#include "src/GridMap.h"
#include <iostream>
#include <time.h>
#include "src/parse_best.h"

using namespace model;
using namespace std;

int main(int argc, char* argv[]) {
    genetic_reactor genetic_reactor1(1000, 10, 1000, 0.2);
    for (size_t i = 0; i < genetic_reactor1.get_generations_count(); ++i) {
        for(size_t j = 0; j < genetic_reactor1.get_generation_size(); ++j) {
            system("cd /home/tekatod/develop/cppstrategy2016/local-runner && bash local-runner-console.sh &");
            std::this_thread::sleep_for(3s);
            std::cout << "hui" << std::endl;
            if (argc == 4) {
                Runner runner(argv[1], argv[2], argv[3]);
                runner.run();
            } else {
                Runner runner("127.0.0.1", "31001", "0000000000000000");
                runner.run();
            }
            genetic_reactor1.set_current_score(read_score("/home/tekatod/develop/cppstrategy2016/local-runner/result.txt"));
            if(j != genetic_reactor1.get_generation_size() - 1) {
                genetic_reactor1.get_next_dna();
            }
        }
        genetic_reactor1.switch_to_new_generation();
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
