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
		const sf::Texture& texture,
		PlatformType platform,
		bool pickable = false);

	unsigned GetCategory() const override;
	sf::FloatRect GetBoundingRect() const override;

	bool IsPickable() const;
	bool IsSelected() const;
	void Select();
	void Deselect();
	void Destroy();
	void SetCellPosition(sf::Vector2i position, float cell_size);
	void SetPickable(bool pickable);

	bool IsDestroyed() const override;

private:
	void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

private:
	PlatformType m_platform;
	bool m_pickable;
	sf::Vector2i m_cell_position;
	bool m_selected;
	bool m_destroy;
	sf::Sprite m_sprite;
};
