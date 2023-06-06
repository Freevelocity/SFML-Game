#pragma once
#include "Component.h"
#include "SpriteIdentifiers.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{

    class Button : public Component
    {
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;


    public:
        Button(const FontHolder& pFonts, const TextureHolder& pTextures);

        void SetCallback(Callback pCallback);
        void SetText(const std::string& pText);
        void SetToggle(bool pFlag);

        virtual bool isSelectable() const;
        virtual void select();
        virtual void deselect();

        virtual void activate();
        virtual void deactivate();

        virtual void handleEvent(const sf::Event& pEvent);


    private:
        virtual void draw(sf::RenderTarget& pTarget, sf::RenderStates pStates) const;


    private:
        Callback m_oCallback;
        const sf::Texture& m_oNormalTexture;
        const sf::Texture& m_oSelectedTexture;
        const sf::Texture& m_oPressedTexture;
        sf::Sprite m_oSprite;
        sf::Text m_oText;
        bool m_bIsToggle;
    };

}


