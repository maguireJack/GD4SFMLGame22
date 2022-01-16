#include "GridNode.hpp"

#include <utility>

#include "Button.hpp"
#include "TexturedButton.hpp"

GridNode::GridNode(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	sf::RenderWindow& window,
	TextureHolder& textures,
	FontHolder& fonts,
	SoundPlayer& sounds,
	Camera& camera,
	int horizontal_cells,
	int vertical_cells,
	float cell_size,
	float line_width,
	bool editor_mode)
	: SceneNode(scene_layers)
	, m_window(window)
	, m_textures(textures)
	, m_fonts(fonts)
	, m_sounds(sounds)
	, m_camera(camera)
	, m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
	, m_editor_gui(window, camera)
	, m_editor_inventory_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
	, m_editor_adder_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0)
	, m_inventory_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
	, m_inventory_labels_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
	, m_selected_button(nullptr)
	, m_editor_mode(editor_mode)
	, m_editor_loaded(false)
	, m_inventory_mode(false)
	, m_can_place(true)
	, m_can_pickup(false)
	, m_coin_count(0)
	, m_mouse_contains_tile(false)
	, m_mouse_cell_position(0, 0)
	, m_selected_tile(nullptr)
	, m_create_texture(Textures::kDefault)
{
	m_background.setFillColor(sf::Color(0, 0, 0, 150));
	m_background.setSize(sf::Vector2f(576, 48));
	m_background_position = sf::Vector2f(0, 276);
}

/// <summary>
/// Vilandas - Set the new "CreateMode" tile, destroy any tiles currently held
/// </summary>
/// <param name="texture"></param>
void GridNode::SetNewTileSettings(Textures texture)
{
	if (IsHoldingTile())
	{
		if (m_editor_mode)
		{
			m_editor_inventory_gui.DeactivateAll();
		}
		else
		{
			m_inventory_gui.DeactivateAll();
		}

		m_selected_tile->Destroy();
		m_selected_tile = nullptr;
	}

	m_create_texture = texture;
}

/// <summary>
/// Vilandas - If the texture m_create texture is kDefault, exit create tile mode
/// </summary>
void GridNode::ExitCreateMode()
{
	SetNewTileSettings(Textures::kDefault);
}

/// <summary>
/// Vilandas/Jack - Apply the tile to it's relative cell position in the grid.
/// </summary>
/// <param name="tile_node"></param>
void GridNode::AddTileNode(std::unique_ptr<TileNode> tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	const sf::Vector2i cell_size = tile_node->Data().GetCellSize();
	tile_node->SetCellPosition(cell_position, 16);

	if (tile_node->Data().GetTexture() == Textures::kCoin)
	{
		m_coin_count++;
	}

	// For tiles that take up multiple cells
	for (int x = 0; x < cell_size.x; x++)
	{
		for (int y = 0; y < cell_size.y; y++)
		{
			sf::Vector2i delta(x, y);
			m_tile_map[cell_position + delta] = tile_node.get();
		}
	}

	GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
}

/// <summary>
/// Jack/Vilandas - Apply the tile to the indicated cell position
/// </summary>
/// <param name="tile_node"></param>
/// <param name="cell_position"></param>
void GridNode::AddTileNode(TileNode* tile_node, sf::Vector2i cell_position)
{
	const sf::Vector2i cell_size = tile_node->Data().GetCellSize();
	tile_node->SetCellPosition(cell_position, 16);

	for (int x = 0; x < cell_size.x; x++)
	{
		for (int y = 0; y < cell_size.y; y++)
		{
			sf::Vector2i delta(x, y);
			m_tile_map[cell_position + delta] = tile_node;
		}
	}
}

/// <summary>
/// Jack/Vilandas - Remove a tile from the grid
/// </summary>
/// <param name="tile_node"></param>
void GridNode::RemoveTile(const TileNode* tile_node)
{
	const sf::Vector2i cell_position = tile_node->Data().GetCellPosition();
	const sf::Vector2i cell_size = tile_node->Data().GetCellSize();

	for (int x = 0; x < cell_size.x; x++)
	{
		for (int y = 0; y < cell_size.y; y++)
		{
			sf::Vector2i delta(x, y);
			m_tile_map.erase(cell_position + delta);
		}
	}
}

bool GridNode::IsHoldingTile() const
{
	return m_selected_tile != nullptr;
}

