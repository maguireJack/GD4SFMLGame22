#include "Tile.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

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

bool Tile::IsSelected() const
{
	return m_selected;
}

void Tile::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void Tile::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}