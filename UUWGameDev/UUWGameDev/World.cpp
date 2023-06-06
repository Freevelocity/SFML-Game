#include "World.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Foreach.h"
#include "TextNode.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderWindow& pWindow, FontHolder& pFonts)
	: m_oWindow(pWindow)
	, m_oWorldView(pWindow.getDefaultView())
	, m_oFonts(pFonts)
	, m_oTextures()
	, m_oSceneGraph()
	, m_oSceneLayers()
	, m_oWorldBounds(0.f, 0.f, m_oWorldView.getSize().x, 2000.f)
	, m_oSpawnPosition(m_oWorldView.getSize().x / 2.f, m_oWorldBounds.height - m_oWorldView.getSize().y / 2.f)
	, m_fScrollSpeed(-50.f)
	, m_oPlayerCraft(nullptr)
	, m_oEnemySpawnPoints()
	, m_oActiveEnemies()
{
	LoadTextures();
	BuildScene();

	// Prepare the view
	m_oWorldView.setCenter(m_oSpawnPosition);
}

void World::Update(sf::Time pDT)
{
	// Scroll the world, reset player velocity
	m_oWorldView.move(0.f, m_fScrollSpeed * pDT.asSeconds());
	m_oPlayerCraft->SetVelocity(0.f, 0.f);

	DestroyEntitiesOutsideView();
	GuideMissiles();

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!m_oCommandQueue.IsEmpty())
		m_oSceneGraph.OnCommand(m_oCommandQueue.Pop(), pDT);
	AdaptPlayerVelocity();

	HandleCollisions();
	// Regular update step, adapt position (correct if outside view)
	//m_oSceneGraph.Update(pDT);
	m_oSceneGraph.RemoveWrecks();
	SpawnEnemies();
	
	m_oSceneGraph.Update(pDT, m_oCommandQueue);
	AdaptPlayerPosition();
	//// Move the player sidewards 
	//sf::Vector2f position = m_oPlayerCraft->getPosition();
	//sf::Vector2f velocity = m_oPlayerCraft->GetVelocity();

	//// If player touches borders, flip its X velocity
	//if (position.x <= m_oWorldBounds.left + 150.f
	//	|| position.x >= m_oWorldBounds.left + m_oWorldBounds.width - 150.f)
	//{
	//	velocity.x = -velocity.x;
	//	m_oPlayerCraft->SetVelocity(velocity);
	//}

	//// Apply movements
	//m_oSceneGraph.Update(pDT);
}

void World::Draw()
{
	m_oWindow.setView(m_oWorldView);
	m_oWindow.draw(m_oSceneGraph);
}

CommandQueue& World::GetCommandQueue()
{
	return m_oCommandQueue;
}

bool World::HasAlivePlayer() const
{
	return !m_oPlayerCraft->IsMarkedForRemoval();
}

bool World::HasPlayerReachedEnd() const
{
	return !m_oWorldBounds.contains(m_oPlayerCraft->getPosition());
}

void World::LoadTextures()
{
	m_oTextures.SLoad(Textures::PassengerJet, "graphics/PassengerJet.png");
	m_oTextures.SLoad(Textures::MilitaryJet, "graphics/MilitaryJet.png");
	m_oTextures.SLoad(Textures::EnemyJet, "graphics/EnemyJet.png");
	m_oTextures.SLoad(Textures::Desert, "graphics/Desert.png");

	m_oTextures.SLoad(Textures::Bullet, "graphics/Bullet.png");
	m_oTextures.SLoad(Textures::Missile, "graphics/Missile.png");

	m_oTextures.SLoad(Textures::HealthRefill, "graphics/HealthRefill.png");
	m_oTextures.SLoad(Textures::MissileRefill, "graphics/MissileRefill.png");
	m_oTextures.SLoad(Textures::FireSpread, "graphics/FireSpread.png");
	m_oTextures.SLoad(Textures::FireRate, "graphics/FireRate.png");
}

