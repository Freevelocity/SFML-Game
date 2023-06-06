#include "PauseState.h"
#include "Button.h"
#include "Utility.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    , m_oBackgroundSprite()
    , m_oPausedText()
    //, m_oInstructionText()
	, m_oGUIContainer()
{
	sf::Font& oFont = pContext.m_oFonts->get(Fonts::Main);
	sf::Vector2f oViewSize = pContext.m_oWindow->getView().getSize();

	m_oPausedText.setFont(oFont);
	m_oPausedText.setString("Game Paused");
	m_oPausedText.setCharacterSize(70);
	CenterOrigin(m_oPausedText);
	m_oPausedText.setPosition(0.5f * oViewSize.x, 0.4f * oViewSize.y);

	/*m_oInstructionText.setFont(font);
	m_oInstructionText.setString("(Press Backspace to return to the main menu)");
	CenterOrigin(m_oInstructionText);
	m_oInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);*/

	auto oReturnButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oReturnButton->setPosition(0.5f * oViewSize.x - 100, 0.4f * oViewSize.y + 75);
	oReturnButton->SetText("Return");
	oReturnButton->SetCallback([this]()
		{
			RequestStackPop();
		});

	auto oBackToMenuButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oBackToMenuButton->setPosition(0.5f * oViewSize.x - 100, 0.4f * oViewSize.y + 125);
	oBackToMenuButton->SetText("Back to menu");
	oBackToMenuButton->SetCallback([this]()
		{
			RequestStateClear();
			RequestStackPush(States::Menu);
		});

	m_oGUIContainer.Pack(oReturnButton);
	m_oGUIContainer.Pack(oBackToMenuButton);
}

void PauseState::draw()
{
	sf::RenderWindow& oWindow = *GetContext().m_oWindow;
	oWindow.setView(oWindow.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(oWindow.getView().getSize());

	oWindow.draw(backgroundShape);
	oWindow.draw(m_oPausedText);
	//oWindow.draw(m_oInstructionText);
	oWindow.draw(m_oGUIContainer);
}

bool PauseState::update(sf::Time pDT)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& pEvent)
{
	//if (pEvent.type != sf::Event::KeyPressed)
	//	return false;

	//if (pEvent.key.code == sf::Keyboard::Escape)
	//{
	//	// Escape pressed, remove itself to return to the game
	//	RequestStackPop();
	//}

	//if (pEvent.key.code == sf::Keyboard::BackSpace)
	//{
	//	// Escape pressed, remove itself to return to the game
	//	RequestStateClear();
	//	RequestStackPush(States::Menu);
	//}
	m_oGUIContainer.handleEvent(pEvent);
    return false;
}
