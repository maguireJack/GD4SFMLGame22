#include "TileNode.hpp"

#include "Application.hpp"

TileNode::TileNode(std::shared_ptr<Tile> tile)
	: m_tile(tile)
{
}


void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

	m_tile->DrawCurrent(target, states);
}

void TileNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_tile->UpdateCurrent(dt, commands);
}