bool GridNode::IsInEditMode() const
{
	return m_editor_mode;
}

bool GridNode::IsInCreateMode() const
{
	return m_create_texture != Textures::kDefault;
}

/// <summary>
/// Vilandas - Checks if a tile will intersect another tile at the indicated cell position
/// </summary>
/// <param name="tile"></param>
/// <param name="cell_position"></param>
/// <returns></returns>
bool GridNode::TileIntersectsTile(TileNode* tile, sf::Vector2i cell_position) const
{
	const sf::Vector2i cell_size = tile->Data().GetCellSize();

	for (int x = 0; x < cell_size.x; x++)
	{
		for (int y = 0; y < cell_size.y; y++)
		{
			sf::Vector2i delta(x, y);
			if (CellContainsTile(cell_position + delta))
			{
				return true;
			}
		}
	}

	return false;
}

bool GridNode::CellContainsTile(sf::Vector2i cell_position) const
{
	return m_tile_map.count(cell_position);
}

/// <summary>
/// Jack/Vilandas
/// </summary>
/// <param name="cell_position"></param>
/// <returns></returns>
bool GridNode::CellPickable(sf::Vector2i cell_position)
{
	return CellContainsTile(cell_position)
		? m_tile_map[cell_position]->Data().IsPickable()
		: false;
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="tile"></param>
/// <returns></returns>
int GridNode::GetInventoryCount(const TileData& tile)
{
	if (m_inventory.count(tile))
	{
		return m_inventory[tile];
	}
	return 0;
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="tile"></param>
void GridNode::SelectFromInventory(TileData tile)
{
	if (!IsHoldingTile() && m_inventory.count(tile))
	{
		m_inventory_mode = true;
		SetNewTileSettings(tile.GetTexture());
	}
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="tile"></param>
/// <param name="count"></param>
void GridNode::AddToInventory(TileData& tile, int count)
{
	if (m_inventory.count(tile))
	{
		m_inventory[tile] += count;
	}
	else
	{
		m_inventory.emplace(tile, count);
	}
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="texture"></param>
/// <param name="count"></param>
void GridNode::AddToInventory(Textures texture, int count)
{
	TileData tile(texture);
	AddToInventory(tile, count);
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="tile"></param>
/// <param name="count"></param>
void GridNode::RemoveFromInventory(const TileData& tile, int count)
{
	if (m_inventory.count(tile))
	{
		m_inventory[tile] -= count;
		m_selected_label->SetText(std::to_string(m_inventory[tile]));

		if (m_inventory[tile] <= 0)
		{
			m_inventory.erase(tile);
			m_selected_label->SetText("");
		}
	}
}

/// <summary>
/// Jack/Vilandas - Convert a position to a cell position, accounting for transforms and camera zoom
/// </summary>
/// <param name="position"></param>
/// <returns></returns>
sf::Vector2i GridNode::GetCellPosition(sf::Vector2i position) const
{
	const sf::Vector2f pixel_position = m_window.mapPixelToCoords(position);

	return {
		static_cast<int>(pixel_position.x / m_cell_size),
		static_cast<int>(pixel_position.y / m_cell_size)
	};
}

sf::Vector2i GridNode::GetCellPosition(sf::Vector2f position) const
{
	return GetCellPosition(sf::Vector2i(position));
}

/// <summary>
/// Jack/Vilandas - Converts the mouse position to its relative cell position
/// </summary>
/// <returns></returns>
sf::Vector2i GridNode::MouseToCellPosition() const
{
	const sf::Vector2f pixel_position = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_camera.GetView());

	return {
		static_cast<int>(pixel_position.x / m_cell_size),
		static_cast<int>(pixel_position.y / m_cell_size)
	};
}

const Camera& GridNode::GetCamera() const
{
	return m_camera;
}

/// <summary>
/// Jack/Vilandas
/// </summary>
/// <param name="target"></param>
/// <param name="states"></param>
void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Draw grid lines
	if (m_editor_mode)
	{
		sf::RectangleShape horizontal_line(sf::Vector2f(m_cell_size * m_horizontal_cells, m_line_width));
		sf::RectangleShape vertical_line(sf::Vector2f(m_line_width, m_cell_size * m_vertical_cells));

		sf::Color color(255, 255, 255, 60);
		horizontal_line.setFillColor(color);
		vertical_line.setFillColor(color);

		for (int i = 0; i < m_vertical_cells; i++)
		{
			horizontal_line.setPosition(sf::Vector2f(0, i * m_cell_size));
			target.draw(horizontal_line, states);
		}

		for (int i = 0; i < m_horizontal_cells; i++)
		{
			vertical_line.setPosition(sf::Vector2f(i * m_cell_size, 0));
			target.draw(vertical_line, states);
		}
	}

	sf::RectangleShape square(sf::Vector2f(m_cell_size, m_cell_size));
	square.setPosition(sf::Vector2f(m_mouse_cell_position.x * m_cell_size, m_mouse_cell_position.y * m_cell_size));

	sf::Color select_color(sf::Color(150, 150, 255, 150));
	sf::Color red(sf::Color(255, 0, 0, 150));

	if (IsHoldingTile())
	{
		select_color = m_can_place
			? select_color
			: red;
	}
	else 
	{
		select_color =  !m_mouse_contains_tile || m_can_pickup
			? select_color
			: red;
	}

	square.setFillColor(select_color);

	target.draw(square, states);
	target.draw(m_background, states);

	if (m_editor_mode)
	{
		target.draw(m_editor_gui, states);
		target.draw(m_editor_inventory_gui, states);
		target.draw(m_editor_adder_gui, states);
	}
	else
	{
		target.draw(m_inventory_gui, states);
		target.draw(m_inventory_labels_gui, states);
	}
}

/// <summary>
/// Jack/Vilandas
/// </summary>
/// <param name="dt"></param>
/// <param name="commands"></param>
void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_mouse_cell_position = MouseToCellPosition();

	if (IsHoldingTile())
	{
		m_selected_tile->SetCellPosition(m_mouse_cell_position, m_cell_size);

		m_can_place = !TileIntersectsTile(m_selected_tile, m_mouse_cell_position);
	}
	else if (IsInCreateMode())
	{
		CreateTile();
	}
	else
	{
		if (CellContainsTile(m_mouse_cell_position))
		{
			m_mouse_contains_tile = true;
			m_can_pickup = m_editor_mode || CellPickable(m_mouse_cell_position);
		}
		else
		{
			m_mouse_contains_tile = false;
			m_can_pickup = false;
		}
	}

	const sf::Vector2f camera_position = GetCamera().getPosition();
	m_background.setPosition(camera_position + m_background_position);

	if (m_editor_mode)
	{
		m_editor_gui.setPosition(camera_position);
		m_editor_inventory_gui.setPosition(camera_position);
		m_editor_adder_gui.setPosition(camera_position);
	}
	else
	{
		m_inventory_gui.setPosition(camera_position);
		m_inventory_labels_gui.setPosition(camera_position);
	}
}

int GridNode::CoinCount() const
{
	return m_coin_count;
}

/// <summary>
/// Vilandas - Switch editing mode on or off (for level creation/editing)
/// </summary>
/// <param name="editor_mode"></param>
void GridNode::SetEditorMode(bool editor_mode)
{
	m_editor_mode = editor_mode;

	if (m_editor_mode)
	{
		LoadEditor();
	}
}

/// <summary>
/// Jack - Load data from .sav & .inv files to load the tile_map and the inventory
///	.sav: pickup-able | x_cell | y_cell | TextureID
///	.inv: TextureID | count
/// </summary>
/// <param name="path"></param>
void GridNode::LoadData(const std::string& path)
{
	m_file_path = path;

	std::ifstream save_data;
	std::string unfiltered;
	save_data.open(path + ".sav");

	while (std::getline(save_data, unfiltered))
	{
		std::string split = ",";
		int data[4];

		for (int& i : data)
		{
			i = std::stoi(unfiltered.substr(0, unfiltered.find(split)));
			unfiltered.erase(0, unfiltered.find(split));
			unfiltered.erase(0, 1);
			std::cout << unfiltered << std::endl;
		}

		const int x = data[1];
		const int y = data[2];
		const int textureID = data[3];
		if (textureID == static_cast<int>(Textures::kCoin)) 
		{
			AnimationData data = { static_cast<Textures>(textureID), 8, 8, 4, 1.f };
			std::unique_ptr<AnimatedTileNode> tile_node(new AnimatedTileNode(m_textures, GetSceneLayers(), static_cast<Textures>(textureID), data));
			tile_node->setPosition(static_cast<float>(16 * x), static_cast<float>(16 * y));
			AddTileNode(std::move(tile_node));
		} 
		else {
			std::unique_ptr<TileNode> tile_node(new TileNode(m_textures, GetSceneLayers(), static_cast<Textures>(textureID)));
			tile_node->setPosition(static_cast<float>(16 * x), static_cast<float>(16 * y));
			AddTileNode(std::move(tile_node));
		}
		
	}

	save_data.close();
	save_data.open(path + ".inv");

	while (std::getline(save_data, unfiltered))
	{
		std::string split = ",";
		int data[2];

		for (int& i : data)
		{
			i = std::stoi(unfiltered.substr(0, unfiltered.find(split)));
			unfiltered.erase(0, unfiltered.find(split));
			unfiltered.erase(0, 1);
			std::cout << unfiltered << std::endl;
		}

		const int textureID = data[0];
		const int count = data[1];

		AddToInventory(static_cast<Textures>(textureID), count);
	}

	if (!m_inventory.empty())
	{
		LoadInventoryGUI();
	}
}

/// <summary>
/// Jack - Save the tile_map, inventory and image of the level
/// </summary>
void GridNode::SaveData()
{
	std::ofstream save_data;
	save_data.open(m_file_path + ".sav", std::fstream::out);
	std::unordered_set<sf::Vector2i, Vector2iHash> saved;

	for (const auto& pair : m_tile_map)
	{
		if (!saved.count(pair.second->Data().GetCellPosition()))
		{
			save_data << pair.second->Data().ToSerial();
			saved.emplace(pair.second->Data().GetCellPosition());
		}
	}

	save_data.close();


	save_data.open(m_file_path + ".inv", std::fstream::out);
	for (const auto& pair : m_inventory)
	{
		save_data << static_cast<int>(pair.first.GetTexture()) << "," << pair.second << std::endl;
	}

	save_data.close();

	sf::Texture texture = sf::Texture();
	texture.create(1920, 1080);
	texture.update(m_window);
	sf::Image image = texture.copyToImage();
	image.saveToFile(m_file_path + ".png");
}

/// <summary>
/// Jack/Vilandas
/// </summary>
/// <param name="event"></param>
/// <param name="commands"></param>
void GridNode::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	//GUI events
	if (m_editor_mode)
	{
		m_editor_gui.HandleEvent(event);
		m_editor_inventory_gui.HandleEvent(event);
		m_editor_adder_gui.HandleEvent(event);
	}
	else
	{
		m_inventory_gui.HandleEvent(event);
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (IsInCreateMode())
			{
				ExitCreateMode();
			}
			else if (IsHoldingTile())
			{
				DropTile();
			}
			else
			{
				PickupTile();
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (!IsHoldingTile())
			{
				return;
			}

			if (m_inventory_mode)
			{
				if (IsHoldingTile() && DropTile())
				{
					const TileData tile(m_create_texture);
					RemoveFromInventory(tile, 1);

					if (!m_inventory.count(tile))
					{
						ExitCreateMode();
						m_inventory_mode = false;
						m_selected_button->SetSelectable(false);
					}
				}
			}
			else if (IsInCreateMode())
			{
				DropTile();
			}
			else if (IsHoldingTile())
			{
				DropTileAt(m_picked_up_position);
			}
		}
	}
}

/// <summary>
/// Jack/Vilandas - Create a tile based off the m_create_texture and select it to follow the mouse
/// </summary>
/// <returns></returns>
bool GridNode::CreateTile()
{
	if (!IsHoldingTile() && IsInCreateMode())
	{
		if (m_create_texture == Textures::kCoin)
		{
			AnimationData data = {m_create_texture, 8, 8, 4, 1.f};
			std::unique_ptr<AnimatedTileNode> tile_node(new AnimatedTileNode(m_textures, GetSceneLayers(), m_create_texture, data));
			m_selected_tile = tile_node.get();	
			GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
		}
		else {
			std::unique_ptr<TileNode> tile_node(new TileNode(m_textures, GetSceneLayers(), m_create_texture));
			m_selected_tile = tile_node.get();
			GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
		}
		
		return true;
	}
	return false;
}

/// <summary>
/// Jack/Vilandas - Attempt to pickup the tile at the current mouse position
/// </summary>
/// <returns></returns>
bool GridNode::PickupTile()
{
	if (m_can_pickup)
	{
		TileNode* tile = m_tile_map[m_mouse_cell_position];
		if (!tile->IsSelected())
		{
			m_picked_up_position = tile->Data().GetCellPosition();

			tile->Select();
			m_selected_tile = tile;
			RemoveTile(tile);

			return true;
		}
	}
	return false;
}

/// <summary>
/// Jack/Vilandas - Drop currently held tile at the mouse position
/// </summary>
/// <returns></returns>
bool GridNode::DropTile()
{
	if (IsHoldingTile() && m_can_place)
	{
		m_selected_tile->Deselect();
		m_selected_tile->SetCellPosition(m_mouse_cell_position, m_cell_size);
		AddTileNode(m_selected_tile, m_mouse_cell_position);

		m_selected_tile = nullptr;
		return true;
	}
	return false;
}

/// <summary>
/// Jack/Vilandas - Drop currently held tile at the indicated cell position
/// </summary>
/// <param name="cell_position"></param>
void GridNode::DropTileAt(sf::Vector2i cell_position)
{
	m_selected_tile->Deselect();
	m_selected_tile->SetCellPosition(cell_position, m_cell_size);
	AddTileNode(m_selected_tile, cell_position);

	m_selected_tile = nullptr;
}

/// <summary>
/// Vilandas - Load tiles in Inventory to the GUI
/// </summary>
void GridNode::LoadInventoryGUI()
{
	for (auto& pair : m_inventory)
	{
		Textures texture = pair.first.GetTexture();
		TileData tile(texture);

		auto label = std::make_shared<GUI::Label>("", m_fonts);

		auto button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, texture);
		button->SetToggle(true);
		button->setPosition(16, 292);
		button->SetCallback([this, tile, button, label]()
			{
				SelectFromInventory(tile);
				m_selected_button = button;
				m_selected_label = label;
			});

		const bool new_page = m_inventory_gui.Pack(button, 16);

		label->SetText(std::to_string(m_inventory[tile]));
		label->setPosition(button->getPosition().x + button->GetBoundingRect().width / 2, 310);
		label->setScale(0.5f, 0.5f);

		m_inventory_labels_gui.PackManual(label, new_page);
	}
}

