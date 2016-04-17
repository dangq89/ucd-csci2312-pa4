//
// Created by Quang Dang on 4/6/16.
//
#include <iostream>
#include <string>

#include "Resource.h"
#include "Agent.h"
#include "Piece.h"

using namespace std;

namespace Gaming {
    unsigned int Piece::__idGen = 1000;

    Piece::~Piece() {
    }

    Piece::Piece(const Game &g, const Position &p) : __game(g), __position(p) {
        __id = __idGen++;
        __finished = false;
        __turned = false;
    }

    std::ostream &operator<<(ostream &os, const Piece &piece) {
        piece.print(os);
        return os;
    }
}
