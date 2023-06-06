#pragma once


#include "State.h"
#include "World.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State
{
public:
	GameState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);


private:
	World m_oWorld;
	Player& m_oPlayer;
};

