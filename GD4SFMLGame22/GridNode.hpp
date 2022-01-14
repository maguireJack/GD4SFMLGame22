#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Camera.hpp"
#include "SceneNode.hpp"
#include "ScrollableContainer.hpp"
#include "SoundPlayer.hpp"
#include "TexturedButton.hpp"
#include "TileDataHash.hpp"
#include "TileNode.hpp"
#include "Vector2iHash.hpp"

class GridNode : public SceneNode
{
public:
	GridNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		sf::RenderWindow& window,
		TextureHolder& textures,
		FontHolder& fonts,
		SoundPlayer& sounds,
		Camera& camera_view,
		int horizontal_cells,
		int vertical_cells,
		float cell_size,
		float line_width,
		bool editor_mode = false);

	void SetNewTileSettings(PlatformType type, Textures texture);
	void ExitCreateMode();
	void AddTileNode(std::unique_ptr<TileNode> tile_node);
	void RemoveTile(const TileNode* tile);

	bool IsHoldingTile() const;
	bool IsInCreateMode() const;
	bool CellContainsTile(sf::Vector2i cell_position) const;
	bool CellPickable(sf::Vector2i cell_position);

	int GetInventoryCount(const TileData& tile);
	void SelectFromInventory(TileData& tile);
	void AddToInventory(TileData& tile, int count = 1);
	void AddToInventory(PlatformType platform, Textures texture, int count = 1);
	void RemoveFromInventory(const TileData& tile, int count = 1);

	sf::Vector2i GetCellPosition(sf::Vector2i position) const;
	sf::Vector2i GetCellPosition(sf::Vector2f position) const;
	sf::Vector2i MouseToCellPosition() const;

	const Camera& GetCamera() const;

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands);

	void HandleEvent(const sf::Event& event, CommandQueue& commands);

private:
	bool CreateTile();
	bool PickupTile();
	bool DropTile();
	void DropTileAt(sf::Vector2i cell_position);

private:
	sf::RenderWindow& m_window;
	TextureHolder& m_textures;
	FontHolder& m_fonts;
	SoundPlayer& m_sounds;
	Camera& m_camera;

	int m_horizontal_cells;
	int m_vertical_cells;
	float m_cell_size;
	float m_line_width;

	std::unordered_map<sf::Vector2i, TileNode*, Vector2iHash> m_tile_map;
	std::unordered_map<TileData, int, TileDataHash> m_inventory;

	GUI::ScrollableContainer m_editor_gui;
	GUI::ScrollableContainer m_inventory_adder_gui;
	GUI::ScrollableContainer m_inventory_gui;
	std::shared_ptr<GUI::TexturedButton> m_selected_button;
	sf::RectangleShape m_background;
	sf::Vector2f m_background_position;

	bool m_editor_mode;
	bool m_inventory_mode;

	bool m_can_place;
	bool m_can_pickup;
	bool m_mouse_contains_tile;

	sf::Vector2i m_mouse_cell_position;
	sf::Vector2i m_picked_up_position;
	TileNode* m_selected_tile;

	Textures m_create_texture;
	PlatformType m_create_type;
};
