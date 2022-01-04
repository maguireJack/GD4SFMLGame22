#include "Grid.hpp"
#include "Tile.hpp"
#include <iostream>
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
	, m_new_cell_position(0,0)
	, m_selected_tile_node(nullptr)
{
}


//void Grid::AddTile(std::shared_ptr<Tile> tile, sf::Vector2f position)
//{
//	sf::Vector2i cell_position = GetCellPosition(position);
//	m_tile_map[cell_position] = tile;
//}

void Grid::AddTileNode(TileNode* tile_node)
{
	sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map[cell_position] = tile_node;
	
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
	m_new_cell_position = cell_position;
	/*std::cout << "(" << cell_position.x << ", " << cell_position.y << ")" << std::endl;*/
	std::cout << "(" << m_selected_cell_position.x << ", " << m_selected_cell_position.y << ")" << std::endl;
	if (cell_position != m_selected_cell_position)
	{
		m_selected_cell_position = cell_position;
	}
	if (m_selected_tile_node != nullptr) 
	{
		m_selected_tile_node->SetTarget(cell_position);
	}
	
}

void Grid::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (m_tile_map[m_selected_cell_position])
			{
				if (m_selected_tile_node == nullptr) {
					std::cout << "Hovering" << std::endl;
					if (!m_tile_map[m_selected_cell_position]->GetTile()->IsSelected())
					{
						m_tile_map[m_selected_cell_position]->GetTile()->Select();
						m_selected_tile_node = m_tile_map[m_selected_cell_position];
						m_tile_map[m_selected_cell_position] = nullptr;
					}
				}

			}
			else if (m_selected_tile_node)
			{
				m_selected_tile_node->GetTile()->Deselect();
				m_selected_tile_node->setPosition(sf::Vector2f(m_new_cell_position * 16));
				m_tile_map[m_selected_cell_position] = nullptr;
				m_tile_map.erase(m_selected_cell_position);
				m_tile_map[m_new_cell_position] = m_selected_tile_node;
				m_selected_tile_node = nullptr;
			}
		}
		/*else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (m_tile_map[m_selected_cell_position])
			{
				if (m_selected_tile_node == nullptr) {
					std::cout << "Hovering" << std::endl;
					TileNode copied_tile(m_tile_map[m_selected_cell_position]);
					
					if (!copied_tile.GetTile()->IsSelected())
					{
						copied_tile.GetTile()->Select();
						m_selected_tile_node = &copied_tile;
					}
				}

			}
		}
		*/
	}
}
