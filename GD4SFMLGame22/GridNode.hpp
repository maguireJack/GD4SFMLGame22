#pragma once
#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class GridNode : public SceneNode
{
public:
	GridNode(int horizontal_cells, int vertical_cells, float cell_size, float line_width);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;
};

