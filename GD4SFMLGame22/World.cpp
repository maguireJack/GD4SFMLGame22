#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <limits>

#include "GridNode.hpp"
#include "Tile.hpp"
#include "Utility.hpp"
#include "TileNode.hpp"

World::World(sf::RenderWindow& window, FontHolder& font, sf::View& camera, Grid& grid)
	: m_window(window)
	, m_camera(camera)
	, m_textures()
	, m_fonts(font)
	, m_grid(grid)
	, m_scenegraph()
	, m_scene_layers()
	, m_world_bounds(0.f, 0.f, 384, 432)
	, m_spawn_position(384/2, 216 + 216/2)
	, m_scrollspeed(-50.f)
{
	LoadTextures();
	BuildScene();
	m_camera.setCenter(m_spawn_position);
	m_camera.setSize(384, 216);
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	/*m_camera.move(0, m_scrollspeed * dt.asSeconds());*/

	/*m_player_aircraft->SetVelocity(0.f, 0.f);*/
	/*m_scene_layers[static_cast<int>(Layers::kJungle2)]->move(-0.1f,0);
	m_scene_layers[static_cast<int>(Layers::kJungle3)]->move(-0.2f,0);
	m_scene_layers[static_cast<int>(Layers::kJungle4)]->move(-0.3f,0);
	m_scene_layers[static_cast<int>(Layers::kJungle5)]->move(-0.4f,0);*/

	//Forward commands to the scenegraph until the command queue is empty
	while(!m_command_queue.IsEmpty())
	{
		m_scenegraph.OnCommand(m_command_queue.Pop(), dt);
	}

	HandleCollisions();
	//Remove all destroyed entities
	m_scenegraph.RemoveWrecks();

	//Apply movement
	m_scenegraph.Update(dt, m_command_queue);
	AdaptPlayerPosition();
}

