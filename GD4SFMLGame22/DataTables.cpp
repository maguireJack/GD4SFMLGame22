#include "DataTables.hpp"

#include "PlatformerCharacterType.hpp"
#include "PlatformType.hpp"

std::vector<PlatformData> InitializePlatformData()
{
	std::vector<PlatformData> data(static_cast<int>(PlatformType::kPlatformCount));

	data[static_cast<int>(PlatformType::kStatic)].m_placetime = 1.f;
	data[static_cast<int>(PlatformType::kStatic)].m_breaktime = 0.f;
	data[static_cast<int>(PlatformType::kStatic)].m_moveDirection = sf::Vector2f(0, 0);
	data[static_cast<int>(PlatformType::kStatic)].m_textures = Textures::kTempTile;

	data[static_cast<int>(PlatformType::kBreakable)].m_placetime = 0.5f;
	data[static_cast<int>(PlatformType::kBreakable)].m_breaktime = 5.f;
	data[static_cast<int>(PlatformType::kBreakable)].m_moveDirection = sf::Vector2f(0, 0);

	data[static_cast<int>(PlatformType::kMoveable)].m_placetime = 2.f;
	data[static_cast<int>(PlatformType::kMoveable)].m_breaktime = 0.f;
	data[static_cast<int>(PlatformType::kMoveable)].m_moveDirection = sf::Vector2f(1, 0);

	return data;
}

std::vector<PlatformerCharacterData> InitializePlatformerCharacterData()
{
	std::vector<PlatformerCharacterData> data(static_cast<int>(PlatformerCharacterType::kPlatformerCount));
	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_jump_height = 10;
	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_acceleration = 1000;
	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_max_velocity = 60;
	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_deceleration = 1000;
	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_health = 100;

	data[static_cast<int>(PlatformerCharacterType::kBruno)].m_animation_data = PlatformerAnimationData{
		{Textures::kBrunoIdle, 21, 35, 12, 1},
		{Textures::kBrunoRun, 23, 34, 8, 1},
		{Textures::kBrunoIdle, 19, 34, 9, 1}
	};

	return data;
}