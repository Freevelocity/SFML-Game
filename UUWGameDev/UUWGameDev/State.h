#pragma once

#include "StateIdentifiers.h"
#include "SpriteIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& pWindow, TextureHolder& pTextures, FontHolder& pFonts, Player& pPlayer);

		sf::RenderWindow* m_oWindow;
		TextureHolder* m_oTextures;
		FontHolder* m_oFonts;
		Player* m_oPlayer;
	};


public:
	State(StateStack& pStack, Context pContext);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time pDT) = 0;
	virtual bool handleEvent(const sf::Event& pEvent) = 0;


protected:
	void RequestStackPush(States::ID stateID);
	void RequestStackPop();
	void RequestStateClear();

	Context GetContext() const;


private:
	StateStack* m_oStack;
	Context m_oContext;
};
