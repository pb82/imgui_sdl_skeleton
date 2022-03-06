#include "Stm.h"

namespace statemachine {
    Stm::Stm(State initial) : currentState(initial) {
        possiblePaths.emplace(initial, std::vector<State>());
    }

    void Stm::addPath(State from, State to) {
        if (possiblePaths.find(from) == possiblePaths.end()) {
            possiblePaths.emplace(from, std::vector<State>());
        }
        possiblePaths.at(from).push_back(to);
    }

    bool Stm::transition(State to) {
        if (possiblePaths.at(currentState).empty()) {
            return false;
        }
        for (auto path: possiblePaths.at(to)) {
            if (path == to) {
                currentState = to;
                return true;
            }
        }
        return false;
    }

    State Stm::current() const {
        return this->currentState;
    }
}