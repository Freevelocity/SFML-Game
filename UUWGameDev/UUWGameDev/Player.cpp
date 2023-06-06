#include "Player.h"
#include "CommandQueue.h"
#include "Craft.h"
#include "Foreach.h"

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

struct CraftMover
{
	CraftMover(float pVx, float pVy)
		: m_oVelocity(pVx, pVy)
	{
	}

	void operator() (Craft& pCraft, sf::Time) const
	{
		//pCraft.Accelerate(m_oVelocity);
		pCraft.Accelerate(m_oVelocity * pCraft.GetMaxSpeed());
	}

	sf::Vector2f m_oVelocity;
};

Player::Player()
	: m_oCurrentMissionStatus(Player::MissionRunning)
{
	// Set initial key bindings
	m_oKeyBinding[sf::Keyboard::Left] = MoveLeft;
	m_oKeyBinding[sf::Keyboard::Right] = MoveRight;
	m_oKeyBinding[sf::Keyboard::Up] = MoveUp;
	m_oKeyBinding[sf::Keyboard::Down] = MoveDown;
	m_oKeyBinding[sf::Keyboard::Space] = Fire;
	m_oKeyBinding[sf::Keyboard::M] = LaunchMissile;

	// Set initial action bindings
	InitializeActions();

	// Assign all categories to player's aircraft
	FOREACH(auto & oPair, m_oActionBinding)
		oPair.second.m_iPlayerType = PlayerType::PlayerCraft;
}

void Player::HandleEvent(const sf::Event& pEvent, CommandQueue& pCommands)
{
	if (pEvent.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto oFound = m_oKeyBinding.find(pEvent.key.code);
		if (oFound != m_oKeyBinding.end() && !IsRealtimeAction(oFound->second))
			pCommands.Push(m_oActionBinding[oFound->second]);
	}
}

void Player::HandleRealtimeInput(CommandQueue& pCommands)
{
	
	FOREACH(auto oPair, m_oKeyBinding)
	{
		
		if (sf::Keyboard::isKeyPressed(oPair.first) && IsRealtimeAction(oPair.second))
			pCommands.Push(m_oActionBinding[oPair.second]);
	}
}

void Player::AssignKey(Action pAction, sf::Keyboard::Key pKey)
{

	for (auto itr = m_oKeyBinding.begin(); itr != m_oKeyBinding.end(); )
	{
		if (itr->second == pAction)
			m_oKeyBinding.erase(itr++);
		else
			++itr;
	}

	
	m_oKeyBinding[pKey] = pAction;
}

sf::Keyboard::Key Player::GetAssignedKey(Action pAction) const
{
	FOREACH(auto oPair, m_oKeyBinding)
	{
		if (oPair.second == pAction)
			return oPair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::SetMissionStatus(MissionStatus pStatus)
{
	m_oCurrentMissionStatus = pStatus;
}

Player::MissionStatus Player::GetMissionStatus() const
{
	return m_oCurrentMissionStatus;
}

//sf::Keyboard::Key Player::GetAssignedKey(Action pAction) const
//{
//	FOREACH(auto oPair, m_oKeyBinding)
//	{
//		if (oPair.second == pAction)
//			return oPair.first;
//	}
//
//	return sf::Keyboard::Unknown;
//}

void Player::InitializeActions()
{
	//const float fPlayerSpeed = 200.f;

	m_oActionBinding[MoveLeft].m_oAction = derivedAction<Craft>(CraftMover(-1, 0));
	m_oActionBinding[MoveRight].m_oAction = derivedAction<Craft>(CraftMover(+1, 0));
	m_oActionBinding[MoveUp].m_oAction = derivedAction<Craft>(CraftMover(0, -1));
	m_oActionBinding[MoveDown].m_oAction = derivedAction<Craft>(CraftMover(0, +1));
	m_oActionBinding[Fire].m_oAction = derivedAction<Craft>([](Craft& pCraft, sf::Time) { pCraft.Fire(); });
	m_oActionBinding[LaunchMissile].m_oAction = derivedAction<Craft>([](Craft& pCraft, sf::Time) { pCraft.LaunchMissile(); });
}

bool Player::IsRealtimeAction(Action pAction)
{
	switch (pAction)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
	case Fire:
		return true;

	default:
		return false;
	}
}


