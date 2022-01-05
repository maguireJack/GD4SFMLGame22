#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Tile.hpp"
#include "SceneNode.hpp"

class TileNode : public SceneNode
{
public:
	TileNode(std::shared_ptr<Tile> tile);
	TileNode(TileNode* tile);
	std::shared_ptr<Tile> GetTile();
	void SetTarget(sf::Vector2i pos);
	sf::FloatRect GetBoundingRect() const override;
	unsigned GetCategory() const override;

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	std::shared_ptr<Tile> m_tile;
	sf::Vector2f m_cell_position;
	
};
