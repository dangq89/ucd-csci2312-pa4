//
// Created by Quang Dang on 4/6/16.
//

#include "Food.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace Gaming {
    const char Food::FOOD_ID = 'F';

    Food::Food(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {
    }

    void Food::print(std::ostream &os) const {
        std::string str;
        str = std::to_string(__id);
        std::stringstream ss;
        ss << Food::FOOD_ID;
        ss << str;
        std::getline(ss, str);
        for (int i = 0; i < str.length(); i++) {
            os << str[i];
        }
    }

    Food::~Food() {
    }
}