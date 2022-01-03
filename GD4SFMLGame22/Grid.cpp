#include "Grid.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>

Grid::Grid(const sf::RenderWindow& window, const sf::View& camera_view, int horizontal_cells, int vertical_cells, float cell_size, float line_width)
	: m_window(window)
	, m_camera_view(camera_view)
	, m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
	, m_selected_cell_position(0, 0)
{
}


void Grid::AddTile(std::shared_ptr<Tile> tile, sf::Vector2f position)
{
	sf::Vector2i cell_position = GetCellPosition(position);
	m_tile_map[cell_position] = tile;
}

void Grid::AddTileNode(std::unique_ptr<TileNode> tile_node)
{
	/*sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map[cell_position] = tile_node->GetTile();
	*/
}

void Grid::RemoveTile(Tile* tile)
{
}

//Tile& Grid::GetTile()
//{
//}

sf::Vector2i Grid::GetCellPosition(sf::Vector2i position) const
{
	const sf::Vector2f pixel_position = m_window.mapPixelToCoords(position, m_camera_view);

	return {
		static_cast<int>(pixel_position.x / m_cell_size),
		static_cast<int>(pixel_position.y / m_cell_size)
	};
}

sf::Vector2i Grid::GetCellPosition(sf::Vector2f position) const
{
	return GetCellPosition(sf::Vector2i(position));
}

void Grid::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape horizontal_line(sf::Vector2f(m_cell_size * m_horizontal_cells, m_line_width));
	sf::RectangleShape vertical_line(sf::Vector2f(m_line_width, m_cell_size * m_vertical_cells));

	horizontal_line.setFillColor(sf::Color::White);
	vertical_line.setFillColor(sf::Color::White);

	for (int i = 0; i < m_vertical_cells; i++)
	{
		horizontal_line.setPosition(sf::Vector2f(0, i * m_cell_size));
		target.draw(horizontal_line, states);
	}

	for (int i = 0; i < m_horizontal_cells; i++)
	{
		vertical_line.setPosition(sf::Vector2f(i * m_cell_size, 0));
		target.draw(vertical_line, states);
	}

	sf::RectangleShape square(sf::Vector2f(m_cell_size, m_cell_size));
	square.setPosition(sf::Vector2f(m_selected_cell_position.x * m_cell_size, m_selected_cell_position.y * m_cell_size));
	target.draw(square, states);
}

void Grid::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	const sf::Vector2i cell_position = GetCellPosition(sf::Mouse::getPosition(m_window));

	if (cell_position != m_selected_cell_position)
	{
		m_selected_cell_position = cell_position;

		//TODO: Send command that position has changed to update the position of the selected tile
	}
}