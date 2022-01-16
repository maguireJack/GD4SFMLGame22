#include "GridNode.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

#include "Application.hpp"
#include "Button.hpp"
#include "Label.hpp"
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
	, m_editor_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
	, m_inventory_adder_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0)
	, m_inventory_gui(window, fonts, camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
	, m_selected_button(nullptr)
	, m_editor_mode(editor_mode)
	, m_inventory_mode(false)
	, m_can_place(true)
	, m_can_pickup(false)
	, m_mouse_contains_tile(false)
	, m_mouse_cell_position(0, 0)
	, m_selected_tile(nullptr)
	, m_create_texture(Textures::kDefault)
{
	m_background.setFillColor(sf::Color(0, 0, 0, 150));
	m_background.setSize(sf::Vector2f(576, 48));
	m_background_position = sf::Vector2f(0, 276);

	if (!editor_mode)
	{
		return;
	}

	for (int texture_index = static_cast<int>(Textures::kWooden_2x1); texture_index <= static_cast<int>(Textures::kGrassTiles24); texture_index++)
	{
		auto texture = static_cast<Textures>(texture_index);

		auto button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, texture);
		button->SetToggle(true);
		button->setPosition(16, 292);
		button->SetCallback([this, texture, button]()
			{
				SetNewTileSettings(texture);
				m_inventory_gui.DeactivateAllExcept(button);
				m_selected_button = button;
			});

		const bool new_page = m_editor_gui.Pack(button, 16);

		auto label = std::make_shared<GUI::Label>("", m_fonts);
		label->setPosition(button->getPosition().x + button->GetBoundingRect().width / 2, 310);
		label->setScale(0.5f, 0.5f);

		auto remove_button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, Textures::kRemoveButton);
		remove_button->setPosition(button->getPosition().x - 5, 310);
		remove_button->SetCallback([this, label, texture]()
			{
				TileData tile(texture, true);
				RemoveFromInventory(tile);

				if (m_inventory.count(tile))
				{
					label->SetText(std::to_string(m_inventory[tile]));
				}
				else
				{
					label->SetText("");
				}
			});

		auto add_button = std::make_shared<GUI::TexturedButton>(m_fonts, m_textures, Textures::kAddButton);
		add_button->setPosition(button->getPosition().x + button->GetBoundingRect().width, 310);
		add_button->SetCallback([this, label, texture]()
			{
				TileData tile(texture, true);
				AddToInventory(tile);
				label->SetText(std::to_string(m_inventory[tile]));
			});

		m_inventory_adder_gui.PackManual(remove_button, new_page);
		m_inventory_adder_gui.PackManual(label);
		m_inventory_adder_gui.PackManual(add_button);
	}
}

void GridNode::SetNewTileSettings(Textures texture)
{
	if (m_selected_tile != nullptr)
	{
		if (m_editor_mode)
		{
			m_editor_gui.DeactivateAll();
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

void GridNode::ExitCreateMode()
{
	SetNewTileSettings(Textures::kDefault);
}

void GridNode::AddTileNode(std::unique_ptr<TileNode> tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	const sf::Vector2i cell_size = tile_node->Data().GetCellSize();

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

void GridNode::AddTileNode(TileNode* tile_node, sf::Vector2i cell_position)
{
	const sf::Vector2i cell_size = tile_node->Data().GetCellSize();

	for (int x = 0; x < cell_size.x; x++)
	{
		for (int y = 0; y < cell_size.y; y++)
		{
			sf::Vector2i delta(x, y);
			m_tile_map[cell_position + delta] = tile_node;
		}
	}
}

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

bool GridNode::IsInCreateMode() const
{
	return m_create_texture != Textures::kDefault;
}

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

bool GridNode::CellPickable(sf::Vector2i cell_position)
{
	return CellContainsTile(cell_position)
		? m_tile_map[cell_position]->Data().IsPickable()
		: false;
}

int GridNode::GetInventoryCount(const TileData& tile)
{
	if (m_inventory.count(tile))
	{
		return m_inventory[tile];
	}
	return 0;
}

void GridNode::SelectFromInventory(TileData& tile)
{
	if (!IsHoldingTile() && m_inventory.count(tile))
	{
		m_inventory_mode = true;
		SetNewTileSettings(tile.GetTexture());
	}
}

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

void GridNode::AddToInventory(Textures texture, int count)
{
	TileData tile(texture);
	AddToInventory(tile, count);
}

void GridNode::RemoveFromInventory(const TileData& tile, int count)
{
	if (m_inventory.count(tile))
	{
		m_inventory[tile] -= count;

		if (m_inventory[tile] <= 0)
		{
			m_inventory.erase(tile);
		}
	}
}

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

void GridNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
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
		target.draw(m_inventory_adder_gui, states);
	}
	else
	{
		target.draw(m_inventory_gui, states);
	}
}

void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_mouse_cell_position = MouseToCellPosition();

	if (m_selected_tile != nullptr)
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
	m_editor_gui.setPosition(camera_position);
	m_inventory_gui.setPosition(camera_position);
	m_inventory_adder_gui.setPosition(camera_position);
}

void GridNode::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	std::ofstream save_data;
	save_data.open("test.txt", std::fstream::out);

	for (auto x : m_tile_map)
	{
		save_data << x.second->Data().ToSerial();
	}

	if (m_editor_mode)
	{
		m_editor_gui.HandleEvent(event);
		m_inventory_adder_gui.HandleEvent(event);
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
		/*else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (m_tile_map[m_mouse_cell_position])
			{
				if (m_selected_tile == nullptr) {
					std::cout << "Hovering" << std::endl;
					TileNode copied_tile(m_tile_map[m_mouse_cell_position]);

					if (!copied_tile.GetTile()->IsSelected())
					{
						copied_tile.GetTile()->Select();
						m_selected_tile = &copied_tile;
					}
				}

			}
		}
		*/

	}
	save_data.close();
}

bool GridNode::CreateTile()
{
	if (!IsHoldingTile() && IsInCreateMode())
	{
		std::unique_ptr<TileNode> tile_node(new TileNode(m_textures, GetSceneLayers(), m_create_texture));
		m_selected_tile = tile_node.get();
		GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));

		return true;
	}
	return false;
}

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

void GridNode::DropTileAt(sf::Vector2i cell_position)
{
	m_selected_tile->Deselect();
	m_selected_tile->SetCellPosition(cell_position, m_cell_size);
	AddTileNode(m_selected_tile, cell_position);

	m_selected_tile = nullptr;
}