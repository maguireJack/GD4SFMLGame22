#include "TileNode.hpp"

#include <iostream>
#include <ostream>

#include "Application.hpp"
#include "Utility.hpp"

TileNode::TileNode(
	const TextureHolder& textures,
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	Textures texture,
	PlatformType platform,
	bool pickable)
	: SceneNode(scene_layers)
	, m_data(texture, platform, pickable)
	, m_selected(false)
	, m_destroy(false)
	, m_sprite(textures.Get(texture))
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

void TileNode::Destroy()
{
	m_destroy = true;
}

void TileNode::SetCellPosition(sf::Vector2i position, float cell_size)
{
	m_data.SetCellPosition(position);
	setPosition(sf::Vector2f(position) * cell_size);

}

TileData TileNode::Data() const
{
	return m_data;
}

bool TileNode::IsDestroyed() const
{
	return m_destroy;
}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
