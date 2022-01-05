#pragma once
#include <SFML/Window/Event.hpp>

#include "GridNode.hpp"

class Grid
{
public:
	Grid();
	GridNode& Node() const;
	void SetNode(GridNode* grid);
	void HandleEvent(const sf::Event& event, CommandQueue& commands) const;

private:
	GridNode* m_grid_node;
};
