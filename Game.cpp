//
// Created by Quang Dang on 4/6/16.
// Game.cpp

#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

#include <sstream>
#include <fstream>
#include <set>
#include <iomanip>

using namespace std;
namespace Gaming {

    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;

    PositionRandomizer Game::__posRandomizer = PositionRandomizer();

    Game::Game() {
        __width = 3;
        __height = 3;
        __status = NOT_STARTED;
        __verbose = false;
        __round = 1;

        for (unsigned i = 0; i < (__width * __height); i++) {
            __grid.push_back(nullptr);
        }

    }

    Game::Game(unsigned width, unsigned height, bool manual) {
        __width = width;
        __height = height;

        if (width < MIN_WIDTH || height < MIN_HEIGHT)
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);

        for (int i = 0; i < (__width * __height); i++) {
            __grid.push_back(nullptr);
        }

        if (!manual)
            populate();
    }

    Game::Game(const Game &another) {
    }

    Game::~Game() {
    // destructor
    }

    void Gaming::Game::populate() {

        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);

        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

        // given by PA4
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;

        while (numStrategic > 0) {      // populates strategic agents
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }

        while (numSimple > 0) {      // populates simple agents
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }

        while (numFoods > 0) {      // populates foods
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }

        while (numAdvantages > 0) {     // used resources
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
            }
        }
    }

    // getters
    unsigned int Game::getNumPieces() const {
        unsigned int numPieces = 0;
        for (auto it = __grid.begin(); it != __grid.end(); it++) {
            if (*it != nullptr) {
                numPieces++;
            }
        }
        return numPieces;
    }

    unsigned int Game::getNumAgents() const {
        unsigned int numAgents = 0;
        for (auto it = __grid.begin(); it != __grid.end(); it++) {
            Agent *simple = dynamic_cast<Agent *>(*it);
            if (simple) numAgents++;
        }
        return numAgents;
    }

    unsigned int Game::getNumSimple() const {
        unsigned int NumAgents = 0;
        for (auto it = __grid.begin(); it != __grid.end(); it++) {
            Simple *simple = dynamic_cast<Simple *>(*it);
            if (simple) NumAgents++;
        }
        return NumAgents;
    }

    unsigned int Game::getNumStrategic() const {
        unsigned int NumAgents = 0;
        for (auto it = __grid.begin(); it != __grid.end(); it++) {
            Strategic *simple = dynamic_cast<Strategic *>(*it);
            if (simple) NumAgents++;
        }
        return NumAgents;
    }

    unsigned int Game::getNumResources() const {
        unsigned int NumAgents = 0;
        for (auto it = __grid.begin(); it != __grid.end(); it++) {
            Resource *simple = dynamic_cast<Resource *>(*it);
            if (simple) NumAgents++;
        }
        return NumAgents;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[y + (x * __width)] == nullptr)
            throw PositionEmptyEx(x, y);

        return __grid[y + (x * __width)];
    }

    // grid population methods
    void Game::addSimple(const Position &position) {
        int locate = (position.x * __width) + position.y;
        if (position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[locate])
            throw PositionNonemptyEx(position.x, position.y);

        __grid[locate] = new Simple(*this, position, STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(const Position &position, double energy) {
        int locate = (position.x * __width) + position.y;
        if (position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[locate])
            throw PositionNonemptyEx(position.x, position.y);

        __grid[locate] = new Simple(*this, position, energy);
    }

    void Game::addSimple(unsigned x, unsigned y) {
        int locate = (x * __width) + y;
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[locate])
            throw PositionNonemptyEx(x, y);

        __grid[locate] = new Simple(*this, Position(x, y), STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(unsigned x, unsigned y, double energy) {
        int locate = (x * __width) + y;
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[locate])
            throw PositionNonemptyEx(x, y);

        __grid[locate] = new Simple(*this, Position(x, y), energy);
    }

    void Game::addStrategic(const Position &position, Strategy *s) {
        int locate = (position.x * __width) + position.y;
        if (position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[locate])
            throw PositionNonemptyEx(position.x, position.y);

        __grid[locate] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);

    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s) {
        int locate = (x * __width) + y;
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[locate])
            throw PositionNonemptyEx(x, y);

        __grid[locate] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
    }

    void Game::addFood(const Position &position) {
        int locate = (position.x * __width) + position.y;
        if (position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[locate])
            throw PositionNonemptyEx(position.x, position.y);

        __grid[locate] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addFood(unsigned x, unsigned y) {
        int locate = (x * __width) + y;
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[locate])
            throw PositionNonemptyEx(x, y);

        __grid[locate] = new Food(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(const Position &position) {
        int locate = (position.x * __width) + position.y;
        if (position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);

        if (__grid[locate])
            throw PositionNonemptyEx(position.x, position.y);

        __grid[locate] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(unsigned x, unsigned y) {
        int locate = (x * __width) + y;
        if (y >= __width || x >= __height)
            throw OutOfBoundsEx(__width, __height, x, y);

        if (__grid[locate])
            throw PositionNonemptyEx(x, y);

        __grid[locate] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    const Surroundings Game::getSurroundings(const Position &pos) const {
        Surroundings sur;
        for (int i = 0; i < 9; ++i) {
            sur.array[i] = EMPTY;
        }
        for (int row = -1; row <= 1; ++row) {
            for (int col = -1; col <= 1; ++col) {
                if (pos.x + row >= 0 && pos.x + row < __height && pos.y + col >= 0 && pos.y + col < __width) {
                    unsigned int index = pos.y + col + ((pos.x + row) * __width);
                    if (__grid[index])
                        sur.array[col + 1 + ((row + 1) * 3)] = __grid[index]->getType();
                }
                else {
                    sur.array[col + 1 + ((row + 1) * 3)] = INACCESSIBLE;
                }
            }
        }
        sur.array[4] = SELF;
        return sur;
    }

    // gameplay methods
    const ActionType Game::reachSurroundings(const Position &from, const Position &to) {
        int x_diff;
        int y_diff;
        x_diff = to.x - from.x;
        y_diff = to.y - from.y;
        x_diff++;
        y_diff++;

        int locate = (y_diff + (x_diff * 3));
        switch (locate) {
            case 0: return NW;
            case 1: return N;
            case 2: return NE;
            case 3: return W;
            case 4: return STAY;
            case 5: return E;
            case 6: return SW;
            case 7: return S;
            case 8: return SE;
            default: return STAY;
        }
    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const {
        int x_diff;
        int y_diff;
        x_diff = pos.x;
        y_diff = pos.y;

        switch (ac) {
            case E:
                y_diff++;
                break;
            case NE:
                y_diff++;
                x_diff--;
                break;
            case N:
                x_diff--;
                break;
            case NW:
                y_diff--;
                x_diff--;
                break;
            case W:
                y_diff--;
                break;
            case SW:
                y_diff--;
                x_diff++;
                break;
            case S:
                x_diff++;
                break;
            case SE:
                x_diff++;
                y_diff++;
                break;
            default:
                break;
        }
        Position p((unsigned) x_diff, (unsigned) y_diff);
        if (p.x < __height && p.y < __width)
            return true;
        return false;
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const {
        if (isLegal(ac, pos)) {
            int x_diff;
            int y_diff;
            x_diff = pos.x;
            y_diff = pos.y;

            switch (ac) {
                case E: y_diff++; break;
                case NE: y_diff++; x_diff--; break;
                case N: x_diff--; break;
                case NW: y_diff--; x_diff--; break;
                case W: y_diff--; break;
                case SW: y_diff--; x_diff++; break;
                case S: x_diff++; break;
                case SE: x_diff++; y_diff++; break;
                default: break;
            }
            Position p((unsigned)x_diff, (unsigned)y_diff);
            return p;
        }
        return pos;
    }

    void Game::round() {   // plays a single round
        std::set<Piece*> pieces;

        for (auto i = __grid.begin(); i != __grid.end(); i++) {
            if (*i) {
                pieces.insert(pieces.end(), *i);
                (*i)->setTurned(false);
            }
        }
        for (auto i = pieces.begin(); i != pieces.end(); i++) { // taking turns
            if (!(*i)->getTurned()) {
                (*i)->setTurned(true);
                (*i)->age();
                ActionType ac = (*i)->takeTurn(getSurroundings((*i)->getPosition()));
                Position pos0 = (*i)->getPosition();
                Position pos1 = move(pos0, ac);
                if (pos0.x != pos1.x || pos0.y != pos1.y) {
                    Piece *p = __grid[pos1.y + (pos1.x * __width)];
                    if (p) {
                        if ((*i)->getPosition().x != pos0.x || (*i)->getPosition().y != pos0.y) {
                            __grid[pos1.y + (pos1.x * __width)] = (*i);
                            __grid[pos0.y + (pos0.x * __width)] = p;
                        }
                    } else {
                        (*i)->setPosition(pos1);
                        __grid[pos1.y + (pos1.x * __width)] = (*i);
                        __grid[pos0.y + (pos0.x * __width)] = nullptr;
                    }
                }
            }
        }
        for (unsigned int i = 0; i < __grid.size(); ++i) {
            if (__grid[i] && !(__grid[i]->isViable())) {
                delete __grid[i];
                __grid[i] = nullptr;
            }
        }
        if (getNumResources() <= 0) { // game over?
            __status = Status::OVER;
        }
        __round++;
    }

    void Game::play(bool verbose) {    // play until finished
        __verbose = verbose;
        __status = PLAYING;
        std::cout << *this;

        while (__status != OVER) {
            round();
            if (verbose) std::cout << *this;
        }
        if (!verbose) std::cout << *this;
    }

    std::ostream &operator<<(std::ostream &os, const Game &game) {
        os << "Round " << game.__round << std::endl;
        int column = 0;

        for (auto it = game.__grid.begin(); it != game.__grid.end(); ++it) {
            if (*it == nullptr) {
                os << "[" << std::setw(6) << "]";
            } else {
                std::stringstream ss;
                ss << "[" << **it;
                std::string str;
                std::getline(ss, str);
                os << str << "]";
            }
            if (++column == game.__width) {
                column = 0;
                os << std::endl;
            }
        }
        os << "Status: ";
        switch (game.getStatus()) {
            case Game::Status::NOT_STARTED:
                std::cout << "Not Started!" << std::endl; break;
            case Game::Status::PLAYING:
                std::cout << "Playing!" << std::endl; break;
            default:
                std::cout << "Over!" << std::endl; break;
        }
        return os;
    }
}

