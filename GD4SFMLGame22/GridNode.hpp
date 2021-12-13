#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "SceneNode.hpp"

class GridNode : public SceneNode
{
public:
	GridNode(const float width, const float height, float cellsize);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	std::vector<sf::Vector2<int>> m_points;
	float m_width;
	float m_height;
	float m_cellsize;

};

