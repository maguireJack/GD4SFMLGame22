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

std::vector<PlatformData> InitializePlatformData();