#pragma once
#include "CommandQueue.hpp"
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints);
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	void Accelerate(sf::Vector2f velocity);
	void Accelerate(float vx, float vy);
	sf::Vector2f GetVelocity() const;

	int GetHitPoints() const;
	void Repair(unsigned int points);
	void Damage(unsigned int points);
	void Destroy();
	virtual bool IsDestroyed() const override;

protected:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f m_velocity;
	unsigned int m_hitpoints;
};