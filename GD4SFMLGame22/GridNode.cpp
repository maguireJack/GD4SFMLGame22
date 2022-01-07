#include "GridNode.hpp"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Application.hpp"

GridNode::GridNode(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	const sf::RenderWindow& window,
	const TextureHolder& textures,
	const Camera& camera,
	int horizontal_cells,
	int vertical_cells,
	float cell_size,
	float line_width)
	: SceneNode(scene_layers)
	, m_window(window)
	, m_textures(textures)
	, m_camera(camera)
	, m_horizontal_cells(horizontal_cells)
	, m_vertical_cells(vertical_cells)
	, m_cell_size(cell_size)
	, m_line_width(line_width)
	, m_can_place(true)
	, m_mouse_cell_position(0, 0)
	, m_selected_tile(nullptr)
	, m_create_texture(Textures::kDefault)
	, m_create_type(PlatformType::kNone)
{
}

void GridNode::SetNewTileSettings(PlatformType type, Textures texture)
{
	m_create_type = type;
	m_create_texture = texture;
}

void GridNode::AddTileNode(std::unique_ptr<TileNode> tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map[cell_position] = tile_node.get();

	GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
}

void GridNode::RemoveTile(const TileNode* tile_node)
{
	const sf::Vector2i cell_position = GetCellPosition(tile_node->getPosition());
	m_tile_map.erase(cell_position);
}

bool GridNode::IsHoldingTile() const
{
	return m_selected_tile != nullptr;
}

bool GridNode::IsInCreateMode() const
{
	return m_create_type != PlatformType::kNone;
}

bool GridNode::CellContainsTile(sf::Vector2i cell_position) const
{
	return m_tile_map.count(cell_position);
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

	sf::Color select_color = m_can_place
		? sf::Color(255, 255, 255, 150)
		: sf::Color(255, 0, 0, 150);

	square.setFillColor(select_color);

	target.draw(square, states);
}

void GridNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_mouse_cell_position = MouseToCellPosition();

	if (m_selected_tile != nullptr)
	{
		m_selected_tile->SetCellPosition(m_mouse_cell_position, m_cell_size);

		m_can_place = !CellContainsTile(m_mouse_cell_position);
	}
	else if (IsInCreateMode())
	{
		CreateTile();
	}
}

void GridNode::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (IsHoldingTile())
			{
				DropTile();
			}
			else if (CellContainsTile(m_mouse_cell_position))
			{
				PickupTile();
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (IsInCreateMode())
			{
				SetNewTileSettings(PlatformType::kNone, Textures::kDefault);
				m_selected_tile->Destroy();
				m_selected_tile = nullptr;
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
}

void GridNode::CreateTile()
{
	if (!IsHoldingTile() && IsInCreateMode())
	{
		std::unique_ptr<TileNode> tile_node(new TileNode(GetSceneLayers(), m_textures.Get(m_create_texture), m_create_type));
		m_selected_tile = tile_node.get();
		GetSceneLayers()[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
	}
}

void GridNode::PickupTile()
{
	TileNode* tile = m_tile_map[m_mouse_cell_position];
	if (!tile->IsSelected())
	{
		m_picked_up_position = m_mouse_cell_position;

		tile->Select();
		m_selected_tile = tile;
		m_tile_map.erase(m_mouse_cell_position);
	}
}

void GridNode::DropTile()
{
	if (m_can_place)
	{
		m_selected_tile->Deselect();
		m_selected_tile->SetCellPosition(m_mouse_cell_position, m_cell_size);
		m_tile_map[m_mouse_cell_position] = m_selected_tile;

		m_selected_tile = nullptr;
	}
}

void GridNode::DropTileAt(sf::Vector2i cell_position)
{
	m_selected_tile->Deselect();
	m_selected_tile->SetCellPosition(cell_position, m_cell_size);
	m_tile_map[cell_position] = m_selected_tile;

	m_selected_tile = nullptr;
}