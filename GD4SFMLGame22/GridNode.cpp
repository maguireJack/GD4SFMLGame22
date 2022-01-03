#include "GridNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Application.hpp"

GridNode::GridNode(Grid& grid)
	: m_grid(grid)
{
}

void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	m_grid.DrawCurrent(target, states);
}

void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_grid.UpdateCurrent(dt, commands);
}
