#include "PlatformerCharacter.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "DataTables.hpp"
#include "ResourceHolder.hpp"

namespace
{
	const std::vector<PlatformerCharacterData> Table = InitializePlatformerCharacterData();
}


PlatformerCharacter::PlatformerCharacter(PlatformerCharacterType type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[static_cast<int>(type)].m_health, Table[static_cast<int>(type)].m_speed)
	, m_type(type)
	, m_artist(Table[static_cast<int>(type)].m_animation_data.ToVector(), textures)
	, m_health_display(nullptr)
{
	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	m_health_display = healthDisplay.get();
	AttachChild(std::move(healthDisplay));
}

unsigned PlatformerCharacter::GetCategory() const
{
	return Category::kPlayerCharacter;
}

void PlatformerCharacter::UpdateTexts() const
{
	m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());
}

sf::FloatRect PlatformerCharacter::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_artist.GetBoundingRect());
}

void PlatformerCharacter::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_artist.DrawCurrent(target, states);
}

void PlatformerCharacter::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::UpdateCurrent(dt, commands);
	m_artist.UpdateCurrent(dt);

	UpdateTexts();
}