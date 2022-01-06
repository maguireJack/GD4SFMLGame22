#pragma once
#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		const sf::Texture& texture);

	SpriteNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		const sf::Texture& texture,
		const sf::IntRect& textureRect);

	SpriteNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		const sf::Texture& texture,
		const sf::IntRect& textureRect,
		const float position_x,
		const float position_y);

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;
};

