#pragma once

#include "SpriteHolder.h"
#include "SpriteIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Craft.h"
#include "CommandQueue.h"
#include "Command.h"
//#include "WorldView.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit World(sf::RenderWindow& pWindow, FontHolder& pFonts);
		void Update(sf::Time pDT);
		void Draw();
		CommandQueue& GetCommandQueue();
		bool HasAlivePlayer() const;
		bool HasPlayerReachedEnd() const;

	private:
		void LoadTextures();
		void BuildScene();
		void AdaptPlayerPosition();
		void AdaptPlayerVelocity();
		void HandleCollisions();

		void AddEnemies();
		void AddEnemy(Craft::Type pType, float pRelX, float pRelY);
		void SpawnEnemies();
		void DestroyEntitiesOutsideView();
		void GuideMissiles();
		sf::FloatRect GetViewBounds() const;
		sf::FloatRect GetBattlefieldBounds() const;


	private:
		enum Layer
		{
			Background,
			Air,
			LayerCount
		};

		struct SpawnPoint
		{
			SpawnPoint(Craft::Type pType, float pX, float pY)
				: m_oType(pType)
				, m_fX(pX)
				, m_fY(pY)
			{
			}

			Craft::Type m_oType;
			float m_fX;
			float m_fY;
		};


	private:
		sf::RenderWindow& m_oWindow;
		sf::View m_oWorldView;
		//WorldView m_oWorldView;
		TextureHolder m_oTextures;
		FontHolder& m_oFonts;

		SceneNode m_oSceneGraph;
		std::array<SceneNode*, LayerCount>	m_oSceneLayers;
		CommandQueue m_oCommandQueue;

		sf::FloatRect m_oWorldBounds;
		sf::Vector2f m_oSpawnPosition;
		
		float m_fScrollSpeed;
		Craft* m_oPlayerCraft;

		std::vector<SpawnPoint>	 m_oEnemySpawnPoints;
		std::vector<Craft*> m_oActiveEnemies;
};

