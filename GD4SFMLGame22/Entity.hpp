#pragma once
#include <unordered_set>

#include "CommandQueue.hpp"
#include "SceneNode.hpp"
#include "Vector2iHash.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints = 1, float acceleration_speed = 1, float max_velocity = 1, float deceleration = 1);
	void SetVelocity(sf::Vector2f velocity);
	void SetVelocity(float vx, float vy);
	void AddDirection(sf::Vector2i direction);
	void RemoveDirection(sf::Vector2i direction);
	sf::Vector2f GetVelocity() const;

	int GetHitPoints() const;
	sf::Vector2f GetDirectionUnit() const;
	float GetMaxVelocity() const;
	void Repair(unsigned int points);
	void Damage(int points);
	void Destroy();
	virtual bool IsDestroyed() const override;

protected:
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	void Accelerate(sf::Time dt);
	void Decelerate(sf::Time dt);
	void ValidateVelocity();
	void UpdateDirectionUnit();

private:
	int m_hitpoints;
	float m_acceleration_speed;
	float m_max_velocity;
	float m_deceleration;
	sf::Vector2f m_velocity;
	std::unordered_set<sf::Vector2i, Vector2iHash> m_directions;
	sf::Vector2f m_direction_unit;

	bool m_is_marked_for_removal;
};
