#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>

#include "SceneNode.hpp"
#include "TileNode.hpp"
#include "Vector2iHash.hpp"

class GridNode : public SceneNode
{
public:
	GridNode(const sf::RenderWindow& window, const sf::View& camera_view, int horizontal_cells, int vertical_cells, float cell_size, float line_width);

	void AddTileNode(std::unique_ptr<TileNode> tile_node);
	void AddTileNode(TileNode* tile_node);
	void RemoveTile(const TileNode* tile);

	sf::Vector2i GetCellPosition(sf::Vector2i position) const;
	sf::Vector2i GetCellPosition(sf::Vector2f position) const;

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands);

	void HandleEvent(const sf::Event& event, CommandQueue& commands);

private:
	const sf::RenderWindow& m_window;
	const sf::View& m_camera_view;

	std::unordered_map<sf::Vector2i, TileNode*, Vector2iHash> m_tile_map;
	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;
	sf::Vector2i m_selected_cell_position;
	sf::Vector2i m_new_cell_position;
	TileNode* m_selected_tile_node;
};
