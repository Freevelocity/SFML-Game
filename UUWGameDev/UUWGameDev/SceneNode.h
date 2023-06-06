#pragma once

#include "PlayerType.h"
//#include "Command.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <utility>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;
		typedef std::pair<SceneNode*, SceneNode*> Pair;


	public:
		//SceneNode();
		explicit SceneNode(PlayerType::Type pPlayerType = PlayerType::None);

		void AttachChild(Ptr pChild);
		Ptr	 DetachChild(const SceneNode& pNode);

		//void Update(sf::Time pDT);
		void Update(sf::Time pDT, CommandQueue& pCommands);

		sf::Vector2f GetWorldPosition() const;
		sf::Transform GetWorldTransform() const;

		void OnCommand(const Command& pCommand, sf::Time pDT);
		virtual unsigned int GetPlayerType() const;

		void CheckSceneCollision(SceneNode& pSceneGraph, std::set<Pair>& pCollisionPairs);
		void CheckNodeCollision(SceneNode& pNode, std::set<Pair>& pCollisionPairs);
		void RemoveWrecks();
		virtual sf::FloatRect GetBoundingRect() const;
		virtual bool IsMarkedForRemoval() const;
		virtual bool IsDestroyed() const;

	private:
		virtual void UpdateCurrent(sf::Time pDT, CommandQueue& pCommands);
		void UpdateChildren(sf::Time pDT, CommandQueue& pCommands);

		virtual void draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;
		virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;
		void DrawChildren(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;
		void DrawBoundingRect(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


	private:
		std::vector<Ptr> m_oChildren;
		SceneNode* m_oParent;
		PlayerType::Type m_oDefaultPlayerType;
};

bool Collision(const SceneNode& lhs, const SceneNode& rhs);
float Distance(const SceneNode& lhs, const SceneNode& rhs);