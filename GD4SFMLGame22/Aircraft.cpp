#include "Aircraft.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "ResourceHolder.hpp"
#include "Utility.hpp"

namespace
{
	const std::vector<AircraftData> Table = InitializeAircraftData();
}


Textures ToTextureID(AircraftType type)
{
	switch(type)
	{
	case AircraftType::kEagle:
		return Textures::kEagle;
	case AircraftType::kRaptor:
		return Textures::kRaptor;
	}
	return Textures::kEagle;
}

Aircraft::Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[static_cast<int>(type)].m_hitpoints)
, m_type(type)
, m_sprite(textures.Get(ToTextureID(type)))
, m_fire_rate(1)
, m_spread_level(1)
, m_missile_ammo(2)
, m_health_display(nullptr)
, m_missile_display(nullptr)
, m_travelled_distance(0.f)
, m_directions_index(0)
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	std::string empty_string;

	std::unique_ptr<TextNode> health_display(new TextNode(fonts, empty_string));
	m_health_display = health_display.get();
	AttachChild(std::move(health_display));

	if(GetCategory() == Category::kPlayerAircraft)
	{
		std::unique_ptr<TextNode> missile_display(new TextNode(fonts, empty_string));
		missile_display->setPosition(0, 70);
		m_missile_display = missile_display.get();
		AttachChild(std::move(missile_display));
	}

	UpdateTexts();

}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

unsigned Aircraft::GetCategory() const
{
	switch(m_type)
	{
	case AircraftType::kEagle:
		return Category::kPlayerAircraft;
	default:
		return Category::kEnemyAircraft;

	}
}

void Aircraft::IncreaseFireRate()
{
	if(m_fire_rate < 10)
	{
		++m_fire_rate;
	}
}

void Aircraft::IncreaseSpread()
{
	if(m_spread_level < 3)
	{
		++m_spread_level;
	}
}

void Aircraft::CollectMissiles(unsigned count)
{
	m_missile_ammo += count;
}

void Aircraft::UpdateTexts()
{
	m_health_display->SetString(GetHitPoints() + "HP");
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());

	if(m_missile_display)
	{
		if(m_missile_ammo == 0)
		{
			m_missile_display->SetString("");
		}
		else
		{
			m_missile_display->SetString("M: " + m_missile_ammo);
		}
	}

}

void Aircraft::UpdateCurrent(sf::Time dt)
{
	UpdateTexts();
}

void Aircraft::UpdateMovementPattern(sf::Time dt)
{
	//Enemy AI
	const std::vector<Direction>& directions = Table[static_cast<int>(m_type)].m_directions;
	if(!directions.empty())
	{
		//Move along the current direction, change direction
		if(m_travelled_distance > directions[m_directions_index].m_distance)
		{
			m_directions_index = (m_directions_index + 1) % directions.size();
			m_travelled_distance = 0.f;
		}

		//Compute velocity from direction
		double radians = Utility::ToRadians(directions[m_directions_index].m_angle + 90.f);
		float vx = GetMaxSpeed() * std::cos(radians);
		float vy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(vx, vy);
		m_travelled_distance += GetMaxSpeed() * dt.asSeconds();

	}
}