void World::Draw()
{
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

void World::LoadTextures()
{
	m_textures.Load(Textures::kBrunoIdle, "Media/Textures/idle.png");

	m_textures.Load(Textures::kJungle1, "Media/Textures/Jungle/plx-1.png");
	m_textures.Load(Textures::kJungle2, "Media/Textures/Jungle/plx-2.png");
	m_textures.Load(Textures::kJungle3, "Media/Textures/Jungle/plx-3.png");
	m_textures.Load(Textures::kJungle4, "Media/Textures/Jungle/plx-4.png");
	m_textures.Load(Textures::kJungle5, "Media/Textures/Jungle/plx-5.png");

	m_textures.Load(Textures::kSky1, "Media/Textures/Sky/cloud_1.png");
	m_textures.Load(Textures::kSky2, "Media/Textures/Sky/cloud_2.png");
	m_textures.Load(Textures::kSky3, "Media/Textures/Sky/cloud_3.png");
	m_textures.Load(Textures::kSky4, "Media/Textures/Sky/cloud_4.png");
	m_textures.Load(Textures::kSky5, "Media/Textures/Sky/cloud_5.png");

	m_textures.Load(Textures::kTempTile, "Media/Textures/temp_tile.png");
}

void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{
		Category::Type category = i == static_cast<int>(Layers::kAir)
			? Category::Type::kScene
			: Category::Type::kNone;

		SceneNode::Ptr layer(new SceneNode(category));
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& jungle1 = m_textures.Get(Textures::kJungle1);
	sf::Texture& jungle2 = m_textures.Get(Textures::kJungle2);
	sf::Texture& jungle3 = m_textures.Get(Textures::kJungle3);
	sf::Texture& jungle4 = m_textures.Get(Textures::kJungle4);
	sf::Texture& jungle5 = m_textures.Get(Textures::kJungle5);

	sf::Texture& sky1 = m_textures.Get(Textures::kSky1);
	sf::Texture& sky2 = m_textures.Get(Textures::kSky2);
	sf::Texture& sky3 = m_textures.Get(Textures::kSky3);
	sf::Texture& sky4 = m_textures.Get(Textures::kSky4);
	sf::Texture& sky5 = m_textures.Get(Textures::kSky5);

	sf::Texture& temp_tile = m_textures.Get(Textures::kTempTile);

	////Add the background sprite to our scene
	std::unique_ptr<SpriteNode> jungle_sprite1(new SpriteNode(jungle1, Utility::GetIntRect(jungle1), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground1)]->AttachChild(std::move(jungle_sprite1));

	std::unique_ptr<SpriteNode> jungle_sprite2(new SpriteNode(jungle2, Utility::GetIntRect(jungle2), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground2)]->AttachChild(std::move(jungle_sprite2));

	std::unique_ptr<SpriteNode> jungle_sprite3(new SpriteNode(jungle3, Utility::GetIntRect(jungle3), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->AttachChild(std::move(jungle_sprite3));

	std::unique_ptr<SpriteNode> jungle_sprite4(new SpriteNode(jungle4, Utility::GetIntRect(jungle4), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->AttachChild(std::move(jungle_sprite4));

	std::unique_ptr<SpriteNode> jungle_sprite5(new SpriteNode(jungle5, Utility::GetIntRect(jungle5), m_world_bounds.left, m_world_bounds.top + 216));
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->AttachChild(std::move(jungle_sprite5));

	std::unique_ptr<SpriteNode> sky_sprite1(new SpriteNode(sky1, Utility::GetIntRect(sky1), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground1)]->AttachChild(std::move(sky_sprite1));

	std::unique_ptr<SpriteNode> sky_sprite2(new SpriteNode(sky2, Utility::GetIntRect(sky2), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground2)]->AttachChild(std::move(sky_sprite2));

	std::unique_ptr<SpriteNode> sky_sprite3(new SpriteNode(sky3, Utility::GetIntRect(sky3), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->AttachChild(std::move(sky_sprite3));

	std::unique_ptr<SpriteNode> sky_sprite4(new SpriteNode(sky4, Utility::GetIntRect(sky4), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->AttachChild(std::move(sky_sprite4));

	std::unique_ptr<SpriteNode> sky_sprite5(new SpriteNode(sky5, Utility::GetIntRect(sky5), m_world_bounds.left, m_world_bounds.top));
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->AttachChild(std::move(sky_sprite5));

	/*std::unique_ptr<SpriteNode> temp_tile_sprite(new SpriteNode(temp_tile, sf::IntRect(0, 0, temp_tile.getSize().x, temp_tile.getSize().y)));
	temp_tile_sprite->setPosition(100, 316);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(temp_tile_sprite));*/

	std::unique_ptr<GridNode> grid_node(new GridNode(m_grid));
	m_scene_layers[static_cast<int>(Layers::kGrid)]->AttachChild(std::move(grid_node));

	std::shared_ptr<Tile> tile(new Tile(PlatformType::kStatic, m_textures));
	std::unique_ptr<TileNode> tile_node(new TileNode(tile));
	tile_node->setPosition(16 * 5, 16 * 16);

	m_grid.AddTile(tile, tile_node->getPosition());
	m_scene_layers[static_cast<int>(Layers::kPlatforms)]->AttachChild(std::move(tile_node));
	

	//Add player's character
	std::unique_ptr<PlatformerCharacter> player(new PlatformerCharacter(PlatformerCharacterType::kBruno, m_textures, m_fonts));
	m_player = player.get();
	m_player->setPosition(m_spawn_position);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(player));
}

void World::AdaptPlayerPosition()
{
	//Keep the player on the screen
	//sf::FloatRect view_bounds = GetViewBounds();
	//const float border_distance = 40.f;
	//sf::Vector2f position = m_player_aircraft->getPosition();
	//position.x = std::max(position.x, view_bounds.left + border_distance);
	//position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
	//position.y = std::max(position.y, view_bounds.top + border_distance);
	//position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
	//m_player_aircraft->setPosition(position);

}

CommandQueue& World::GetCommandQueue()
{
	return m_command_queue;
}

sf::FloatRect World::GetViewBounds() const
{
	return sf::FloatRect(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
}

sf::FloatRect World::GetBattlefieldBounds() const
{
	//Return camera bounds + a small area at the top where enemies spawn offscreen
	sf::FloatRect bounds = GetViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

bool MatchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->GetCategory();
	unsigned int category2 = colliders.second->GetCategory();
	std::cout << category1 << category2 << std::endl;
	if(type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if(type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

// Should let each entity class decide how collision is handled
void World::HandleCollisions()
{
	//std::set<SceneNode::Pair> collision_pairs;
	//m_scenegraph.CheckSceneCollision(m_scenegraph, collision_pairs);
	//for(SceneNode::Pair pair : collision_pairs)
	//{
	//	if(MatchesCategories(pair, Category::Type::kPlayerAircraft, Category::Type::kEnemyAircraft))
	//	{
	//		auto& player = static_cast<Aircraft&>(*pair.first);
	//		auto& enemy = static_cast<Aircraft&>(*pair.second);
	//		//Collision
	//		player.Damage(enemy.GetHitPoints());
	//		enemy.Destroy();
	//	}

	//	else if (MatchesCategories(pair, Category::Type::kPlayerAircraft, Category::Type::kPickup))
	//	{
	//		auto& player = static_cast<Aircraft&>(*pair.first);
	//		auto& pickup = static_cast<Pickup&>(*pair.second);
	//		//Apply the pickup effect
	//		pickup.Apply(player);
	//		pickup.Destroy();
	//	}

	//	else if (MatchesCategories(pair, Category::Type::kPlayerAircraft, Category::Type::kEnemyProjectile) || MatchesCategories(pair, Category::Type::kEnemyAircraft, Category::Type::kAlliedProjectile))
	//	{
	//		auto& aircraft = static_cast<Aircraft&>(*pair.first);
	//		auto& projectile = static_cast<Projectile&>(*pair.second);
	//		//Apply the projectile damage to the plane
	//		aircraft.Damage(projectile.GetDamage());
	//		projectile.Destroy();
	//	}

	//  }
}

void World::DestroyEntitiesOutsideView()
{
	//Command command;
	//command.category = Category::Type::kEnemyAircraft | Category::Type::kProjectile;
	//command.action = DerivedAction<Entity>([this](Entity& e, sf::Time)
	//{
	//	//Does the object intersect with the battlefield
	//	if (!GetBattlefieldBounds().intersects(e.GetBoundingRect()))
	//	{
	//		std::cout << "Destroying the entity" << std::endl;
	//		e.Destroy();
	//	}
	//});
	//m_command_queue.Push(command);
}
