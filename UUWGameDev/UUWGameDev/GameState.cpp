#include "GameState.h"

GameState::GameState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    , m_oWorld(*pContext.m_oWindow, *pContext.m_oFonts)
    , m_oPlayer(*pContext.m_oPlayer)
{
    m_oPlayer.SetMissionStatus(Player::MissionRunning);
}

void GameState::draw()
{
    m_oWorld.Draw();
}

bool GameState::update(sf::Time pDT)
{
    m_oWorld.Update(pDT);

   /* CommandQueue& commands = m_oWorld.GetCommandQueue();
    m_oPlayer.HandleRealtimeInput(commands);*/
    if (!m_oWorld.HasAlivePlayer())
    {
        m_oPlayer.SetMissionStatus(Player::MissionFailure);
        RequestStackPush(States::GameOver);
    }
    else if (m_oWorld.HasPlayerReachedEnd())
    {
        m_oPlayer.SetMissionStatus(Player::MissionSuccess);
        RequestStackPush(States::GameOver);
    }

    CommandQueue& oCommands = m_oWorld.GetCommandQueue();
    m_oPlayer.HandleRealtimeInput(oCommands);


    return true;
}

bool GameState::handleEvent(const sf::Event& pEvent)
{
    // Game input handling
    CommandQueue& oCommands = m_oWorld.GetCommandQueue();
    m_oPlayer.HandleEvent(pEvent, oCommands);

    // Escape pressed, trigger the pause screen
    if (pEvent.type == sf::Event::KeyPressed && pEvent.key.code == sf::Keyboard::Escape)
        RequestStackPush(States::Pause);

    return true;
}
