#include "TileNode.hpp"

#include "Application.hpp"
#include "Utility.hpp"
#include <iostream>


TileNode::TileNode(std::shared_ptr<Tile> tile) : m_tile(tile)
{

}

unsigned int TileNode::GetCategory() const
{
	return Category::kPlatform;
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

sf::FloatRect TileNode::GetBoundingRect() const 
{
	return GetWorldTransform().transformRect(m_tile->GetSprite().getGlobalBounds());
}


std::shared_ptr<Tile> TileNode::GetTile()
{
	return m_tile;
}
