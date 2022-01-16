#pragma once
#include <SFML/Window/Event.hpp>

#include "GridNode.hpp"

class Grid
{
public:
	Grid();
	GridNode& Node() const;
	void SetNode(GridNode* grid);
	void SetPathToLoad(const std::string& path);
	void HandleEvent(const sf::Event& event, CommandQueue& commands) const;

private:
	GridNode* m_grid_node;
	std::string m_path_to_load;
};
