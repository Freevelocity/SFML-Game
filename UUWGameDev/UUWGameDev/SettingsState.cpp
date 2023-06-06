#include "SettingsState.h"
#include "Utility.h"
#include "SpriteHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& pStack, Context pContext)
    : State(pStack, pContext)
    , m_oGUIContainer()
{
	m_oBackgroundSprite.setTexture(pContext.m_oTextures->get(Textures::TitleScreen));

	// Build key binding buttons and labels
	/*AddButtonLabel(Player::MoveLeft, 150.f, "Move Left", pContext);
	AddButtonLabel(Player::MoveRight, 200.f, "Move Right", pContext);
	AddButtonLabel(Player::MoveUp, 250.f, "Move Up", pContext);
	AddButtonLabel(Player::MoveDown, 300.f, "Move Down", pContext);*/
	AddButtonLabel(Player::MoveLeft, 300.f, "Move Left", pContext);
	AddButtonLabel(Player::MoveRight, 350.f, "Move Right", pContext);
	AddButtonLabel(Player::MoveUp, 400.f, "Move Up", pContext);
	AddButtonLabel(Player::MoveDown, 450.f, "Move Down", pContext);
	AddButtonLabel(Player::Fire, 500.f, "Fire", pContext);
	AddButtonLabel(Player::LaunchMissile, 550.f, "Missile", pContext);

	UpdateLabels();

	/*auto backButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	backButton->setPosition(80.f, 375.f);
	backButton->SetText("Back");
	backButton->SetCallback(std::bind(&SettingsState::RequestStackPop, this));*/
	auto oBackButton = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	oBackButton->setPosition(80.f, 620.f);
	oBackButton->SetText("Back");
	oBackButton->SetCallback(std::bind(&SettingsState::RequestStackPop, this));


	m_oGUIContainer.Pack(oBackButton);

}

void SettingsState::draw()
{
	sf::RenderWindow& oWindow = *GetContext().m_oWindow;

	oWindow.draw(m_oBackgroundSprite);
	oWindow.draw(m_oGUIContainer);
}

bool SettingsState::update(sf::Time pDT)
{
    return true;
}

bool SettingsState::handleEvent(const sf::Event& pEvent)
{
	bool bIsKeyBinding = false;

	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t oAction = 0; oAction < Player::ActionCount; ++oAction)
	{
		if (m_oBindingButtons[oAction]->isActive())
		{
			bIsKeyBinding = true;
			if (pEvent.type == sf::Event::KeyReleased)
			{
				GetContext().m_oPlayer->AssignKey(static_cast<Player::Action>(oAction), pEvent.key.code);
				m_oBindingButtons[oAction]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (bIsKeyBinding)
		UpdateLabels();
	else
		m_oGUIContainer.handleEvent(pEvent);

	return false;
}

void SettingsState::UpdateLabels()
{
	Player& oPlayer = *GetContext().m_oPlayer;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = oPlayer.GetAssignedKey(static_cast<Player::Action>(i));
		m_oBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::AddButtonLabel(Player::Action pAction, float pY, const std::string& pText, Context pContext)
{
	m_oBindingButtons[pAction] = std::make_shared<GUI::Button>(*pContext.m_oFonts, *pContext.m_oTextures);
	m_oBindingButtons[pAction]->setPosition(80.f, pY);
	m_oBindingButtons[pAction]->SetText(pText);
	m_oBindingButtons[pAction]->SetToggle(true);

	m_oBindingLabels[pAction] = std::make_shared<GUI::Label>("", *pContext.m_oFonts);
	m_oBindingLabels[pAction]->setPosition(300.f, pY + 15.f);

	m_oGUIContainer.Pack(m_oBindingButtons[pAction]);
	m_oGUIContainer.Pack(m_oBindingLabels[pAction]);
}
