#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class GridNode : public SceneNode
{
public:
	GridNode(const sf::RenderWindow& window, const sf::View& camera_view, int horizontal_cells, int vertical_cells, float cell_size, float line_width);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	

private:
	const sf::RenderWindow& m_window;
	const sf::View& m_camera_view;

	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;
	sf::Vector2i m_selected_cell_position;
};
