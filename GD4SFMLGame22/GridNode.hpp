#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Grid.hpp"
#include "SceneNode.hpp"

class GridNode : public SceneNode
{
public:
	GridNode(Grid& grid);

	sf::Vector2i GetCellPosition(sf::Vector2i position) const;
	sf::Vector2i GetCellPosition(sf::Vector2f position) const;

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	Grid& m_grid;
};
