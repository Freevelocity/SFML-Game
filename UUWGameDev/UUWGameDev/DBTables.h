#pragma once

#include "SpriteIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


class Craft;

struct Direction
{
	Direction(float pAngle, float pDistance)
		: fAngle(pAngle)
		, fDistance(pDistance)
	{
	}

	float fAngle;
	float fDistance;
};

struct CraftData
{
	int iHitpoints;
	float fSpeed;
	Textures::ID oTexture;
	sf::Time oFireInterval;
	std::vector<Direction> oDirections;
};

struct ProjectileData
{
	int	iDamage;
	float fSpeed;
	Textures::ID oTexture;
};

struct PickupData
{
	std::function<void(Craft&)>	oAction;
	Textures::ID oTexture;
};


std::vector<CraftData>	initializeCraftData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<PickupData>		initializePickupData();
