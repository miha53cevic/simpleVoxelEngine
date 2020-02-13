#pragma once
#include <deque>
#include <memory>
#include "../state.h"

typedef std::unique_ptr<State> StateRef;

class StateManager
{
public:
    StateManager();

    void addState(StateRef newState, bool isReplacing);
    void removeState();

    StateRef& CurrentState();

    bool isEmpty();

private:
    std::deque<StateRef> m_states;
};