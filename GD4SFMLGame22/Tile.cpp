#include "Tile.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include "DataTables.hpp"
#include "ResourceHolder.hpp"

namespace
{
	const std::vector<PlatformData> Table = InitializePlatformData();
}

Tile::Tile(PlatformType platform, const TextureHolder& textures) : 
	  m_platform(platform)
	, m_selected(false)
	, m_sprite(textures.Get(Table[static_cast<int>(platform)].m_textures))
{
}

bool Tile::IsSelected()
{
	return m_selected;
}

Tile Tile::Select()
{
	std::cout << "Selected" << std::endl;
	m_selected = true;
	m_sprite.setColor(sf::Color(255, 255, 255, 100));
	return *this;
}

void Tile::Deselect()
{
	std::cout << "Deslected" << std::endl;
	m_selected = false;
	m_sprite.setColor(sf::Color(255, 255, 255, 255));
}

void Tile::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{

}

void Tile::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}