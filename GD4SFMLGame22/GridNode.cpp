#include "GridNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

GridNode::GridNode(int horizontal_cells, int vertical_cells, float cell_size, float line_width)
	: m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
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
}
