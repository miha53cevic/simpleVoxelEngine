#include "stateManager.h"

StateManager::StateManager()
{
}

void StateManager::addState(StateRef newState, bool isReplacing)
{
    if (!isEmpty())
    {
        if (isReplacing) m_states.pop_front();
        else             CurrentState()->pause();
    }

    m_states.push_front(std::move(newState));
    CurrentState()->init();
}

void StateManager::removeState()
{
    m_states.pop_front();

    if (!isEmpty())
    {
        CurrentState()->resume();
    }
}

StateRef & StateManager::CurrentState()
{
    return m_states.front();
}

bool StateManager::isEmpty()
{
    return m_states.size() == 0;
}


