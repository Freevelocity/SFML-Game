#pragma once
#include "Component.h"
#include "SpriteIdentifiers.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/Text.hpp>


namespace GUI
{

    class Label : public Component
    {
    public:
        typedef std::shared_ptr<Label> Ptr;


    public:
        Label(const std::string& pText, const FontHolder& pFonts);
        virtual bool isSelectable() const;
        void setText(const std::string& pText);
        virtual void handleEvent(const sf::Event& pEvent);


    private:
        void draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


    private:
        sf::Text m_oText;
    };

}
