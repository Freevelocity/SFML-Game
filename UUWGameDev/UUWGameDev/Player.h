#pragma once


#include "Command.h"
#include <SFML/Window/Event.hpp>
#include <map>


class CommandQueue;

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount

	};

	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};

public:
	Player();

	void HandleEvent(const sf::Event& pEvent, CommandQueue& pCommands);
	void HandleRealtimeInput(CommandQueue& pCommands);

	void AssignKey(Action pAction, sf::Keyboard::Key pKey);
	sf::Keyboard::Key GetAssignedKey(Action pAction) const;

	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus() const;

private:
	void InitializeActions();
	static bool	IsRealtimeAction(Action pAction);


private:
	std::map<sf::Keyboard::Key, Action>	m_oKeyBinding;
	std::map<Action, Command> m_oActionBinding;
	MissionStatus m_oCurrentMissionStatus;
};
