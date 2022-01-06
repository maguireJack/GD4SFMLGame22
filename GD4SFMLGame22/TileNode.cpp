#include "TileNode.hpp"

#include <iostream>
#include <ostream>

#include "Application.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"

namespace
{
	const std::vector<PlatformData> Table = InitializePlatformData();
}

TileNode::TileNode(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers, 
	PlatformType platform,
	const TextureHolder& textures)
	: SceneNode(scene_layers)
	, m_platform(platform)
	, m_selected(false)
	, m_sprite(textures.Get(Table[static_cast<int>(platform)].m_textures))
{
}

unsigned TileNode::GetCategory() const
{
	return Category::kPlatform;
}

sf::FloatRect TileNode::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

bool TileNode::IsSelected() const
{
	return m_selected;
}

void TileNode::Select()
{
	std::cout << "Selected" << std::endl;
	m_selected = true;
	m_sprite.setColor(sf::Color(255, 255, 255, 100));
}

void TileNode::Deselect()
{
	std::cout << "Deselected" << std::endl;
	m_selected = false;
	m_sprite.setColor(sf::Color(255, 255, 255, 255));
}

void TileNode::SetCellPosition(sf::Vector2i position, float cell_size)
{
	m_cell_position = position;
	setPosition(sf::Vector2f(position) * cell_size);

}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
