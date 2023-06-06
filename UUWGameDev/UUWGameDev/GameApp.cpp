#include "GameApp.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"
#include "GameOverState.h"

//#include "StringHelpers.h"
//#include <iostream>

//#include <SFML/Window/Event.hpp>


//const float GameApp::Const_PlayerSpeed = 100.f;
const sf::Time GameApp::Const_TimePerFrame = sf::seconds(1.f / 60.f);

GameApp::GameApp()
	: m_oWindow(sf::VideoMode(1024, 768), "UUW Game App", sf::Style::Close)
	, m_oTextures()
	, m_oFonts()
	//, m_oPlayer()
	//, m_oLandscape()
	//, m_oWorld(m_oWindow)
	, m_oPlayer()
	//, m_oFont()
	, m_oStateStack(State::Context(m_oWindow, m_oTextures, m_oFonts, m_oPlayer))
	, m_oStatisticsText()
	, m_oStatisticsUpdateTime()
	, m_oStatisticsNumFrames(0)
	//, m_bIsMovingUp(false)
	//, m_bIsMovingDown(false)
	//, m_bIsMovingRight(false)
	//, m_bIsMovingLeft(false)
{
	m_oWindow.setKeyRepeatEnabled(false);

	/*try
	{
		m_oSpriteHolder.SLoad(Textures::Landscape, "Graphics/Desert.png");
		m_oSpriteHolder.SLoad(Textures::PassengerJet, "Graphics/PassengerJet.png");
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		return;
	}*/

	//if (!m_oTexture.loadFromFile("Graphics/PassengerJet.png"))
	//{
	//	// Handle loading error
	//}



	////m_oPlayer.setTexture(m_oTexture);
	//m_oLandscape.setTexture(m_oSpriteHolder.get(Textures::Landscape));
	//m_oPlayer.setTexture(m_oSpriteHolder.get(Textures::PassengerJet));
	//m_oPlayer.setPosition(100.f, 100.f);

	//m_oFont.loadFromFile("Graphics/Fonts/Sansation.ttf");
	m_oFonts.SLoad(Fonts::Main, "Graphics/Fonts/Sansation.ttf");
	m_oTextures.SLoad(Textures::TitleScreen, "Graphics/TitleScreen.png");
	m_oTextures.SLoad(Textures::ButtonNormal, "Graphics/ButtonNormal.png");
	m_oTextures.SLoad(Textures::ButtonSelected, "Graphics/ButtonSelected.png");
	m_oTextures.SLoad(Textures::ButtonPressed, "Graphics/ButtonPressed.png");
	m_oStatisticsText.setFont(m_oFonts.get(Fonts::Main));
	m_oStatisticsText.setPosition(5.f, 5.f);
	m_oStatisticsText.setCharacterSize(10u);
	RegisterStates();
	m_oStateStack.PushState(States::Title);
}

void GameApp::Start()
{
	sf::Clock oClock;
	sf::Time oTimeSinceLastUpdate = sf::Time::Zero;
	while (m_oWindow.isOpen())
	{
		sf::Time oElapsedTime = oClock.restart();
		oTimeSinceLastUpdate += oElapsedTime;
		while (oTimeSinceLastUpdate > Const_TimePerFrame)
		{
			oTimeSinceLastUpdate -= Const_TimePerFrame;
			//ProcessEvents();
			ProcessInput();
			Update(Const_TimePerFrame);
			if (m_oStateStack.IsEmpty())
				m_oWindow.close();
		}
		UpdateStatistics(oElapsedTime);
		Render();
	}
}

//void GameApp::ProcessEvents()
//{
//	sf::Event oEvent;
//	while (m_oWindow.pollEvent(oEvent))
//	{
//		switch (oEvent.type)
//		{
//		case sf::Event::KeyPressed:
//			HandlePlayerInput(oEvent.key.code, true);
//			break;
//
//		case sf::Event::KeyReleased:
//			HandlePlayerInput(oEvent.key.code, false);
//			break;
//
//		case sf::Event::Closed:
//			m_oWindow.close();
//			break;
//		}
//	}
//}

void GameApp::ProcessInput()
{

	sf::Event oEvent;
	while (m_oWindow.pollEvent(oEvent))
	{
		m_oStateStack.HandleEvent(oEvent);

		if (oEvent.type == sf::Event::Closed)
			m_oWindow.close();
	}

	//CommandQueue& oCommands = m_oWorld.GetCommandQueue();

	/*sf::Event event;
	while (m_oWindow.pollEvent(event))
	{
		m_oPlayer.HandleEvent(event, oCommands);

		if (event.type == sf::Event::Closed)
			m_oWindow.close();
	}

	m_oPlayer.HandleRealtimeInput(oCommands);*/

}

void GameApp::Update(sf::Time pElapsedTime)
{
	m_oStateStack.Update(pElapsedTime);
	//m_oWorld.Update(pElapsedTime);
	/*sf::Vector2f movement(0.f, 0.f);
	if (m_bIsMovingUp)
		movement.y -= Const_PlayerSpeed;
	if (m_bIsMovingDown)
		movement.y += Const_PlayerSpeed;
	if (m_bIsMovingLeft)
		movement.x -= Const_PlayerSpeed;
	if (m_bIsMovingRight)
		movement.x += Const_PlayerSpeed;

	m_oPlayer.move(movement * pElapsedTime.asSeconds());*/
}

void GameApp::Render()
{
	m_oWindow.clear(); 
	m_oStateStack.Draw();

	m_oWindow.setView(m_oWindow.getDefaultView());
	m_oWindow.draw(m_oStatisticsText);

	m_oWindow.display();
	//m_oWorld.Draw();
	//m_oWindow.setView(m_oWindow.getDefaultView());
	////m_oWindow.draw(m_oLandscape);
	////m_oWindow.draw(m_oPlayer);

	//m_oWindow.draw(m_oStatisticsText);
	//m_oWindow.display();
}

void GameApp::UpdateStatistics(sf::Time pElapsedTime)
{
	m_oStatisticsUpdateTime += pElapsedTime;
	m_oStatisticsNumFrames += 1;

	if (m_oStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		m_oStatisticsText.setString(
			"Frames / Second = " + toString(m_oStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(m_oStatisticsUpdateTime.asMicroseconds() / m_oStatisticsNumFrames) + "us");

		m_oStatisticsUpdateTime -= sf::seconds(1.0f);
		m_oStatisticsNumFrames = 0;
	}
}

void GameApp::RegisterStates()
{
	m_oStateStack.RegisterState<TitleState>(States::Title);
	m_oStateStack.RegisterState<MenuState>(States::Menu);
	m_oStateStack.RegisterState<GameState>(States::Game);
	m_oStateStack.RegisterState<PauseState>(States::Pause);
	m_oStateStack.RegisterState<SettingsState>(States::Settings);
	m_oStateStack.RegisterState<GameOverState>(States::GameOver);
}

//void GameApp::HandlePlayerInput(sf::Keyboard::Key pKey, bool pIsPressed)
//{
//	/*if (pKey == sf::Keyboard::W)
//		m_bIsMovingUp = pIsPressed;
//	else if (pKey == sf::Keyboard::S)
//		m_bIsMovingDown = pIsPressed;
//	else if (pKey == sf::Keyboard::A)
//		m_bIsMovingLeft = pIsPressed;
//	else if (pKey == sf::Keyboard::D)
//		m_bIsMovingRight = pIsPressed;*/
//}