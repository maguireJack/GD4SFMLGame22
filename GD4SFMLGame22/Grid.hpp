#pragma once

#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

#include "Tile.hpp"
#include "TileNode.hpp"

/// <summary>
/// A hash function used to hash a vector2 of integers.
///	Implementation modified from boost hash_combine
/// </summary>
///	<see cref="https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x"/>
struct Vector2iHash {
	size_t operator()(const sf::Vector2i vector) const
	{
		const size_t hash1 = std::hash<int>{}(vector.x);
		const size_t hash2 = std::hash<int>{}(vector.y);

		return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
	}
};

class Grid
{
public:
	Grid(const sf::RenderWindow& window, const sf::View& camera_view, int horizontal_cells, int vertical_cells, float cell_size, float line_width);
	void AddTile(std::shared_ptr<Tile> tile, sf::Vector2f position);
	void AddTileNode(std::unique_ptr<TileNode> tile_node);
	void RemoveTile(Tile* tile);
	Tile& GetTile();

	sf::Vector2i GetCellPosition(sf::Vector2i position) const;
	sf::Vector2i GetCellPosition(sf::Vector2f position) const;

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands);

private:
	const sf::RenderWindow& m_window;
	const sf::View& m_camera_view;

	std::unordered_map<sf::Vector2i, std::shared_ptr<Tile>, Vector2iHash> m_tile_map;
	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;
	sf::Vector2i m_selected_cell_position;
};
