#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState : public State
{
public:
	TitleState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);


private:
	sf::Sprite m_oBackgroundSprite;
	sf::Text m_oText;

	bool m_bShowText;
	sf::Time m_oTextEffectTime;
};

