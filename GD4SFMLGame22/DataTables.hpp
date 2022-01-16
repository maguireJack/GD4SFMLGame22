#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "PlatformEffects.hpp"
#include "ResourceIdentifiers.hpp"

struct PlatformData
{
	sf::Vector2i m_cell_size;
	sf::Vector2i m_offset;
	std::unordered_set<PlatformEffects> m_effects;
};

struct AnimationData
{
	Textures m_texture;
	int m_width_per_frame;
	int m_height_per_frame;
	int m_frames;
	float m_time_per_frame;
};

struct PlatformerAnimationData
{
	AnimationData m_idle;
	AnimationData m_run;
	AnimationData m_jump;

	std::vector<AnimationData> ToVector() const
	{
		return { m_idle, m_run, m_jump };
	}
};

struct PlatformerCharacterData
{
	float m_jump_force;
	float m_acceleration;
	sf::Vector2f m_max_velocity;
	float m_deceleration;
	float m_gravity;
	float m_coyote_time;
	int m_health;
	PlatformerAnimationData m_animation_data;
};

struct ParticleData
{
	sf::Color m_color;
	sf::Time m_lifetime;
};

std::unordered_map<Textures, PlatformData> InitializePlatformData();
std::vector<PlatformerCharacterData> InitializePlatformerCharacterData();
std::vector<ParticleData> InitializeParticleData();