#pragma once
#include "TileNode.hpp"
#include "AnimatedSpriteArtist.hpp"

//D00218436 Vilandas Morrissey
//D00219343 Jack Maguire

class AnimatedTileNode : public TileNode
{
public:
	AnimatedTileNode(const TextureHolder& textures,
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		Textures texture, 
		AnimationData data,
		bool pickable = false);

	unsigned GetCategory() const override;
	sf::FloatRect GetBoundingRect() const override;

private:
	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	AnimatedSpriteArtist m_anim_artist;
};