void World::BuildScene()
{
	
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		PlayerType::Type oType= (i == Air) ? PlayerType::Scene : PlayerType::None;
		SceneNode::Ptr oLayer(new SceneNode(oType));
		m_oSceneLayers[i] = oLayer.get();

		m_oSceneGraph.AttachChild(std::move(oLayer));
	}

	
	sf::Texture& oTexture = m_oTextures.get(Textures::Desert);
	sf::IntRect oTextureRect(m_oWorldBounds);
	oTexture.setRepeated(true);

	
	std::unique_ptr<SpriteNode> oBackgroundSprite(new SpriteNode(oTexture, oTextureRect));
	oBackgroundSprite->setPosition(m_oWorldBounds.left, m_oWorldBounds.top);
	m_oSceneLayers[Background]->AttachChild(std::move(oBackgroundSprite));


	std::unique_ptr<Craft> oPlayer(new Craft(Craft::PassengerJet, m_oTextures, m_oFonts));
	m_oPlayerCraft = oPlayer.get();
	m_oPlayerCraft->setPosition(m_oSpawnPosition);
	 //m_oPlayerCraft->SetVelocity(40.f, m_fScrollSpeed);
	m_oSceneLayers[Air]->AttachChild(std::move(oPlayer));

	AddEnemies();
	/*std::unique_ptr<Craft> oLeftEscort(new Craft(Craft::MilitaryJet, m_oTextures));
	oLeftEscort->setPosition(-80.f, 50.f);
	m_oPlayerCraft->AttachChild(std::move(oLeftEscort));

	std::unique_ptr<Craft> oRightEscort(new Craft(Craft::MilitaryJet, m_oTextures));
	oRightEscort->setPosition(80.f, 50.f);
	m_oPlayerCraft->AttachChild(std::move(oRightEscort));*/
}

void World::AddEnemies()
{
	// Add enemies to the spawn point container
	AddEnemy(Craft::MilitaryJet, 0.f, 500.f);
	AddEnemy(Craft::MilitaryJet, 0.f, 1000.f);
	AddEnemy(Craft::MilitaryJet, +100.f, 1100.f);
	AddEnemy(Craft::MilitaryJet, -100.f, 1100.f);
	AddEnemy(Craft::EnemyJet1, -70.f, 1400.f);
	AddEnemy(Craft::EnemyJet1, -70.f, 1600.f);
	AddEnemy(Craft::EnemyJet1, 70.f, 1400.f);
	AddEnemy(Craft::EnemyJet1, 70.f, 1600.f);

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(m_oEnemySpawnPoints.begin(), m_oEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.m_fY < rhs.m_fY;
		});
}

void World::AddEnemy(Craft::Type pType, float pRelX, float pRelY)
{
	SpawnPoint oSpawn(pType, m_oSpawnPosition.x + pRelX, m_oSpawnPosition.y - pRelY);
	m_oEnemySpawnPoints.push_back(oSpawn);
}

void World::SpawnEnemies()
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!m_oEnemySpawnPoints.empty()
		&& m_oEnemySpawnPoints.back().m_fY > GetBattlefieldBounds().top)
	{
		SpawnPoint oSpawn = m_oEnemySpawnPoints.back();

		std::unique_ptr<Craft> oEnemy(new Craft(oSpawn.m_oType, m_oTextures, m_oFonts));
		oEnemy->setPosition(oSpawn.m_fX, oSpawn.m_fY);
		oEnemy->setRotation(180.f);

		m_oSceneLayers[Air]->AttachChild(std::move(oEnemy));

		// Enemy is spawned, remove from the list to spawn
		m_oEnemySpawnPoints.pop_back();
	}
}

void World::DestroyEntitiesOutsideView()
{
	Command oCommand;
	oCommand.m_iPlayerType = PlayerType::Projectile | PlayerType::EnemyCraft;
	oCommand.m_oAction = derivedAction<Entity>([this](Entity& pEntity, sf::Time)
		{
			if (!GetBattlefieldBounds().intersects(pEntity.GetBoundingRect()))
				pEntity.Destroy();
		});

	m_oCommandQueue.Push(oCommand);
}

void World::GuideMissiles()
{
	// Setup command that stores all enemies in mActiveEnemies
	Command oEnemyCollector;
	oEnemyCollector.m_iPlayerType = PlayerType::EnemyCraft;
	oEnemyCollector.m_oAction = derivedAction<Craft>([this](Craft& pEnemy, sf::Time)
		{
			if (!pEnemy.IsDestroyed())
				m_oActiveEnemies.push_back(&pEnemy);
		});

	// Setup command that guides all missiles to the enemy which is currently closest to the player
	Command oMissileGuider;
	oMissileGuider.m_iPlayerType = PlayerType::AlliedProjectile;
	oMissileGuider.m_oAction = derivedAction<Projectile>([this](Projectile& pMissile, sf::Time)
		{
			// Ignore unguided bullets
			if (!pMissile.IsGuided())
				return;

			float fMinDistance = std::numeric_limits<float>::max();
			Craft* oClosestEnemy = nullptr;

			// Find closest enemy
			FOREACH(Craft * oEnemy, m_oActiveEnemies)
			{
				float fEnemyDistance = Distance(pMissile, *oEnemy);

				if (fEnemyDistance < fMinDistance)
				{
					oClosestEnemy = oEnemy;
					fMinDistance = fEnemyDistance;
				}
			}

			if (oClosestEnemy)
				pMissile.GuideTowards(oClosestEnemy->GetWorldPosition());
		});

	// Push commands, reset active enemies
	m_oCommandQueue.Push(oEnemyCollector);
	m_oCommandQueue.Push(oMissileGuider);
	m_oActiveEnemies.clear();
}



