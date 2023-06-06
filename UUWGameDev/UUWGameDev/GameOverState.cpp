#include "GameOverState.h"
#include "Utility.h"
#include "Player.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


GameOverState::GameOverState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    , m_oGameOverText()
    , m_oElapsedTime(sf::Time::Zero)
{
	sf::Font& oFont = pContext.m_oFonts->get(Fonts::Main);
	sf::Vector2f oWindowSize(pContext.m_oWindow->getSize());

	m_oGameOverText.setFont(oFont);
	if (pContext.m_oPlayer->GetMissionStatus() == Player::MissionFailure)
		m_oGameOverText.setString("Mission failed!");
	else
		m_oGameOverText.setString("Mission successful!");

	m_oGameOverText.setCharacterSize(70);
	CenterOrigin(m_oGameOverText);
	m_oGameOverText.setPosition(0.5f * oWindowSize.x, 0.4f * oWindowSize.y);

}

void GameOverState::draw()
{
	sf::RenderWindow& oWindow = *GetContext().m_oWindow;
	oWindow.setView(oWindow.getDefaultView());

	sf::RectangleShape oBackgroundShape;
	oBackgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	oBackgroundShape.setSize(oWindow.getView().getSize());

	oWindow.draw(oBackgroundShape);
	oWindow.draw(m_oGameOverText);
}

bool GameOverState::update(sf::Time pDT)
{

	m_oElapsedTime += pDT;
	if (m_oElapsedTime > sf::seconds(3))
	{
		RequestStateClear();
		RequestStackPush(States::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event& pEvent)
{
    return false;
}
