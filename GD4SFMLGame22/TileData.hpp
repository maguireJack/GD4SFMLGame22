#pragma once
#include <SFML/System/Vector2.hpp>

#include "DataTables.hpp"
#include "PlatformType.hpp"
#include "Textures.hpp"

namespace
{
	const std::vector<PlatformData> Table = InitializePlatformData();
	const std::unordered_map<Textures, sf::Vector2i> CellSizes = InitializeTextureCellSizeData();
}


class TileData
{
public:
	TileData(Textures textures, PlatformType platform, bool pickable = false);

	bool IsPickable() const;
	sf::Vector2i GetCellPosition() const;
	sf::Vector2i GetCellSize() const;

	void SetCellPosition(sf::Vector2i position);
	void SetPickable(bool pickable);

	PlatformType GetPlatformType() const;
	Textures GetTexture() const;

	bool operator ==(const TileData& other) const;

private:
	Textures m_texture;
	PlatformType m_platform;
	bool m_pickable;
	sf::Vector2i m_cell_position;
};

