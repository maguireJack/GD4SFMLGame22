#include "TileData.hpp"

TileData::TileData(Textures textures, PlatformType platform, bool pickable)
	: m_texture(textures)
	, m_platform(platform)
	, m_pickable(pickable)
{
}

bool TileData::IsPickable() const
{
	return m_pickable;
}

sf::Vector2i TileData::GetCellPosition() const
{
	return m_cell_position;
}

sf::Vector2i TileData::GetCellSize() const
{
	if (CellSizes.count(m_texture))
	{
		return CellSizes.at(m_texture);
	}

	return { 1, 1 };
}

void TileData::SetCellPosition(sf::Vector2i position)
{
	m_cell_position = position;
}

void TileData::SetPickable(bool pickable)
{
	m_pickable = pickable;
}

PlatformType TileData::GetPlatformType() const
{
	return m_platform;
}

Textures TileData::GetTexture() const
{
	return m_texture;
}

bool TileData::operator==(const TileData& other) const
{
	return m_platform == other.m_platform &&
		m_texture == other.m_texture;
}