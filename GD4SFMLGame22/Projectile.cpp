#include "Projectile.hpp"

#include <valarray>
#include <SFML/Graphics/RenderTarget.hpp>

#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

namespace
{
	const std::vector<ProjectileData> Table = InitializeProjectileData();
}

Projectile::Projectile(ProjectileType type, const TextureHolder& textures)
: Entity(1)
, m_type(type)
, m_sprite(textures.Get(Table[static_cast<int>(type)].m_texture))
{
	Utility::CentreOrigin(m_sprite);
}

void Projectile::GuideTowards(sf::Vector2f position)
{
	assert(IsGuided());
	m_target_direction = Utility::UnitVector(position - GetWorldPosition());
}

bool Projectile::IsGuided() const
{
	return m_type == ProjectileType::kMissile;
}

unsigned int Projectile::GetCategory() const
{
	if (m_type == ProjectileType::kEnemyBullet)
		return static_cast<int>(Category::kEnemyProjectile);
	else
		return static_cast<int>(Category::kAlliedProjectile);
}

//Axis aligned bounding box
sf::FloatRect Projectile::GetBoundingRect()
{
	return GetWorldTransform().transformRect((m_sprite.getGlobalBounds()));
}

float Projectile::GetMaxSpeed() const
{
	return Table[static_cast<int>(m_type)].m_speed;
}

int Projectile::GetDamage() const
{
	return Table[static_cast<int>(m_type)].m_damage;
}

void Projectile::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if(IsGuided())
	{
		//Increase approach rate to move in the direction of the target faster
		const float approach_rate = 200.f;
		sf::Vector2f new_velocity = Utility::UnitVector(approach_rate * dt.asSeconds() * m_target_direction + GetVelocity());
		new_velocity *= GetMaxSpeed();
		float angle = std::atan2(new_velocity.y, new_velocity.x);
		setRotation(Utility::ToDegrees(angle) + 90.f);
		SetVelocity(new_velocity);
	}
	Entity::UpdateCurrent(dt, commands);
}

void Projectile::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
