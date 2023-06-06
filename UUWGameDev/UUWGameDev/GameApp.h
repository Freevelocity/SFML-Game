#pragma once

//#include "World.h"
#include "SpriteHolder.h"
#include "SpriteIdentifiers.h"
#include "Player.h"
#include "StateStack.h"

#include <SFML/System/Time.hpp>
//#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/Font.hpp>


//namespace Textures
//{
//	enum ID
//	{
//		Landscape,
//		PassengerJet,
//	};
//}

class GameApp : private sf::NonCopyable
{

public:
	GameApp();
	void Start();


private:
	//void ProcessEvents();
	void ProcessInput();
	void Update(sf::Time pElapsedTime);
	void Render();

	void UpdateStatistics(sf::Time pElapsedTime);
	//void HandlePlayerInput(sf::Keyboard::Key pKey, bool pIsPressed);
	void RegisterStates();


private:
	//static const float Const_PlayerSpeed;
	static const sf::Time	Const_TimePerFrame;

	sf::RenderWindow m_oWindow;
	/*SpriteHolder<sf::Texture, Textures::ID> m_oTextures;
	SpriteHolder<sf::Font, Fonts::ID> m_oFonts;*/
	TextureHolder m_oTextures;
	FontHolder m_oFonts;
	
	//World m_oWorld;
	Player m_oPlayer;
	StateStack m_oStateStack;
	//sf::Texture	 m_oTexture;
	/*SpriteHolder<sf::Texture, Textures::ID>  m_oSpriteHolder;
	sf::Sprite m_oPlayer;
	sf::Sprite m_oLandscape;*/
	//sf::Font m_oFont;
	sf::Text m_oStatisticsText;
	sf::Time m_oStatisticsUpdateTime;
	std::size_t	 m_oStatisticsNumFrames;
	/*bool m_bIsMovingUp;
	bool m_bIsMovingDown;
	bool m_bIsMovingRight;
	bool m_bIsMovingLeft;*/


};

