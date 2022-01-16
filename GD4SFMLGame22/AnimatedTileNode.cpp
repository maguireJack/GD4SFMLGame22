#include "AnimatedTileNode.hpp"

/// <summary>
/// Animated Tile Node
/// Jack - Creates an animated tile node for pickups within the game
/// </summary>
/// <param name="textures">Texture Holder</param>
/// <param name="scene_layers">Scene Layers</param>
/// <param name="texture">Texture of the AnimatedTileNode</param>
/// <param name="data">Animation Data for the Animated Sprite Artist</param>
/// <param name="pickable">Whether or not its pickable</param>

AnimatedTileNode::AnimatedTileNode(const TextureHolder& textures,
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	Textures texture,
	AnimationData data,
	bool pickable) : TileNode(textures, scene_layers, texture, pickable), m_anim_artist(std::vector({ data }), textures)
{

}

/// <summary>
/// Returns the BoundingRect relative to the world transform
/// </summary>
/// <returns></returns>
sf::FloatRect AnimatedTileNode::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_anim_artist.GetLargestBounds());
}

/// <summary>
/// Draw Method
/// </summary>
/// <param name="target"></param>
/// <param name="states"></param>
void AnimatedTileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_anim_artist.DrawCurrent(target, states);
}

/// <summary>
/// Update
/// </summary>
/// <param name="dt"></param>
/// <param name="commands"></param>
void AnimatedTileNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_anim_artist.UpdateCurrent(dt);
}

/// <summary>
/// Returns Pickup Category
/// </summary>
/// <returns></returns>
unsigned AnimatedTileNode::GetCategory() const
{
	return Category::kPickup;
}
