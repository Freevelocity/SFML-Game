#include "SpriteNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture& pTexture)
:m_oSprite(pTexture)
{
}

SpriteNode::SpriteNode(const sf::Texture& pTexture, const sf::IntRect& pTextureRect)
: m_oSprite(pTexture, pTextureRect)
{
}

void SpriteNode::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	pTarget.draw(m_oSprite, pStates);
}
