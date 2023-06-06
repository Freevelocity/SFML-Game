#include "StateStack.h"
#include "Foreach.h"

#include <cassert>

StateStack::StateStack(State::Context pContext)
    : m_oStack()
    , m_oPendingList()
    , m_oContext(pContext)
    , m_oFactories()
{
}

void StateStack::Update(sf::Time pDT)
{
    // Iterate from top to bottom, stop as soon as update() returns false
    for (auto itr = m_oStack.rbegin(); itr != m_oStack.rend(); ++itr)
    {
        if (!(*itr)->update(pDT))
            break;
    }

    ApplyPendingChanges();
}

void StateStack::Draw()
{
    // Draw all active states from bottom to top
    FOREACH(State::Ptr & oState, m_oStack)
        oState->draw();
}

void StateStack::HandleEvent(const sf::Event& pEvent)
{
    // Iterate from top to bottom, stop as soon as handleEvent() returns false
    for (auto itr = m_oStack.rbegin(); itr != m_oStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(pEvent))
            break;
    }

    ApplyPendingChanges();
}

void StateStack::PushState(States::ID pStateID)
{
    m_oPendingList.push_back(PendingChange(Push, pStateID));
}

void StateStack::PopState()
{
    m_oPendingList.push_back(PendingChange(Pop));
}

void StateStack::ClearStates()
{
    m_oPendingList.push_back(PendingChange(Clear));
}

bool StateStack::IsEmpty() const
{
    return m_oStack.empty();
}

State::Ptr StateStack::CreateState(States::ID pStateID)
{
    auto found = m_oFactories.find(pStateID);
    assert(found != m_oFactories.end());

    return found->second();
}

void StateStack::ApplyPendingChanges()
{
    FOREACH(PendingChange oChange, m_oPendingList)
    {
        switch (oChange.m_oAction)
        {
        case Push:
            m_oStack.push_back(CreateState(oChange.m_oStateID));
            break;

        case Pop:
            m_oStack.pop_back();
            break;

        case Clear:
            m_oStack.clear();
            break;
        }
    }

    m_oPendingList.clear();

}

 StateStack::PendingChange::PendingChange(Action pAction, States::ID pStateID)
    :m_oAction(pAction)
    ,m_oStateID(pStateID)
{
}
