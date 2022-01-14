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

TileNode::TileNode(const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	const sf::Texture& texture,
	PlatformType platform,
	bool pickable)
	: SceneNode(scene_layers)
	, m_platform(platform)
	, m_pickable(pickable)
	, m_selected(false)
	, m_destroy(false)
	, m_sprite(texture)
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

bool TileNode::IsPickable() const
{
	return m_pickable;
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

void TileNode::Destroy()
{
	m_destroy = true;
}

void TileNode::SetCellPosition(sf::Vector2i position, float cell_size)
{
	m_cell_position = position;
	setPosition(sf::Vector2f(position) * cell_size);

}

void TileNode::SetPickable(bool pickable)
{
	m_pickable = pickable;
}

bool TileNode::IsDestroyed() const
{
	return m_destroy;
}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
