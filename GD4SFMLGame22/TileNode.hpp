#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "PlatformType.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

class TileNode : public SceneNode
{
public:
	TileNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers, 
		PlatformType platform,
		const TextureHolder& textures);

	unsigned GetCategory() const override;
	sf::FloatRect GetBoundingRect() const override;

	bool IsSelected() const;
	void Select();
	void Deselect();
	void SetCellPosition(sf::Vector2i position, float cell_size);

private:
	void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

private:
	PlatformType m_platform;
	sf::Vector2i m_cell_position;
	sf::Vector2f m_follow_target;
	bool m_selected = false;
	bool m_follow = false;
	sf::Sprite m_sprite;
	sf::Vector2f follow_mouse;
	
};