void World::AdaptPlayerPosition()
{
	
	/*sf::FloatRect viewBounds(m_oWorldView.getCenter() - m_oWorldView.getSize() / 2.f, m_oWorldView.getSize());
	const float borderDistance = 40.f;*/
	sf::FloatRect oViewBounds = GetViewBounds();
	const float fBorderDistance = 40.f;

	sf::Vector2f oPosition = m_oPlayerCraft->getPosition();
	oPosition.x = std::max(oPosition.x, oViewBounds.left + fBorderDistance);
	oPosition.x = std::min(oPosition.x, oViewBounds.left + oViewBounds.width - fBorderDistance);
	oPosition.y = std::max(oPosition.y, oViewBounds.top + fBorderDistance);
	oPosition.y = std::min(oPosition.y, oViewBounds.top + oViewBounds.height - fBorderDistance);
	m_oPlayerCraft->setPosition(oPosition);
}

void World::AdaptPlayerVelocity()
{
	sf::Vector2f velocity = m_oPlayerCraft->GetVelocity();
	if (velocity.x != 0.f && velocity.y != 0.f)
		m_oPlayerCraft->SetVelocity(velocity / std::sqrt(2.f));
	m_oPlayerCraft->Accelerate(0.f, m_fScrollSpeed);
}

bool MatchesCategories(SceneNode::Pair& pColliders, PlayerType::Type pType1, PlayerType::Type pType2)
{
	unsigned int iType1 = pColliders.first->GetPlayerType();
	unsigned int iType2 = pColliders.second->GetPlayerType();

	// Make sure first pair entry has category type1 and second has type2
	if (pType1 & iType1 && pType2 & iType2)
	{
		return true;
	}
	else if (pType1 & iType2 && pType2 & iType1)
	{
		std::swap(pColliders.first, pColliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::HandleCollisions()
{
	std::set<SceneNode::Pair> oCollisionPairs;
	m_oSceneGraph.CheckSceneCollision(m_oSceneGraph, oCollisionPairs);

	FOREACH(SceneNode::Pair oPair, oCollisionPairs)
	{
		if (MatchesCategories(oPair, PlayerType::PlayerCraft, PlayerType::EnemyCraft))
		{
			auto& oPlayer = static_cast<Craft&>(*oPair.first);
			auto& oEnemy = static_cast<Craft&>(*oPair.second);

			// Collision: Player damage = enemy's remaining HP
			oPlayer.Damage(oEnemy.GetHitpoints());
			oEnemy.Destroy();
		}

		else if (MatchesCategories(oPair, PlayerType::PlayerCraft, PlayerType::Pickup))
		{
			auto& oPlayer = static_cast<Craft&>(*oPair.first);
			auto& oPickup = static_cast<Pickup&>(*oPair.second);

			// Apply pickup effect to player, destroy projectile
			oPickup.Apply(oPlayer);
			oPickup.Destroy();
		}

		else if (MatchesCategories(oPair, PlayerType::EnemyCraft, PlayerType::AlliedProjectile)
			|| MatchesCategories(oPair, PlayerType::PlayerCraft, PlayerType::EnemyProjectile))
		{
			auto& oCraft = static_cast<Craft&>(*oPair.first);
			auto& oProjectile = static_cast<Projectile&>(*oPair.second);

			// Apply projectile damage to aircraft, destroy projectile
			oCraft.Damage(oProjectile.GetDamage());
			oProjectile.Destroy();
		}
	}
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(m_oWorldView.getCenter() - m_oWorldView.getSize() / 2.f, m_oWorldView.getSize());
}

sf::FloatRect World::GetBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect oBounds = GetViewBounds();
	oBounds.top -= 100.f;
	oBounds.height += 100.f;

	return oBounds;
}
