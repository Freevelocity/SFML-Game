#pragma once


namespace PlayerType
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerCraft = 1 << 1,
		AlliedCraft = 1 << 2,
		EnemyCraft = 1 << 3,
		Pickup = 1 << 4,
		AlliedProjectile = 1 << 5,
		EnemyProjectile = 1 << 6,

		Craft = PlayerCraft | AlliedCraft | EnemyCraft,
		Projectile = AlliedProjectile | EnemyProjectile,
	};
}