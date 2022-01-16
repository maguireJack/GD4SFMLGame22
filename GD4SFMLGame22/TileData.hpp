#pragma once
#include <SFML/System/Vector2.hpp>

#include "DataTables.hpp"
#include "Textures.hpp"
#include <sstream>

class TileData
{
public:
	TileData(Textures textures, bool pickable = false);

	bool IsPickable() const;
	sf::Vector2i GetCellPosition() const;
	sf::Vector2i GetCellSize() const;

	void SetCellPosition(sf::Vector2i position);
	void SetPickable(bool pickable);

	std::unordered_set<PlatformEffects> GetEffects() const;
	Textures GetTexture() const;
	std::string ToSerial();

	bool operator ==(const TileData& other) const;

private:
	Textures m_texture;
	bool m_pickable;
	sf::Vector2i m_cell_position;
};

