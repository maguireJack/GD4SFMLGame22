#include "Entity.hpp"

#include "Utility.hpp"

Entity::Entity(int hitpoints, float acceleration_speed, float max_velocity, float deceleration)
	: m_hitpoints(hitpoints)
	, m_acceleration_speed(acceleration_speed)
	, m_max_velocity(max_velocity)
	, m_deceleration(deceleration)
	, m_is_marked_for_removal(false)
{
}

void Entity::SetVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
	ValidateVelocity();
}

void Entity::SetVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
	ValidateVelocity();
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

	move(m_velocity * dt.asSeconds());
	Decelerate(dt);
	Accelerate(dt);
}

void Entity::Accelerate(sf::Time dt)
{
	m_velocity += m_direction_unit * m_acceleration_speed * dt.asSeconds();

	ValidateVelocity();
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

	if (m_direction_unit.y == 0.f)
	{
		if (m_velocity.y > 0)
		{
			m_velocity.y = std::max(m_velocity.y - deceleration, 0.f);
		}
		else if (m_velocity.y < 0)
		{
			m_velocity.y = std::min(m_velocity.y + deceleration, 0.f);
		}
	}
}

void Entity::ValidateVelocity()
{
	if (m_velocity.x > m_max_velocity)
	{
		m_velocity.x = m_max_velocity;
	}
	else if (m_velocity.x < -m_max_velocity)
	{
		m_velocity.x = -m_max_velocity;
	}

	if (m_velocity.y > m_max_velocity)
	{
		m_velocity.y = m_max_velocity;
	}
	else if (m_velocity.y < -m_max_velocity)
	{
		m_velocity.y = -m_max_velocity;
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

float Entity::GetMaxVelocity() const
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
