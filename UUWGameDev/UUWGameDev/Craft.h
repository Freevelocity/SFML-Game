#pragma once

#include "Entity.h"
#include "Command.h"
#include "SpriteIdentifiers.h"
#include "Projectile.h"
#include "TextNode.h"

#include <SFML/Graphics/Sprite.hpp>

class Craft: public Entity
{

public:
	enum Type
	{
		PassengerJet,
		MilitaryJet,
		EnemyJet1,
		TypeCount,
	};


public:
	Craft(Type pType, const TextureHolder& pTextures, const FontHolder& pFonts);
	virtual unsigned int GetPlayerType() const;
	virtual sf::FloatRect	GetBoundingRect() const;
	virtual bool 			IsMarkedForRemoval() const;

	bool IsAllied() const;
	float GetMaxSpeed() const;

	void IncreaseFireRate();
	void IncreaseSpread();
	void CollectMissiles(unsigned int pCount);

	void Fire();
	void LaunchMissile();


private:
	virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;
	virtual void UpdateCurrent(sf::Time pDT, CommandQueue& pCommands);
	void UpdateMovementPattern(sf::Time pDT);
	void CheckPickupDrop(CommandQueue& pCommands);
	void CheckProjectileLaunch(sf::Time pDT, CommandQueue& pCommands);

	void CreateBullets(SceneNode& pNode, const TextureHolder& pTextures) const;
	void CreateProjectile(SceneNode& pNode, Projectile::Type pType, float pXOffset, float pYOffset, const TextureHolder& pTextures) const;
	void CreatePickup(SceneNode& pNode, const TextureHolder& pTextures) const;

	void UpdateTexts();


private:
	Type m_oType;
	sf::Sprite m_oSprite;
	Command m_oFireCommand;
	Command m_oMissileCommand;
	sf::Time m_oFireCountdown;
	bool m_bIsFiring;
	bool m_bIsLaunchingMissile;
	bool m_bIsMarkedForRemoval;

	int	m_iFireRateLevel;
	int	m_iSpreadLevel;
	int	m_iMissileAmmo;

	Command m_oDropPickupCommand;
	float m_fTravelledDistance;
	std::size_t	m_oDirectionIndex;
	TextNode* m_oHealthDisplay;
	TextNode* m_oMissileDisplay;

};

