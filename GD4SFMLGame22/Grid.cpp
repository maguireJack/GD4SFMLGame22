#include "Grid.hpp"

#include "GridNode.hpp"

Grid::Grid()
	: m_grid_node(nullptr)
	, m_editor_mode(false)
{
}

GridNode& Grid::Node() const
{
	return *m_grid_node;
}

void Grid::SetNode(GridNode* grid_node)
{
	m_grid_node = grid_node;
	m_grid_node->SetEditorMode(m_editor_mode);
	m_grid_node->LoadData(m_path_to_load);
}

void Grid::SetPathToLoad(const std::string& path)
{
	m_path_to_load = path;
}

void Grid::SetEditMode(bool editor_mode)
{
	m_editor_mode = editor_mode;
}

void Grid::CreatePath()
{
	int number = 0;
	std::string path;
	std::fstream stream;

	do
	{
		number++;
		path = "Levels/level" + std::to_string(number) + ".sav";
		stream = std::fstream(path);
	} while (stream.good());

	stream.open("Levels/level" + std::to_string(number), std::fstream::out);
	m_path_to_load = path;
}

void Grid::HandleEvent(const sf::Event& event, CommandQueue& commands) const
{
	m_grid_node->HandleEvent(event, commands);
}
