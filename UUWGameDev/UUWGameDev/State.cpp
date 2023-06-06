#include "State.h"
#include "StateStack.h"

State::State(StateStack& pStack, Context pContext)
	: m_oStack(&pStack)
	, m_oContext(pContext)
{
}

State::~State()
{
}



void State::RequestStackPush(States::ID stateID)
{
	m_oStack->PushState(stateID);
}

void State::RequestStackPop()
{
	m_oStack->PopState();
}

void State::RequestStateClear()
{
	m_oStack->ClearStates();
}

State::Context State::GetContext() const
{
	return m_oContext;
}

State::Context::Context(sf::RenderWindow& pWindow, TextureHolder& pTextures, FontHolder& pFonts, Player& pPlayer)
	: m_oWindow(&pWindow)
	, m_oTextures(&pTextures)
	, m_oFonts(&pFonts)
	, m_oPlayer(&pPlayer)
{
}
