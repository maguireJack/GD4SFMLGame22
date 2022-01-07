#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>

#include "Camera.hpp"
#include "SceneNode.hpp"
#include "TileNode.hpp"
#include "Vector2iHash.hpp"

class GridNode : public SceneNode
{
public:
	GridNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		const sf::RenderWindow& window,
		const TextureHolder& textures,
		const Camera& camera_view,
		int horizontal_cells,
		int vertical_cells,
		float cell_size,
		float line_width);

	void SetNewTileSettings(PlatformType type, Textures texture);
	void AddTileNode(std::unique_ptr<TileNode> tile_node);
	void RemoveTile(const TileNode* tile);

	bool IsHoldingTile() const;
	bool IsInCreateMode() const;
	bool CellContainsTile(sf::Vector2i cell_position) const;

	sf::Vector2i GetCellPosition(sf::Vector2i position) const;
	sf::Vector2i GetCellPosition(sf::Vector2f position) const;
	sf::Vector2i MouseToCellPosition() const;

	const Camera& GetCamera() const;

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands);

	void HandleEvent(const sf::Event& event, CommandQueue& commands);

private:
	void CreateTile();
	void PickupTile();
	void DropTile();
	void DropTileAt(sf::Vector2i cell_position);

private:
	const sf::RenderWindow& m_window;
	const TextureHolder& m_textures;
	const Camera& m_camera;

	std::unordered_map<sf::Vector2i, TileNode*, Vector2iHash> m_tile_map;
	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;

	bool m_can_place;

	sf::Vector2i m_mouse_cell_position;
	sf::Vector2i m_picked_up_position;
	TileNode* m_selected_tile;

	Textures m_create_texture;
	PlatformType m_create_type;
};
