#include "World.hpp"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Collision.hpp"
#include "GridNode.hpp"
#include "Utility.hpp"

World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& font, SoundPlayer& sounds, Camera& camera, Grid& grid)
	: m_window(window)
	, m_textures(textures)
	, m_fonts(font)
	, m_sounds(sounds)
	, m_camera(camera)
	, m_grid(grid)
	, m_scene_layers()
	, m_scenegraph(m_scene_layers)
	, m_world_bounds(0.f, 0.f, 768, 432)
	, m_spawn_position(20, 100)
	, m_player(nullptr)
{
	m_scene_texture.create(m_window.getSize().x, m_window.getSize().y);

	m_camera.SetCenter(m_spawn_position);
	m_camera.SetSize(384 * 1.5f, 216 * 1.5f);
	m_camera.SetBoundsConstraint(m_world_bounds);

	BuildScene();

	// background height = 216 x2 backgrounds
	// background width = 384 (set to repeated)
}

void World::Update(sf::Time dt)
{
	//Forward commands to the scenegraph until the command queue is empty
	while(!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}

	//Remove all destroyed entities
	m_scenegraph.RemoveWrecks();

	//Apply movement
	m_scenegraph.Update(dt, m_command_queue);
}

void World::Draw()
{
	if (PostEffect::IsSupported())
	{
		m_scene_texture.clear();
		m_scene_texture.setView(m_camera.GetView());
		m_scene_texture.draw(m_scenegraph);
		m_scene_texture.display();
		m_bloom_effect.Apply(m_scene_texture, m_window);
	}
	else
	{
		m_window.setView(m_camera.GetView());
		m_window.draw(m_scenegraph);
	}
}


void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{
		const Category::Type category = i == static_cast<int>(Layers::kAir)
			? Category::Type::kScene
			: Category::Type::kNone;

		SceneNode::Ptr layer(new SceneNode(m_scene_layers, category));
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& jungle0 = m_textures.Get(Textures::kJungle0);
	sf::Texture& jungle1 = m_textures.Get(Textures::kJungle1);
	sf::Texture& jungle2 = m_textures.Get(Textures::kJungle2);
	sf::Texture& jungle3 = m_textures.Get(Textures::kJungle3);
	sf::Texture& jungle4 = m_textures.Get(Textures::kJungle4);

	sf::Texture& sky0 = m_textures.Get(Textures::kSky0);
	sf::Texture& sky1 = m_textures.Get(Textures::kSky1);
	sf::Texture& sky2 = m_textures.Get(Textures::kSky2);
	sf::Texture& sky3 = m_textures.Get(Textures::kSky3);
	sf::Texture& sky4 = m_textures.Get(Textures::kSky4);

	////Add the background sprite to our scene
	std::unique_ptr<SpriteNode> jungle_sprite0(new SpriteNode(m_scene_layers, jungle0, GetBackgroundRect(jungle0), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground1)]->AttachChild(std::move(jungle_sprite0));

	std::unique_ptr<SpriteNode> jungle_sprite1(new SpriteNode(m_scene_layers, jungle1, GetBackgroundRect(jungle1), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground2)]->AttachChild(std::move(jungle_sprite1));

	std::unique_ptr<SpriteNode> jungle_sprite2(new SpriteNode(m_scene_layers, jungle2, GetBackgroundRect(jungle2), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->AttachChild(std::move(jungle_sprite2));

	std::unique_ptr<SpriteNode> jungle_sprite3(new SpriteNode(m_scene_layers, jungle3, GetBackgroundRect(jungle3), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->AttachChild(std::move(jungle_sprite3));

	std::unique_ptr<SpriteNode> jungle_sprite4(new SpriteNode(m_scene_layers, jungle4, GetBackgroundRect(jungle4), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->AttachChild(std::move(jungle_sprite4));


	std::unique_ptr<SpriteNode> sky_sprite0(new SpriteNode(m_scene_layers, sky0, GetBackgroundRect(sky0), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground1)]->AttachChild(std::move(sky_sprite0));

	std::unique_ptr<SpriteNode> sky_sprite1(new SpriteNode(m_scene_layers, sky1, GetBackgroundRect(sky1), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground2)]->AttachChild(std::move(sky_sprite1));

	std::unique_ptr<SpriteNode> sky_sprite2(new SpriteNode(m_scene_layers, sky2, GetBackgroundRect(sky2), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->AttachChild(std::move(sky_sprite2));

	std::unique_ptr<SpriteNode> sky_sprite3(new SpriteNode(m_scene_layers, sky3, GetBackgroundRect(sky3), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->AttachChild(std::move(sky_sprite3));

	std::unique_ptr<SpriteNode> sky_sprite4(new SpriteNode(m_scene_layers, sky4, GetBackgroundRect(sky4), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->AttachChild(std::move(sky_sprite4));

	/*std::unique_ptr<SpriteNode> temp_tile_sprite(new SpriteNode(temp_tile, sf::IntRect(0, 0, temp_tile.getSize().x, temp_tile.getSize().y)));
	temp_tile_sprite->setPosition(100, 316);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(temp_tile_sprite));*/

	std::unique_ptr<GridNode> grid_node(
		new GridNode(
			m_scene_layers,
			m_window,
			m_textures,
			m_fonts,
			m_sounds,
			m_camera,
			static_cast<int>(m_world_bounds.width)/16,
			static_cast<int>(m_world_bounds.height)/16,
			16,
			0.5f,
			false));

	m_grid.SetNode(grid_node.get());
	m_scene_layers[static_cast<int>(Layers::kGrid)]->AttachChild(std::move(grid_node));

	//Add player's character

	std::unique_ptr<PlatformerCharacter> player(new PlatformerCharacter(m_scene_layers, PlatformerCharacterType::kBruno, m_camera, m_textures, m_fonts, m_sounds));
	m_player = player.get();
	m_player->setPosition(m_spawn_position);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(player));
}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

/// <summary>
/// Checks if the player is within the world bounds
/// </summary>
/// <returns></returns>
bool World::HasAlivePlayer() const
{
	if (!m_grid.Node().IsInEditMode())
	{
		return m_player->GetBoundingRect().intersects(m_world_bounds);
	}
	return true;
}

/// <summary>
/// Checks if player has collected all coins in the level
/// </summary>
/// <returns></returns>
bool World::HasPlayerAchievedVictory() const
{
	if (!m_grid.Node().IsInEditMode())
	{
		return m_player->GetCollectedCoins() >= m_grid.Node().CoinCount();
	}
	return false;
}

sf::IntRect World::GetBackgroundRect(sf::Texture& texture) const
{
	texture.setRepeated(true);
	return { 0, 0, static_cast<int>(m_world_bounds.width), static_cast<int>(texture.getSize().y) };
}