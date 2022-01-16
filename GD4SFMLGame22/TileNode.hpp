#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "TileData.hpp"

//D00218436 Vilandas Morrissey
//D00219343 Jack Maguire

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
	sf::Vector2f GetVelocity() const override;
	float GetDeltaTimeInSeconds() const override;

	bool IsSelected() const;
	void Select();
	void Deselect();
	void Destroy();
	void SetCellPosition(sf::Vector2i position, float cell_size);

	TileData Data() const;

	bool IsDestroyed() const override;

// Effects
public:
	void ActivateVerticalMovement();

private:
	void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	TileData m_data;
	bool m_selected;
	bool m_destroy;
	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;
	float m_delta_time_in_seconds;
};
