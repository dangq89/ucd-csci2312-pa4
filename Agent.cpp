//
// Created by Quang Dang on 4/6/16.
//
#include <iostream> 

#include "Piece.h"
#include "Agent.h"
#include "Resource.h"

using namespace std; 

namespace Gaming {
    const double Agent::AGENT_FATIGUE_RATE = 0.3; 

    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g, p) {
        __energy = energy; 
    }
    
    Agent::~Agent() {
    }

    void Agent::age() {
        __energy -= AGENT_FATIGUE_RATE;
    }

    Piece &Agent::operator*(Piece &other) {
        Piece *p = &other;
        Resource *res = dynamic_cast<Resource*>(p);
        if (res) {
            interact(res);
        }
        Agent *agent = dynamic_cast<Agent*>(p);
        if (agent) {
            interact(agent);
        }
        if (!isFinished()) {
            Position newPos;
            newPos = other.getPosition();
            Position oldPos;
            oldPos = getPosition();
            setPosition(newPos);
            other.setPosition(oldPos);
        }
        return *this;
    }

    Piece &Agent::interact(Agent *agent) {
        if (this->__energy < agent->__energy) {
            this->finish();
            return *agent;
        }
        if (this->__energy > agent->__energy) {
            agent->finish();
            return *this;
        }
        this->finish();
        agent->finish();
        return *this;
    }

    Piece &Agent::interact(Resource *other) {
        __energy += other->consume();
        return *this; 
    }
}
