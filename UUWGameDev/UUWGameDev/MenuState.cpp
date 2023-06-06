#include "MenuState.h"
#include "Button.h"
#include "Utility.h"
//#include "Foreach.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    //, m_oOptions()
    //, m_oOptionIndex(0)
	, m_oGUIContainer()

{
	sf::Texture& texture = pContext.m_oTextures->get(Textures::TitleScreen);
	//sf::Font& font = pContext.m_oFonts->get(Fonts::Main);

	m_oBackgroundSprite.setTexture(texture);

	// A simple menu demonstration
	//sf::Text playOption;
	//playOption.setFont(font);
	//playOption.setString("Play");
	//CenterOrigin(playOption);
	//playOption.setPosition(pContext.m_oWindow->getView().getSize() / 2.f);
	////m_oOptions.push_back(playOption);

	//sf::Text exitOption;
	//exitOption.setFont(font);
	//exitOption.setString("Exit");
	//CenterOrigin(exitOption);
	//exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	//m_oOptions.push_back(exitOption);

	//UpdateOptionText();
	auto oPlayButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oPlayButton->setPosition(100, 300);
	oPlayButton->SetText("Play");
	oPlayButton->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush(States::Game);
		});

	auto oSettingsButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oSettingsButton->setPosition(100, 350);
	oSettingsButton->SetText("Settings");
	oSettingsButton->SetCallback([this]()
		{
			RequestStackPush(States::Settings);
		});

	auto oExitButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oExitButton->setPosition(100, 400);
	oExitButton->SetText("Exit");
	oExitButton->SetCallback([this]()
		{
			RequestStackPop();
		});

	m_oGUIContainer.Pack(oPlayButton);
	m_oGUIContainer.Pack(oSettingsButton);
	m_oGUIContainer.Pack(oExitButton);

}

void MenuState::draw()
{
	sf::RenderWindow& oWindow = *GetContext().m_oWindow;

	oWindow.setView(oWindow.getDefaultView());
	oWindow.draw(m_oBackgroundSprite);
	oWindow.draw(m_oGUIContainer);

	/*FOREACH(const sf::Text & oText, m_oOptions)
		oWindow.draw(oText);*/
}

bool MenuState::update(sf::Time pDT)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& pEvent)
{
	m_oGUIContainer.handleEvent(pEvent);
	// The demonstration menu logic
	//if (pEvent.type != sf::Event::KeyPressed)
	//	return false;

	//if (pEvent.key.code == sf::Keyboard::Return)
	//{
	//	if (m_oOptionIndex == Play)
	//	{
	//		RequestStackPop();
	//		RequestStackPush(States::Game);
	//	}
	//	else if (m_oOptionIndex == Exit)
	//	{
	//		// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
	//		RequestStackPop();
	//	}
	//}

	//else if (pEvent.key.code == sf::Keyboard::Up)
	//{
	//	// Decrement and wrap-around
	//	if (m_oOptionIndex > 0)
	//		m_oOptionIndex--;
	//	else
	//		m_oOptionIndex = m_oOptions.size() - 1;

	//	UpdateOptionText();
	//}

	//else if (pEvent.key.code == sf::Keyboard::Down)
	//{
	//	// Increment and wrap-around
	//	if (m_oOptionIndex < m_oOptions.size() - 1)
	//		m_oOptionIndex++;
	//	else
	//		m_oOptionIndex = 0;

	//	UpdateOptionText();
	//}

	return false;
}

//void MenuState::UpdateOptionText()
//{
//	if (m_oOptions.empty())
//		return;
//
//	// White all texts
//	FOREACH(sf::Text & oText, m_oOptions)
//		oText.setFillColor(sf::Color::White);
//
//	// Red the selected text
//	m_oOptions[m_oOptionIndex].setFillColor(sf::Color::Red);
//}
