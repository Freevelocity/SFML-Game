#include "SceneNode.h"
#include "Command.h"
#include "Foreach.h"
#include "Utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


SceneNode::SceneNode(PlayerType::Type pPlayerType)
	: m_oChildren()
	, m_oParent(nullptr)
	, m_oDefaultPlayerType(pPlayerType)
{
}

void SceneNode::AttachChild(Ptr pChild)
{
	pChild->m_oParent = this;
	m_oChildren.push_back(std::move(pChild));
}

SceneNode::Ptr SceneNode::DetachChild(const SceneNode& pNode)
{

	auto found = std::find_if(m_oChildren.begin(), m_oChildren.end(), [&](Ptr& p) { return p.get() == &pNode; });
	assert(found != m_oChildren.end());

	Ptr result = std::move(*found);
	result->m_oParent = nullptr;
	m_oChildren.erase(found);
	return result;
	//return Ptr();
}

void SceneNode::Update(sf::Time pDT, CommandQueue& pCommands)
{
	UpdateCurrent(pDT, pCommands);
	UpdateChildren(pDT, pCommands);
}

//void SceneNode::UpdateCurrent(sf::Time, CommandQueue&)
//{
//	// Do nothing by default
//}

sf::Vector2f SceneNode::GetWorldPosition() const
{
	return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const
{
	sf::Transform oTransform = sf::Transform::Identity;

	for (const SceneNode* oNode = this; oNode != nullptr; oNode = oNode->m_oParent)
		oTransform = oNode->getTransform() * oTransform;

	return oTransform;
}

void SceneNode::OnCommand(const Command& pCommand, sf::Time pDT)
{
	// Command current node, if category matches
	if (pCommand.m_iPlayerType & GetPlayerType())
		pCommand.m_oAction(*this, pDT);

	// Command children
	FOREACH(Ptr & oChild, m_oChildren)
		oChild->OnCommand(pCommand, pDT);
}

unsigned int SceneNode::GetPlayerType() const
{
	//return PlayerType::Scene;
	return m_oDefaultPlayerType;
}

void SceneNode::CheckSceneCollision(SceneNode& pSceneGraph, std::set<Pair>& pCollisionPairs)
{
	CheckNodeCollision(pSceneGraph, pCollisionPairs);

	FOREACH(Ptr & oChild, pSceneGraph.m_oChildren)
		CheckSceneCollision(*oChild, pCollisionPairs);
}

void SceneNode::CheckNodeCollision(SceneNode& pNode, std::set<Pair>& pCollisionPairs)
{

	if (this != &pNode && Collision(*this, pNode) && !IsDestroyed() && !pNode.IsDestroyed())
		pCollisionPairs.insert(std::minmax(this, &pNode));

	FOREACH(Ptr & oChild, m_oChildren)
		oChild->CheckNodeCollision(pNode, pCollisionPairs);
}

void SceneNode::RemoveWrecks()
{
	auto oWreckfieldBegin = std::remove_if(m_oChildren.begin(), m_oChildren.end(), std::mem_fn(&SceneNode::IsMarkedForRemoval));
	m_oChildren.erase(oWreckfieldBegin, m_oChildren.end());

	// Call function recursively for all remaining children
	std::for_each(m_oChildren.begin(), m_oChildren.end(), std::mem_fn(&SceneNode::RemoveWrecks));
}

sf::FloatRect SceneNode::GetBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::IsMarkedForRemoval() const
{
	//return false;
	return IsDestroyed();
}

bool SceneNode::IsDestroyed() const
{
	return false;
}

//void SceneNode::UpdateCurrent(sf::Time pDT)
//{
//}

void SceneNode::UpdateCurrent(sf::Time pDT, CommandQueue& pCommands)
{

}

void SceneNode::UpdateChildren(sf::Time pDT, CommandQueue& pCommands)
{
	FOREACH(Ptr & oChild, m_oChildren)
		oChild->Update(pDT, pCommands);
}

void SceneNode::draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{

	pStates.transform *= getTransform();

	DrawCurrent(pTarget, pStates);
	DrawChildren(pTarget, pStates);
}

void SceneNode::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{

}

void SceneNode::DrawChildren(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	FOREACH(const Ptr & oChild, m_oChildren)
		oChild->draw(pTarget, pStates);
}

void SceneNode::DrawBoundingRect(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	sf::FloatRect oRect = GetBoundingRect();

	sf::RectangleShape oShape;
	oShape.setPosition(sf::Vector2f(oRect.left, oRect.top));
	oShape.setSize(sf::Vector2f(oRect.width, oRect.height));
	oShape.setFillColor(sf::Color::Transparent);
	oShape.setOutlineColor(sf::Color::Green);
	oShape.setOutlineThickness(1.f);

	pTarget.draw(oShape);
}

bool Collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.GetBoundingRect().intersects(rhs.GetBoundingRect());
}

float Distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.GetWorldPosition() - rhs.GetWorldPosition());
}


//void SceneNode::RemoveWrecks()
//{
//	// Remove all children which request so
//	auto oWreckfieldBegin = std::remove_if(m_oChildren.begin(), m_oChildren.end(), std::mem_fn(&SceneNode::IsMarkedForRemoval));
//	m_oChildren.erase(oWreckfieldBegin, m_oChildren.end());
//
//	// Call function recursively for all remaining children
//	std::for_each(m_oChildren.begin(), m_oChildren.end(), std::mem_fn(&SceneNode::RemoveWrecks));
//}