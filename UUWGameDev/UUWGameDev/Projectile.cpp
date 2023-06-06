#include "Projectile.h"
#include "DBTables.h"
#include "Utility.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>


namespace
{
    const std::vector<ProjectileData> DBProjectileTable = initializeProjectileData();
}


Projectile::Projectile(Type pType, const TextureHolder& pTextures)
    : Entity(1)
    , m_oType(pType)
    , m_oSprite(pTextures.get(DBProjectileTable[pType].oTexture))
    , m_oTargetDirection()
{
    CenterOrigin(m_oSprite);
}

void Projectile::GuideTowards(sf::Vector2f pPosition)
{
    assert(IsGuided());
    m_oTargetDirection = unitVector(pPosition - GetWorldPosition());
}

bool Projectile::IsGuided() const
{
    return m_oType == Missile;
}

unsigned int Projectile::GetPlayerType() const
{
    if (m_oType == EnemyBullet)
        return PlayerType::EnemyProjectile;
    else
        return PlayerType::AlliedProjectile;
}

sf::FloatRect Projectile::GetBoundingRect() const
{
    //return sf::FloatRect();
    return GetWorldTransform().transformRect(m_oSprite.getGlobalBounds());

}

float Projectile::GetMaxSpeed() const
{
    return DBProjectileTable[m_oType].fSpeed;
}

int Projectile::GetDamage() const
{
    return DBProjectileTable[m_oType].iDamage;
}

void Projectile::UpdateCurrent(sf::Time pDT, CommandQueue& pCommands)
{
    if (IsGuided())
    {
        const float fApproachRate = 200.f;

        sf::Vector2f newVelocity = unitVector(fApproachRate * pDT.asSeconds() * m_oTargetDirection + GetVelocity());
        newVelocity *= GetMaxSpeed();
        float angle = std::atan2(newVelocity.y, newVelocity.x);

        setRotation(toDegree(angle) + 90.f);
        SetVelocity(newVelocity);
    }

    Entity::UpdateCurrent(pDT, pCommands);

}

void Projectile::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
    pTarget.draw(m_oSprite, pStates);
}
