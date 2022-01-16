#include "TileNode.hpp"

#include <iostream>
#include <ostream>

#include "Utility.hpp"

/// <summary>
/// TileNode
/// TileNode exists within the grid, refers to each tile as a SceneNode.
/// </summary>
/// <param name="textures">Texture Holder</param>
/// <param name="scene_layers">Scene Layers</param>
/// <param name="texture">Texture of the TileNode</param>
/// <param name="pickable">Pickable</param>
TileNode::TileNode(
	const TextureHolder& textures,
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	Textures texture,
	bool pickable)
	: SceneNode(scene_layers)
	, m_data(texture, pickable)
	, m_selected(false)
	, m_destroy(false)
	, m_sprite(textures.Get(texture))
	, m_delta_time_in_seconds(0)
{
}

unsigned TileNode::GetCategory() const
{
	return Category::kPlatform;
}

sf::FloatRect TileNode::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

sf::Vector2f TileNode::GetVelocity() const
{
	return m_velocity;
}

float TileNode::GetDeltaTimeInSeconds() const
{
	return m_delta_time_in_seconds;
}

/// <summary>
/// Check if the tile is selected
/// </summary>
/// <returns>Selected</returns>
bool TileNode::IsSelected() const
{
	return m_selected;
}

/// <summary>
/// Selects the current Tile
/// </summary>
void TileNode::Select()
{
	std::cout << "Selected" << std::endl;
	m_selected = true;
	m_sprite.setColor(sf::Color(255, 255, 255, 100));
}

/// <summary>
/// Deselects the current Tile
/// </summary>
void TileNode::Deselect()
{
	std::cout << "Deselected" << std::endl;
	m_selected = false;
	m_sprite.setColor(sf::Color(255, 255, 255, 255));
}

void TileNode::Destroy()
{
	m_destroy = true;
}

/// <summary>
/// Sets the current TileNodes position within the Grid
/// </summary>
/// <param name="position">Position relative to the world, not the grid</param>
/// <param name="cell_size">Size of each square on the grid</param>
void TileNode::SetCellPosition(sf::Vector2i position, float cell_size = 16)
{
	m_data.SetCellPosition(position);
	position += m_data.GetCellOffset();
	setPosition(sf::Vector2f(position) * cell_size);
}

/// <summary>
/// Returns the underlying TileData for the current Node
/// </summary>
/// <returns></returns>
TileData TileNode::Data() const
{
	return m_data;
}

bool TileNode::IsDestroyed() const
{
	return m_destroy;
}

/// <summary>
/// Used for Moving Platforms, activates its movement path
/// </summary>
void TileNode::ActivateVerticalMovement()
{
	if (m_velocity == sf::Vector2f())
	{
		m_velocity = sf::Vector2f(0, -30);
	}
}

void TileNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void TileNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_delta_time_in_seconds = dt.asSeconds();

	if (m_velocity != sf::Vector2f())
	{
		move(m_velocity * dt.asSeconds());

		const sf::Vector2i travel_distance = Data().GetCellSize() - sf::Vector2i(1, 1);

		const sf::Vector2i cell_position = m_data.GetCellPosition();
		const sf::Vector2i cell_offset = m_data.GetCellOffset();
		const sf::Vector2f position = sf::Vector2f(cell_position * 16);
		const sf::Vector2f offset_position = position + sf::Vector2f(cell_offset * 16);
		auto x = getPosition();

		if (getPosition().x > position.x || getPosition().y < position.y)
		{
			m_velocity *= -1.f;
		}
		else if (getPosition().x < offset_position.x || getPosition().y > offset_position.y)
		{
			m_velocity = sf::Vector2f();
			SetCellPosition(cell_position);
		}
	}
}
