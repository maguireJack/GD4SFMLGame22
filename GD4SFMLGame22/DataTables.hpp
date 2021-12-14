#pragma once
#include <functional>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
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