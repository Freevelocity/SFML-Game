#pragma once

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
//#include "State.h"

class GameOverState : public State
{

public:
	GameOverState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);


private:
	sf::Text m_oGameOverText;
	sf::Time m_oElapsedTime;

};

