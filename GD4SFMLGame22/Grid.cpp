#include "Grid.hpp"

#include "GridNode.hpp"

Grid::Grid()
	: m_grid_node(nullptr)
{
}

GridNode& Grid::Node() const
{
	return *m_grid_node;
}

void Grid::SetNode(GridNode* grid_node)
{
	m_grid_node = grid_node;
}

void Grid::HandleEvent(const sf::Event& event, CommandQueue& commands) const
{
	m_grid_node->HandleEvent(event, commands);
}
