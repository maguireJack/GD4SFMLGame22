#include "GridNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Application.hpp"

GridNode::GridNode(const sf::RenderWindow& window, const sf::View& camera_view, int horizontal_cells, int vertical_cells, float cell_size, float line_width)
	: m_window(window)
	, m_camera_view(camera_view)
	, m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
	, m_selected_cell_position(0, 0)
{
}

void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
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

void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	sf::Vector2i mouse_position = sf::Mouse::getPosition(m_window);
	sf::Vector2f pixel_position = m_window.mapPixelToCoords(mouse_position, m_camera_view);

	sf::Vector2i cell_position(pixel_position.x / m_cell_size, pixel_position.y / m_cell_size);

	if (cell_position != m_selected_cell_position)
	{
		m_selected_cell_position = cell_position;

		//TODO: Send command that position has changed to update the position of the selected tile
	}
}
