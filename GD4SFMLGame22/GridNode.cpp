#include "GridNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

GridNode::GridNode(const float width, const float height, float cellsize) : m_width(width), m_height(height), m_cellsize(cellsize)
{
	for (int i = 0; i < width / cellsize; i++)
	{
		for (int j = 0; j < height / cellsize; j++)
		{
			sf::Vector2<int> init(i,j);
			m_points.emplace_back(init);
		}
	}
}
void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Vector2f sizeH(2.f, m_height);
	sf::Vector2f sizeW(m_width, 2.f);
	sf::RectangleShape lineH(sizeH);
	sf::RectangleShape lineW(sizeW);

	lineH.setFillColor(sf::Color::Green);
	lineW.setFillColor(sf::Color::Green);

	target.draw(lineH, states);
	target.draw(lineW, states);

	for (auto iter= m_points.begin(); iter != m_points.end(); iter++)
	{
		lineH.setPosition(sf::Vector2f(iter->x * m_cellsize, 0));
		target.draw(lineH);

		lineW.setPosition(sf::Vector2f(0, iter->y * m_cellsize));
		target.draw(lineW);


	}
	
}