/// <summary>
/// Vilandas - Load all tiles and other related gui items for use in editor mode
/// </summary>
void GridNode::LoadEditor()
{
	if (m_editor_loaded)
	{
		return;
	}
	m_editor_loaded = true;

	auto save_button = std::make_shared<GUI::Button>(m_textures, m_fonts, m_sounds);
	save_button->setScale(0.5f, 0.5f);
	save_button->setPosition(m_camera.getPosition().x + m_camera.GetBoundingRect().width - save_button->GetBoundingRect().width, 0);
	save_button->SetText("Save");
	save_button->SetCallback([this]()
		{
			this->SaveData();
		});

	m_editor_gui.Pack(save_button);

	for (int texture_index = static_cast<int>(Textures::kWooden_2x1); texture_index <= static_cast<int>(Textures::kCoin); texture_index++)
	{
		auto texture = static_cast<Textures>(texture_index);

		auto button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, texture);
		button->SetToggle(true);
		button->setPosition(16, 292);
		button->SetCallback([this, texture, button]()
			{
				SetNewTileSettings(texture);
				m_selected_button = button;
			});

		const bool new_page = m_editor_inventory_gui.Pack(button, 16);

		auto label = std::make_shared<GUI::Label>("", m_fonts);
		label->setPosition(button->getPosition().x + button->GetBoundingRect().width / 2, 310);
		label->setScale(0.5f, 0.5f);

		auto remove_button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, Textures::kRemoveButton);
		remove_button->setPosition(button->getPosition().x - 5, 310);
		remove_button->SetCallback([this, label, texture]()
			{
				TileData tile(texture, true);
				m_selected_label = label;
				RemoveFromInventory(tile);
			});

		auto add_button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, Textures::kAddButton);
		add_button->setPosition(button->getPosition().x + button->GetBoundingRect().width, 310);
		add_button->SetCallback([this, label, texture]()
			{
				TileData tile(texture, true);
				AddToInventory(tile);
				label->SetText(std::to_string(m_inventory[tile]));
			});

		m_editor_adder_gui.PackManual(remove_button, new_page);
		m_editor_adder_gui.PackManual(label);
		m_editor_adder_gui.PackManual(add_button);
	}
}
