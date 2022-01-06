#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "ResourceIdentifiers.hpp"

struct PlatformData
{
	sf::Vector2f m_moveDirection;
	Textures m_textures;
	float m_breaktime;
	float m_placetime;
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
	float m_jump_height;
	float m_acceleration;
	sf::Vector2f m_max_velocity;
	float m_deceleration;
	float m_gravity;
	float m_coyote_time;
	int m_health;
	PlatformerAnimationData m_animation_data;
};

std::vector<PlatformData> InitializePlatformData();
std::vector<PlatformerCharacterData> InitializePlatformerCharacterData();