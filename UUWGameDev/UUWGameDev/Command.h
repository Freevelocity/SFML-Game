#pragma once

#include "PlayerType.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>


class SceneNode;

struct Command
{
	Command();
	typedef std::function<void(SceneNode&, sf::Time)>Action;
	Action m_oAction;
	unsigned int m_iPlayerType;
};

template <typename GameObject, typename Function>
Command::Action derivedAction(Function pFN)
{
	return [=](SceneNode& pNode, sf::Time pDT)
	{
		assert(dynamic_cast<GameObject*>(&pNode) != nullptr);
		pFN(static_cast<GameObject&>(pNode), pDT);
	};
}
//std::function<void(SceneNode&, sf::Time)> derivedAction(Function pFN)
//{
//	return [=](SceneNode& pNode, sf::Time pDT)
//	{
//		// Check if cast is safe
//		assert(dynamic_cast<GameObject*>(&pNode) != nullptr);
//
//		// Downcast node and invoke function on it
//		pFN(static_cast<GameObject&>(pNode), pDT);
//	};
//}