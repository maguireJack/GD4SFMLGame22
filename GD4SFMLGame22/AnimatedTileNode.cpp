#include "AnimatedTileNode.hpp"


AnimatedTileNode::AnimatedTileNode(const TextureHolder& textures,
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	Textures texture,
	AnimationData data,
	bool pickable) : TileNode(textures, scene_layers, texture, pickable), m_anim_artist(std::vector({ data }), textures)
{

}

sf::FloatRect AnimatedTileNode::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_anim_artist.GetLargestBounds());
}

void AnimatedTileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_anim_artist.DrawCurrent(target, states);
}

void AnimatedTileNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_anim_artist.UpdateCurrent(dt);
}

unsigned AnimatedTileNode::GetCategory() const
{
	return Category::kPickup;
}
