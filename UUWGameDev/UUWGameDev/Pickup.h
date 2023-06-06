#pragma once

#include "Entity.h"
#include "Command.h"
#include "SpriteIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Craft;

class Pickup : public Entity
{

	public:
		enum Type
		{
			HealthRefill,
			MissileRefill,
			FireSpread,
			FireRate,
			TypeCount
		};

	public:
		Pickup(Type pType, const TextureHolder& pTextures);

		virtual unsigned int	GetPlayerType() const;
		virtual sf::FloatRect	GetBoundingRect() const;

		void Apply(Craft& pPlayer) const;


	protected:
		virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


	private:
		Type m_oType;
		sf::Sprite m_oSprite;


};

