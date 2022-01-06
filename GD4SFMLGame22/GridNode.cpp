#include "GridNode.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Application.hpp"

GridNode::GridNode(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	const sf::RenderWindow& window,
	const sf::View& camera_view,
	int horizontal_cells,
	int vertical_cells,
	float cell_size,
	float line_width)
	: SceneNode(scene_layers)
	, m_window(window)
	, m_camera_view(camera_view)
	, m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
	, m_selected_cell_position(0, 0)
	, m_new_cell_position(0, 0)
	, m_selected_tile_node(nullptr)
{
}

void GridNode::AddTileNode(std::unique_ptr<TileNode> tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map[cell_position] = tile_node.get();

	GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
}

void GridNode::AddTileNode(TileNode* tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map[cell_position] = tile_node;
}

void GridNode::RemoveTile(const TileNode* tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map.erase(cell_position);
}

sf::Vector2i GridNode::GetCellPosition(sf::Vector2i position) const
{
	const sf::Vector2f pixel_position = m_window.mapPixelToCoords(position, m_camera_view);

	return {
		static_cast<int>(pixel_position.x / m_cell_size),
		static_cast<int>(pixel_position.y / m_cell_size)
	};
}

sf::Vector2i GridNode::GetCellPosition(sf::Vector2f position) const
{
	return GetCellPosition(sf::Vector2i(position));
}

void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape horizontal_line(sf::Vector2f(m_cell_size * m_horizontal_cells, m_line_width));
	sf::RectangleShape vertical_line(sf::Vector2f(m_line_width, m_cell_size * m_vertical_cells));

	sf::Color color(255, 255, 255, 150);
	horizontal_line.setFillColor(color);
	vertical_line.setFillColor(color);

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

void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	const sf::Vector2i cell_position = GetCellPosition(sf::Mouse::getPosition(m_window));
	m_new_cell_position = cell_position;
	/*std::cout << "(" << cell_position.x << ", " << cell_position.y << ")" << std::endl;*/
	// std::cout << "(" << m_selected_cell_position.x << ", " << m_selected_cell_position.y << ")" << std::endl;
	if (cell_position != m_selected_cell_position)
	{
		m_selected_cell_position = cell_position;
	}
	if (m_selected_tile_node != nullptr)
	{
		m_selected_tile_node->SetTarget(cell_position);
	}
}

void GridNode::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (m_tile_map[m_selected_cell_position])
			{
				if (m_selected_tile_node == nullptr) {
					std::cout << "Hovering" << std::endl;
					if (!m_tile_map[m_selected_cell_position]->IsSelected())
					{
						m_tile_map[m_selected_cell_position]->Select();
						m_selected_tile_node = m_tile_map[m_selected_cell_position];
						m_tile_map[m_selected_cell_position] = nullptr;
					}
				}

			}
			else if (m_selected_tile_node)
			{
				m_selected_tile_node->Deselect();
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
