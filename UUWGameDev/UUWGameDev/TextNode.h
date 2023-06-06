#pragma once

#include "SpriteHolder.h"
#include "SpriteIdentifiers.h"
#include "SceneNode.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder& pFonts, const std::string& pText);

	void SetString(const std::string& pText);


private:
	virtual void DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


private:
	sf::Text m_oText;
};

