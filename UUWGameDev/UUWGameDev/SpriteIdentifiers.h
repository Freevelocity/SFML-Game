#pragma once


namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		PassengerJet,
		MilitaryJet,
		EnemyJet,
		Bullet,
		Missile,
		Desert,
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TitleScreen,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

template <typename Resource, typename Identifier>
class SpriteHolder;

typedef SpriteHolder<sf::Texture, Textures::ID> TextureHolder;
typedef SpriteHolder<sf::Font, Fonts::ID> FontHolder;

