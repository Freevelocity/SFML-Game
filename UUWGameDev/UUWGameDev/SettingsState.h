#pragma once
#include "State.h"
#include "Player.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
public:
	SettingsState(StateStack& pStack, Context pContext);

	virtual void draw();
	virtual bool update(sf::Time pDT);
	virtual bool handleEvent(const sf::Event& pEvent);


private:
	void UpdateLabels();
	void AddButtonLabel(Player::Action pAction, float pY, const std::string& pText, Context pContext);


private:
	sf::Sprite m_oBackgroundSprite;
	GUI::Container m_oGUIContainer;
	std::array<GUI::Button::Ptr, Player::ActionCount> m_oBindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount> m_oBindingLabels;
};
