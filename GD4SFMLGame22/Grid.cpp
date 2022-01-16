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
	m_grid_node->LoadData(m_path_to_load);
}

void Grid::SetPathToLoad(const std::string& path)
{
	m_path_to_load = path;
}

void Grid::HandleEvent(const sf::Event& event, CommandQueue& commands) const
{
	m_grid_node->HandleEvent(event, commands);
}
