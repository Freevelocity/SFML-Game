#pragma once

#include "State.h"
#include "StateIdentifiers.h"
#include "SpriteIdentifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>


namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	explicit StateStack(State::Context pContext);

	template <typename T>
	void RegisterState(States::ID pStateID);

	void Update(sf::Time pDT);
	void Draw();
	void HandleEvent(const sf::Event& pEvent);

	void PushState(States::ID pStateID);
	void PopState();
	void ClearStates();

	bool IsEmpty() const;


private:
	State::Ptr CreateState(States::ID pStateID);
	void ApplyPendingChanges();


private:
	struct PendingChange
	{
		explicit PendingChange(Action pAction, States::ID pStateID = States::None);

		Action m_oAction;
		States::ID m_oStateID;
	};


private:
	std::vector<State::Ptr> m_oStack;
	std::vector<PendingChange> m_oPendingList;

	State::Context m_oContext;
	std::map<States::ID, std::function<State::Ptr()>> m_oFactories;
};


template <typename T>
void StateStack::RegisterState(States::ID pStateID)
{
	m_oFactories[pStateID] = [this]()
	{
		return State::Ptr(new T(*this, m_oContext));
	};
}
