#pragma once
#include "Entity.h"

#include "SpriteIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
	public:
		enum Type
		{
			AlliedBullet,
			EnemyBullet,
			Missile,
			TypeCount
		};

	public:
		Projectile(Type pType, const TextureHolder& pTextures);

		void GuideTowards(sf::Vector2f pPosition);
		bool IsGuided() const;

		virtual unsigned int GetPlayerType() const;
		virtual sf::FloatRect GetBoundingRect() const;
		float GetMaxSpeed() const;
		int	GetDamage() const;


	private:
		virtual void UpdateCurrent(sf::Time pDT, CommandQueue& pCommands);
		virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


	private:
		Type m_oType;
		sf::Sprite m_oSprite;
		sf::Vector2f m_oTargetDirection;
};

