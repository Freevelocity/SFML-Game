#pragma once


#include "SceneNode.h"


class Entity : public SceneNode
{
	public:
		explicit Entity(int pHitpoints);
		void SetVelocity(sf::Vector2f pVelocity);
		void SetVelocity(float pVx, float pVy);
		void Accelerate(sf::Vector2f pVelocity);
		void Accelerate(float pVx, float pVy);
		sf::Vector2f GetVelocity() const;
		int	GetHitpoints() const;
		void Repair(int pPoints);
		void Damage(int pPoints);
		void Destroy();
		virtual bool IsDestroyed() const;


	protected:
		virtual void UpdateCurrent(sf::Time pDT, CommandQueue& pCommands);


	private:
		sf::Vector2f m_oVelocity;
		int m_iHitpoints;
};
