#include "DBTables.h"
#include "Craft.h"
#include "Projectile.h"
#include "Pickup.h"

using namespace std::placeholders;

std::vector<CraftData> initializeCraftData()
{
	std::vector<CraftData> data(Craft::TypeCount);

	data[Craft::PassengerJet].iHitpoints = 100;
	data[Craft::PassengerJet].fSpeed = 200.f;
	data[Craft::PassengerJet].oFireInterval = sf::seconds(1);
	data[Craft::PassengerJet].oTexture = Textures::PassengerJet;

	data[Craft::MilitaryJet].iHitpoints = 20;
	data[Craft::MilitaryJet].fSpeed = 80.f;
	data[Craft::MilitaryJet].oTexture = Textures::MilitaryJet;
	data[Craft::MilitaryJet].oDirections.push_back(Direction(+45.f, 80.f));
	data[Craft::MilitaryJet].oDirections.push_back(Direction(-45.f, 160.f));
	data[Craft::MilitaryJet].oDirections.push_back(Direction(+45.f, 80.f));
	data[Craft::MilitaryJet].oFireInterval = sf::Time::Zero;

	data[Craft::EnemyJet1].iHitpoints = 40;
	data[Craft::EnemyJet1].fSpeed = 50.f;
	data[Craft::EnemyJet1].oTexture = Textures::EnemyJet;
	data[Craft::EnemyJet1].oDirections.push_back(Direction(+45.f, 50.f));
	data[Craft::EnemyJet1].oDirections.push_back(Direction(0.f, 50.f));
	data[Craft::EnemyJet1].oDirections.push_back(Direction(-45.f, 100.f));
	data[Craft::EnemyJet1].oDirections.push_back(Direction(0.f, 50.f));
	data[Craft::EnemyJet1].oDirections.push_back(Direction(+45.f, 50.f));
	data[Craft::EnemyJet1].oFireInterval = sf::seconds(2);

	return data;
}


std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> oData(Projectile::TypeCount);

	oData[Projectile::AlliedBullet].iDamage = 10;
	oData[Projectile::AlliedBullet].fSpeed = 300.f;
	oData[Projectile::AlliedBullet].oTexture = Textures::Bullet;

	oData[Projectile::EnemyBullet].iDamage = 10;
	oData[Projectile::EnemyBullet].fSpeed = 300.f;
	oData[Projectile::EnemyBullet].oTexture = Textures::Bullet;

	oData[Projectile::Missile].iDamage = 200;
	oData[Projectile::Missile].fSpeed = 150.f;
	oData[Projectile::Missile].oTexture = Textures::Missile;

	return oData;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> oData(Pickup::TypeCount);

	oData[Pickup::HealthRefill].oTexture = Textures::HealthRefill;
	oData[Pickup::HealthRefill].oAction = [](Craft& pCraft) { pCraft.Repair(25); };

	oData[Pickup::MissileRefill].oTexture = Textures::MissileRefill;
	oData[Pickup::MissileRefill].oAction = std::bind(&Craft::CollectMissiles, _1, 3);

	oData[Pickup::FireSpread].oTexture = Textures::FireSpread;
	oData[Pickup::FireSpread].oAction = std::bind(&Craft::IncreaseSpread, _1);

	oData[Pickup::FireRate].oTexture = Textures::FireRate;
	oData[Pickup::FireRate].oAction = std::bind(&Craft::IncreaseFireRate, _1);

	return oData;
}
