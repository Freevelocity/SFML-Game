#include "Pickup.h"
#include "DBTables.h"
#include "PlayerType.h"
#include "CommandQueue.h"
#include "Utility.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PickupData> PickUpTable = initializePickupData();
}

Pickup::Pickup(Type pType, const TextureHolder& pTextures)
	: Entity(1)
	, m_oType(pType)
	, m_oSprite(pTextures.get(PickUpTable[pType].oTexture))
{
	CenterOrigin(m_oSprite);
}

unsigned int Pickup::GetPlayerType() const
{
	return PlayerType::Pickup;
}

sf::FloatRect Pickup::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_oSprite.getGlobalBounds());
}

void Pickup::Apply(Craft& pPlayer) const
{
	PickUpTable[m_oType].oAction(pPlayer);
}

void Pickup::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	pTarget.draw(m_oSprite, pStates);
}
