#include "TitleState.h"
#include "Utility.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    , m_oText()
    , m_bShowText(true)
    , m_oTextEffectTime(sf::Time::Zero)
{
    m_oBackgroundSprite.setTexture(pContext.m_oTextures->get(Textures::TitleScreen));

    m_oText.setFont(pContext.m_oFonts->get(Fonts::Main));
    m_oText.setString("Press any key to start");
    CenterOrigin(m_oText);
    m_oText.setPosition(pContext.m_oWindow->getView().getSize() / 2.f);
}

void TitleState::draw()
{
    sf::RenderWindow& oWindow = *GetContext().m_oWindow;
    oWindow.draw(m_oBackgroundSprite);

    if (m_bShowText)
        oWindow.draw(m_oText);
}

bool TitleState::update(sf::Time pDT)
{
    m_oTextEffectTime += pDT;

    if (m_oTextEffectTime >= sf::seconds(0.5f))
    {
        m_bShowText = !m_bShowText;
        m_oTextEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& pEvent)
{
    // If any key is pressed, trigger the next screen
    if (pEvent.type == sf::Event::KeyPressed)
    {
        RequestStackPop();
        RequestStackPush(States::Menu);
    }

    return true;
}
