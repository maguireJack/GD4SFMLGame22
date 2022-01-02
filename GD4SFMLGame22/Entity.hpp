#pragma once
#include "CommandQueue.hpp"
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints = 1, float max_speed = 1);
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	void Accelerate(sf::Vector2f velocity);
	void Accelerate(float vx, float vy);
	sf::Vector2f GetVelocity() const;

	int GetHitPoints() const;
	float GetMaxSpeed() const;
	void Repair(unsigned int points);
	void Damage(int points);
	void Destroy();
	virtual bool IsDestroyed() const override;

protected:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);

private:
	int m_hitpoints;
	float m_max_speed;
	sf::Vector2f m_velocity;
};