#include "Entity.hpp"

#include <iostream>

#include "Utility.hpp"

Entity::Entity(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	int hitpoints,
	float acceleration_speed,
	sf::Vector2f max_velocity,
	float deceleration,
	float gravity)
	: SceneNode(scene_layers)
	, m_hitpoints(hitpoints)
	, m_acceleration_speed(acceleration_speed)
	, m_max_velocity(max_velocity)
	, m_deceleration(deceleration)
	, m_gravity(gravity)
	, m_is_marked_for_removal(false)
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

void Entity::SetGravity(float gravity)
{
	m_gravity = gravity;
}

void Entity::SetVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
}

void Entity::AddVelocity(sf::Vector2f velocity)
{
	m_velocity += velocity;
}

void Entity::AddVelocity(float vx, float vy)
{
	m_velocity.x += vx;
	m_velocity.y += vy;
}

void Entity::AddDirection(sf::Vector2i direction)
{
	m_directions.emplace(direction);
	UpdateDirectionUnit();
}

void Entity::RemoveDirection(sf::Vector2i direction)
{
	m_directions.erase(direction);
	UpdateDirectionUnit();
}

sf::Vector2f Entity::GetVelocity() const
{
	return m_velocity;
}

void Entity::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (IsDestroyed())
	{
		m_is_marked_for_removal = true;
		return;
	}

	Accelerate(dt);
	Decelerate(dt);
	ApplyGravity(dt);

	ValidateVelocity();
	HandleCollisions();

	std::cout << m_velocity.x << ", " << m_velocity.y << std::endl;

	move(m_velocity);
}

void Entity::ApplyGravity(sf::Time dt) 
{
	m_velocity.y += m_gravity * dt.asSeconds();
}

void Entity::Accelerate(sf::Time dt)
{
	m_velocity += m_direction_unit * m_acceleration_speed * dt.asSeconds();
}

void Entity::Decelerate(sf::Time dt)
{
	const float deceleration = m_deceleration * dt.asSeconds();

	if (m_direction_unit.x == 0.f)
	{
		if (m_velocity.x > 0)
		{
			m_velocity.x = std::max(m_velocity.x - deceleration, 0.f);
		}
		else if (m_velocity.x < 0)
		{
			m_velocity.x = std::min(m_velocity.x + deceleration, 0.f);
		}
	}

	//if (m_direction_unit.y == 0.f)
	//{
	//	if (m_velocity.y > 0)
	//	{
	//		m_velocity.y = std::max(m_velocity.y - deceleration, 0.f);
	//	}
	//	else if (m_velocity.y < 0)
	//	{
	//		m_velocity.y = std::min(m_velocity.y + deceleration, 0.f);
	//	}
	//}
}

void Entity::ValidateVelocity()
{
	if (m_velocity.x > m_max_velocity.x)
	{
		m_velocity.x = m_max_velocity.x;
	}
	else if (m_velocity.x < -m_max_velocity.x)
	{
		m_velocity.x = -m_max_velocity.x;
	}

	if (m_velocity.y > m_max_velocity.y)
	{
		m_velocity.y = m_max_velocity.y;
	}
	else if (m_velocity.y < -m_max_velocity.y)
	{
		m_velocity.y = -m_max_velocity.y;
	}
}

void Entity::UpdateDirectionUnit()
{
	sf::Vector2i new_direction;

	for (const sf::Vector2i direction : m_directions)
	{
		new_direction += direction;
	}

	m_direction_unit = Utility::UnitVector(new_direction);
}

int Entity::GetHitPoints() const
{
	return m_hitpoints;
}

sf::Vector2f Entity::GetDirectionUnit() const
{
	return m_direction_unit;
}

sf::Vector2f Entity::GetMaxVelocity() const
{
	return m_max_velocity;
}

void Entity::Repair(unsigned int points)
{
	assert(points > 0);
	m_hitpoints += points;
}

void Entity::Damage(int points)
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
