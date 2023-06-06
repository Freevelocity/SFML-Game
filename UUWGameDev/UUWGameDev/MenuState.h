#pragma once


#include "State.h"
#include "Container.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
public:
	MenuState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);

	///void UpdateOptionText();


//private:
//	enum OptionNames
//	{
//		Play,
//		Exit,
//	};


private:
	sf::Sprite m_oBackgroundSprite;
	GUI::Container	m_oGUIContainer;
	/*std::vector<sf::Text> m_oOptions;
	std::size_t	m_oOptionIndex;*/
};

