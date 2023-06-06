#include "Craft.h"
#include "DBTables.h"
#include "Utility.h"
#include "Pickup.h"
#include "CommandQueue.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
//Textures::ID toTextureID(Craft::Type type)
//{
//	switch (type)
//	{
//	case Craft::PassengerJet:
//		return Textures::PassengerJet;
//
//	case Craft::MilitaryJet:
//		return Textures::MilitaryJet;
//	}
//	return Textures::PassengerJet;
//}

namespace
{
	const std::vector<CraftData> DBCraftTable = initializeCraftData();
}

Craft::Craft(Type pType, const TextureHolder& pTextures, const FontHolder& pFonts)
	: Entity(DBCraftTable[pType].iHitpoints)
	,m_oType(pType)
	, m_oSprite(pTextures.get(DBCraftTable[pType].oTexture))
	, m_oFireCommand()
	, m_oMissileCommand()
	, m_oFireCountdown(sf::Time::Zero)
	, m_bIsFiring(false)
	, m_bIsLaunchingMissile(false)
	, m_bIsMarkedForRemoval(false)
	, m_iFireRateLevel(1)
	, m_iSpreadLevel(1)
	, m_iMissileAmmo(2)
	, m_oDropPickupCommand()
	, m_fTravelledDistance(0.f)
	, m_oDirectionIndex(0)
	, m_oHealthDisplay(nullptr)
	, m_oMissileDisplay(nullptr)
{
	/*sf::FloatRect bounds = m_oSprite.getLocalBounds();
	m_oSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);*/
	CenterOrigin(m_oSprite);

	m_oFireCommand.m_iPlayerType = PlayerType::Scene;
	m_oFireCommand.m_oAction = [this, &pTextures](SceneNode& pNode, sf::Time)
	{
		CreateBullets(pNode, pTextures);
	};

	m_oMissileCommand.m_iPlayerType = PlayerType::Scene;
	m_oMissileCommand.m_oAction = [this, &pTextures](SceneNode& pNode, sf::Time)
	{
		CreateProjectile(pNode, Projectile::Missile, 0.f, 0.5f, pTextures);
	};

	m_oDropPickupCommand.m_iPlayerType = PlayerType::Scene;
	m_oDropPickupCommand.m_oAction = [this, &pTextures](SceneNode& pNode, sf::Time)
	{
		CreatePickup(pNode, pTextures);
	};

	std::unique_ptr<TextNode> oHealthDisplay(new TextNode(pFonts, ""));
	m_oHealthDisplay = oHealthDisplay.get();
	AttachChild(std::move(oHealthDisplay));

	if (GetPlayerType() == PlayerType::PlayerCraft)
	{
		std::unique_ptr<TextNode> oMissileDisplay(new TextNode(pFonts, ""));
		oMissileDisplay->setPosition(0, 70);
		m_oMissileDisplay = oMissileDisplay.get();
		AttachChild(std::move(oMissileDisplay));
	}

	UpdateTexts();
}

unsigned int Craft::GetPlayerType() const
{
	/*switch (m_oType)
	{
	case PassengerJet:
		return PlayerType::PlayerCraft;

	default:
		return PlayerType::EnemyCraft;
	}*/
	if (IsAllied())
		return PlayerType::PlayerCraft;
	else
		return PlayerType::EnemyCraft;
}

sf::FloatRect Craft::GetBoundingRect() const
{
	//return sf::FloatRect();
	return GetWorldTransform().transformRect(m_oSprite.getGlobalBounds());
}

bool Craft::IsMarkedForRemoval() const
{
	//return false;
	return m_bIsMarkedForRemoval;
}

bool Craft::IsAllied() const
{
	//return false;
	return m_oType == PassengerJet;
}

float Craft::GetMaxSpeed() const
{
	//return 0.0f;
	return DBCraftTable[m_oType].fSpeed;
}

void Craft::IncreaseFireRate()
{
	if (m_iFireRateLevel < 10)
		++m_iFireRateLevel;
}

void Craft::IncreaseSpread()
{
	if (m_iSpreadLevel < 3)
		++m_iSpreadLevel;
}

void Craft::CollectMissiles(unsigned int pCount)
{
	m_iMissileAmmo += pCount;
}

void Craft::Fire()
{
	if (DBCraftTable[m_oType].oFireInterval != sf::Time::Zero)
		m_bIsFiring = true;
}

void Craft::LaunchMissile()
{
	if (m_iMissileAmmo > 0)
	{
		m_bIsLaunchingMissile = true;
		--m_iMissileAmmo;
	}
}

