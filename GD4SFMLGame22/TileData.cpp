#include "TileData.hpp"

namespace
{
	const std::unordered_map<Textures, PlatformData> Table = InitializePlatformData();
}

TileData::TileData(Textures textures, bool pickable)
	: m_texture(textures)
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
	if (Table.count(m_texture))
	{
		return Table.at(m_texture).m_cell_size;
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

std::unordered_set<PlatformEffects> TileData::GetEffects() const
{
	if (Table.count(m_texture))
	{
		return Table.at(m_texture).m_effects;
	}

	return std::unordered_set<PlatformEffects>();
}

Textures TileData::GetTexture() const
{
	return m_texture;
}

std::string TileData::ToSerial()
{
	std::stringstream ss;
	ss << m_pickable << ',' << m_cell_position.x << ',' << m_cell_position.y << ',' << static_cast<int>(m_texture) << "\n";
	return ss.str();
}

bool TileData::operator==(const TileData& other) const
{
	return m_texture == other.m_texture;
}