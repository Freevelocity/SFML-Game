#include "Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{


    Label::Label(const std::string& pText, const FontHolder& pFonts)
        : m_oText(pText, pFonts.get(Fonts::Main), 16)
    {
    }

    bool Label::isSelectable() const
    {
        return false;
    }

    void Label::setText(const std::string& pText)
    {
        m_oText.setString(pText);
    }

    void Label::handleEvent(const sf::Event& pEvent)
    {

    }

    void Label::draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
    {
        pStates.transform *= getTransform();
        pTarget.draw(m_oText, pStates);
    }
}