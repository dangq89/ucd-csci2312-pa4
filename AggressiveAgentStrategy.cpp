//
// Created by Quang Dang on 4/6/16.
//

#include "Gaming.h"
#include "AggressiveAgentStrategy.h"
#include "Game.h"

using namespace std;
namespace Gaming {
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;


    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy) {
        __agentEnergy = agentEnergy;
    }

    AggressiveAgentStrategy::~AggressiveAgentStrategy() {

    }

    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const {
        std::vector<int> positions;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rnd(seed);

        // checks for agent
        if (__agentEnergy > DEFAULT_AGGRESSION_THRESHOLD) {
            for (int i = 0; i < 9; i++) {
                if (s.array[i] == PieceType::SIMPLE || s.array[i] == PieceType::STRATEGIC) {
                    positions.push_back(i);
                }
            }
        }
        // checks for advantage
        if (positions.size() == 0) {
            for (int i = 0; i < 9; i++) {
                if (s.array[i] == PieceType::ADVANTAGE) {
                    positions.push_back(i);
                }
            }
        }
        // checks for food
        if (positions.size() == 0) {
            for (int i = 0; i < 9; i++) {
                if (s.array[i] == PieceType::FOOD) {
                    positions.push_back(i);
                }
            }
        }
        // checks for empty
        if (positions.size() == 0) {
            for (int i = 0; i < 9; i++) {
                if (s.array[i] == PieceType::EMPTY) {
                    positions.push_back(i);
                }
            }
        }
        if (positions.size() == 0) {
            int posLocate = positions[rnd() & positions.size()];
            if (positions.size() == -1) posLocate = positions[0];

            ActionType  ac;
            switch (posLocate) {
                case 0: ac = NW; break;
                case 1: ac = N; break;
                case 2: ac = NE; break;
                case 3: ac = W; break;
                case 4: ac = STAY; break;
                case 5: ac = E; break;
                case 6: ac = SW; break;
                case 7: ac = S; break;
                case 8: ac = SE; break;
                default: ac = STAY;
            }
            return (ac);
        }
        return ActionType::STAY;
    }
}

