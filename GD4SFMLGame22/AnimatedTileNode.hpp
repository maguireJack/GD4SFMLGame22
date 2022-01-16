#pragma once
#include "TileNode.hpp"
#include "AnimatedSpriteArtist.hpp"

class AnimatedTileNode : public TileNode{

public:
	sf::FloatRect GetBoundingRect() const override;
	AnimatedTileNode(const TextureHolder& textures,
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		Textures texture, 
		AnimationData data,
		bool pickable = false);

private:
	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	unsigned GetCategory() const override;


private:
	AnimatedSpriteArtist m_anim_artist;
};