//
// Created by Quang Dang on 4/6/16.
//

#include <iostream>
#include <sstream>

#include "Advantage.h"

using namespace std; 

namespace Gaming {
    
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;
    const char Advantage::ADVANTAGE_ID = 'D'; 
    
    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {
        
    }

    void Advantage::print(std::ostream &os) const {
        std::string str;

        str = std::to_string(__id);

        std::stringstream ss;
        ss << Advantage::ADVANTAGE_ID;
        ss << str;

        std::getline(ss, str);

        for (int i = 0; i < str.length(); i++) {
            os << str[i];
        }
    }
    
    Advantage::~Advantage() {
        
    }

    double Advantage::getCapacity() const {
        return __capacity * ADVANTAGE_MULT_FACTOR; 
    }

    double Advantage::consume() {
        double ret = getCapacity();
        __capacity = -1;
        finish();

        return ret;

    }
}