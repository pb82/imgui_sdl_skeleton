#ifndef SKELETON_STM_H
#define SKELETON_STM_H

#include <vector>
#include <unordered_map>

namespace statemachine {

    enum State {
        Menu,
        Editor
    };

    class Stm {
    public:
        Stm(State initial);

        void addPath(State from, State to);

        bool transition(State to);

        State current() const;

    private:

        std::unordered_map<State, std::vector<State>> possiblePaths;

        State currentState;
    };
}

#endif //SKELETON_STM_H
