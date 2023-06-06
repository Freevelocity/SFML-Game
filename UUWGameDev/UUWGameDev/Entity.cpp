#include "Entity.h"
#include <cassert>

Entity::Entity(int pHitpoints)
    : m_oVelocity()
    , m_iHitpoints(pHitpoints)
{
}

void Entity::SetVelocity(sf::Vector2f pVelocity)
{

    m_oVelocity = pVelocity;

}

void Entity::SetVelocity(float pVx, float pVy)
{
    m_oVelocity.x = pVx;
    m_oVelocity.y = pVy;
}

void Entity::Accelerate(sf::Vector2f pVelocity)
{
    m_oVelocity += pVelocity;
}

void Entity::Accelerate(float pVx, float pVy)
{
    m_oVelocity.x += pVx;
    m_oVelocity.y += pVy;
}

sf::Vector2f Entity::GetVelocity() const
{
    return m_oVelocity;
}

int Entity::GetHitpoints() const
{
    return m_iHitpoints;
}

void Entity::Repair(int pPoints)
{
    assert(pPoints > 0);

    m_iHitpoints += pPoints;
}

void Entity::Damage(int pPoints)
{
    assert(pPoints > 0);

    m_iHitpoints -= pPoints;
}

void Entity::Destroy()
{
    m_iHitpoints = 0;
}

bool Entity::IsDestroyed() const
{
    return m_iHitpoints <= 0;
}

void Entity::UpdateCurrent(sf::Time pDT, CommandQueue& pCommands)
{
    move(m_oVelocity * pDT.asSeconds());
}
