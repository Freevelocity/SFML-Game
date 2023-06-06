#pragma once


#include "SceneNode.h"
#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
	public:
		explicit SpriteNode(const sf::Texture& pTexture);
		SpriteNode(const sf::Texture& pTexture, const sf::IntRect& pTextureRect);


	private:
		virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


	private:
		sf::Sprite m_oSprite;
};