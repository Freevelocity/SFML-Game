#pragma once
#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State
{
public:
	PauseState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);


private:
	sf::Sprite m_oBackgroundSprite;
	sf::Text m_oPausedText;
	//sf::Text m_oInstructionText;
	GUI::Container m_oGUIContainer;
};

