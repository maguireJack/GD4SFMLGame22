#include "TileNode.hpp"

#include "Application.hpp"
#include "Utility.hpp"
#include <iostream>


TileNode::TileNode(std::shared_ptr<Tile> tile) : m_tile(tile)
{

}

TileNode::TileNode(TileNode* tile)
{
	m_tile = tile->GetTile();
}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_tile->DrawCurrent(target, states);
}

void TileNode::SetTarget(sf::Vector2i position) {
	this->setPosition(sf::Vector2f(position * 16));
}

void TileNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	
}

std::shared_ptr<Tile> TileNode::GetTile()
{
	return m_tile;
}
