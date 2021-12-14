#include "DataTables.hpp"

#include "PlatformType.hpp"

std::vector<PlatformData> InitializePlatformData()
{
	std::vector<PlatformData> data(static_cast<int>(PlatformType::kPlatformCount));

	data[static_cast<int>(PlatformType::kStatic)].m_placetime = 1.f;
	data[static_cast<int>(PlatformType::kStatic)].m_breaktime = 0.f;
	data[static_cast<int>(PlatformType::kStatic)].m_moveDirection = sf::Vector2f(0,0);
	data[static_cast<int>(PlatformType::kStatic)].m_textures = Textures::kTempTile;

	data[static_cast<int>(PlatformType::kBreakable)].m_placetime = 0.5f;
	data[static_cast<int>(PlatformType::kBreakable)].m_breaktime = 5.f;
	data[static_cast<int>(PlatformType::kBreakable)].m_moveDirection = sf::Vector2f(0, 0);

	data[static_cast<int>(PlatformType::kMoveable)].m_placetime = 2.f;
	data[static_cast<int>(PlatformType::kMoveable)].m_breaktime = 0.f;
	data[static_cast<int>(PlatformType::kMoveable)].m_moveDirection = sf::Vector2f(1, 0);

	return data;
}
