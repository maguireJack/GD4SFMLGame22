#include "Entity.hpp"

Entity::Entity(int hitpoints)
	: m_hitpoints(hitpoints)
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

void Entity::SetVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
}

sf::Vector2f Entity::GetVelocity() const
{
	return m_velocity;
}

void Entity::Accelerate(sf::Vector2f velocity)
{
	m_velocity += velocity;
}

void Entity::Accelerate(float vx, float vy)
{
	m_velocity.x += vx;
	m_velocity.y += vy;
}

void Entity::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	move(m_velocity * dt.asSeconds());
}

int Entity::GetHitPoints() const
{
	return m_hitpoints;
}

void Entity::Repair(unsigned int points)
{
	assert(points > 0);
	m_hitpoints += points;
}

void Entity::Damage(unsigned int points)
{
	assert(points > 0);
	m_hitpoints -= points;
}

void Entity::Destroy()
{
	m_hitpoints = 0;
}

bool Entity::IsDestroyed() const
{
	return m_hitpoints <= 0;
}