void Craft::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	pTarget.draw(m_oSprite, pStates);
}

void Craft::UpdateCurrent(sf::Time pDT, CommandQueue& pCommands)
{
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (IsDestroyed())
	{
		CheckPickupDrop(pCommands);

		m_bIsMarkedForRemoval = true;
		return;
	}

	// Check if bullets or missiles are fired
	CheckProjectileLaunch(pDT, pCommands);

	// Update enemy movement pattern; apply velocity
	UpdateMovementPattern(pDT);
	Entity::UpdateCurrent(pDT, pCommands);

	// Update texts
	UpdateTexts();
}

void Craft::UpdateMovementPattern(sf::Time pDT)
{
	const std::vector<Direction>& oDirections = DBCraftTable[m_oType].oDirections;
	if (!oDirections.empty())
	{
		// Moved long enough in current direction: Change direction
		if (m_fTravelledDistance > oDirections[m_oDirectionIndex].fDistance)
		{
			m_oDirectionIndex = (m_oDirectionIndex + 1) % oDirections.size();
			m_fTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(oDirections[m_oDirectionIndex].fAngle + 90.f);
		float fVx = GetMaxSpeed() * std::cos(radians);
		float fVy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(fVx, fVy);

		m_fTravelledDistance += GetMaxSpeed() * pDT.asSeconds();
	}
}

void Craft::CheckPickupDrop(CommandQueue& pCommands)
{
	if (!IsAllied() && randomInt(3) == 0)
		pCommands.Push(m_oDropPickupCommand);
}

void Craft::CheckProjectileLaunch(sf::Time pDT, CommandQueue& pCommands)
{
	// Enemies try to fire all the time
	if (!IsAllied())
		Fire();

	// Check for automatic gunfire, allow only in intervals
	if (m_bIsFiring && m_oFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		pCommands.Push(m_oFireCommand);
		m_oFireCountdown += DBCraftTable[m_oType].oFireInterval / (m_iFireRateLevel + 1.f);
		m_bIsFiring = false;
	}
	else if (m_oFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		m_oFireCountdown -= pDT;
		m_bIsFiring = false;
	}

	// Check for missile launch
	if (m_bIsLaunchingMissile)
	{
		pCommands.Push(m_oMissileCommand);
		m_bIsLaunchingMissile = false;
	}
}

void Craft::CreateBullets(SceneNode& pNode, const TextureHolder& pTextures) const
{
	Projectile::Type oType = IsAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (m_iSpreadLevel)
	{
	case 1:
		CreateProjectile(pNode, oType, 0.0f, 0.5f, pTextures);
		break;

	case 2:
		CreateProjectile(pNode, oType, -0.33f, 0.33f, pTextures);
		CreateProjectile(pNode, oType, +0.33f, 0.33f, pTextures);
		break;

	case 3:
		CreateProjectile(pNode, oType, -0.5f, 0.33f, pTextures);
		CreateProjectile(pNode, oType, 0.0f, 0.5f, pTextures);
		CreateProjectile(pNode, oType, +0.5f, 0.33f, pTextures);
		break;
	}
}

void Craft::CreateProjectile(SceneNode& pNode, Projectile::Type pType, float pXOffset, float pYOffset, const TextureHolder& pTextures) const
{
	std::unique_ptr<Projectile> oProjectile(new Projectile(pType, pTextures));

	sf::Vector2f offset(pXOffset * m_oSprite.getGlobalBounds().width, pYOffset * m_oSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, oProjectile->GetMaxSpeed());

	float sign = IsAllied() ? -1.f : +1.f;
	oProjectile->setPosition(GetWorldPosition() + offset * sign);
	oProjectile->SetVelocity(velocity * sign);
	pNode.AttachChild(std::move(oProjectile));
}

void Craft::CreatePickup(SceneNode& pNode, const TextureHolder& pTextures) const
{
	auto oType = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> oPickup(new Pickup(oType, pTextures));
	oPickup->setPosition(GetWorldPosition());
	oPickup->SetVelocity(0.f, 1.f);
	pNode.AttachChild(std::move(oPickup));
}

void Craft::UpdateTexts()
{

	m_oHealthDisplay->SetString(toString(GetHitpoints()) + " HP");
	m_oHealthDisplay->setPosition(0.f, 50.f);
	m_oHealthDisplay->setRotation(-getRotation());

	if (m_oMissileDisplay)
	{
		if (m_iMissileAmmo == 0)
			m_oMissileDisplay->SetString("");
		else
			m_oMissileDisplay->SetString("M: " + toString(m_iMissileAmmo));
	}

}
