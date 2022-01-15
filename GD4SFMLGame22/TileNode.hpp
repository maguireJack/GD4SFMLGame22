#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "TileData.hpp"

class TileNode : public SceneNode
{
public:
	TileNode(
		const TextureHolder& textures,
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		Textures texture,
		bool pickable = false);

	unsigned GetCategory() const override;
	sf::FloatRect GetBoundingRect() const override;

	bool IsSelected() const;
	void Select();
	void Deselect();
	void Destroy();
	void SetCellPosition(sf::Vector2i position, float cell_size);

	TileData Data() const;

	bool IsDestroyed() const override;

private:
	void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

private:
	TileData m_data;
	bool m_selected;
	bool m_destroy;
	sf::Sprite m_sprite;
};
