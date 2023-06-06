#include "Button.h"
#include "Utility.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

    Button::Button(const FontHolder& pFonts, const TextureHolder& pTextures)
        : m_oCallback()
        , m_oNormalTexture(pTextures.get(Textures::ButtonNormal))
        , m_oSelectedTexture(pTextures.get(Textures::ButtonSelected))
        , m_oPressedTexture(pTextures.get(Textures::ButtonPressed))
        , m_oSprite()
        , m_oText("", pFonts.get(Fonts::Main), 16)
        , m_bIsToggle(false)
    {
        m_oSprite.setTexture(m_oNormalTexture);

        sf::FloatRect bounds = m_oSprite.getLocalBounds();
        m_oText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::SetText(const std::string& pText)
    {
        m_oText.setString(pText);
        CenterOrigin(m_oText);
    }

    void Button::SetToggle(bool pFlag)
    {
        m_bIsToggle = pFlag;
    }

    bool Button::isSelectable() const
    {
        return true;
    }

    void Button::select()
    {
        Component::select();

        m_oSprite.setTexture(m_oSelectedTexture);
    }

    void Button::deselect()
    {
        Component::deselect();

        m_oSprite.setTexture(m_oNormalTexture);
    }

    void Button::activate()
    {
        Component::activate();

        // If we are toggle then we should show that the button is pressed and thus "toggled".
        if (m_bIsToggle)
            m_oSprite.setTexture(m_oPressedTexture);

        if (m_oCallback)
            m_oCallback();

        // If we are not a toggle then deactivate the button since we are just momentarily activated.
        if (!m_bIsToggle)
            deactivate();
    }

    void Button::deactivate()
    {
        Component::deactivate();

        if (m_bIsToggle)
        {
            // Reset texture to right one depending on if we are selected or not.
            if (isSelected())
                m_oSprite.setTexture(m_oSelectedTexture);
            else
                m_oSprite.setTexture(m_oNormalTexture);
        }
    }

    void Button::handleEvent(const sf::Event& pEvent)
    {
    }

    void Button::draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const
    {
        pStates.transform *= getTransform();
        pTarget.draw(m_oSprite, pStates);
        pTarget.draw(m_oText, pStates);
    }

    void Button::SetCallback(Callback pCallback)
    {
        m_oCallback = std::move(pCallback);
    }

}