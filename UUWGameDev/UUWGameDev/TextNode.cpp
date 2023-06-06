#include "TextNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& pFonts, const std::string& pText)
{
	m_oText.setFont(pFonts.get(Fonts::Main));
	m_oText.setCharacterSize(20);
	SetString(pText);
}

void TextNode::SetString(const std::string& pText)
{
	m_oText.setString(pText);
	CenterOrigin(m_oText);
}

void TextNode::DrawCurrent(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
{
	pTarget.draw(m_oText, pStates);
}
