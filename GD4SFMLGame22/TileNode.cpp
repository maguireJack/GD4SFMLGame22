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

TileNode::TileNode(PlatformType platform, const TextureHolder& textures)
	: m_platform(platform)
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

void TileNode::SetTarget(sf::Vector2i position) {
	this->setPosition(sf::Vector2f(position * 16));
}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
