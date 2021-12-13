#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <limits>


#include "GridNode.hpp"
#include "Utility.hpp"

World::World(sf::RenderWindow& window, FontHolder& font)
	: m_window(window)
	, m_camera(window.getDefaultView())
	, m_textures()
	, m_fonts(font)
	, m_scenegraph()
	, m_scene_layers()
	, m_world_bounds(0.f, 0.f, m_camera.getSize().x, 2000)
	, m_spawn_position(m_camera.getSize().x/2.f, m_world_bounds.height - m_camera.getSize().y /2.f)
	, m_scrollspeed(-50.f)
{
	LoadTextures();
	BuildScene();
	std::cout << m_camera.getSize().x << m_camera.getSize().y << std::endl;
	m_camera.setCenter(m_spawn_position);
	//m_camera.setSize(384, 216);
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	/*m_camera.move(0, m_scrollspeed * dt.asSeconds());*/

	/*m_player_aircraft->SetVelocity(0.f, 0.f);*/
	/*m_scene_layers[static_cast<int>(Layers::kBackground2)]->move(-0.1f,0);
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->move(-0.2f,0);
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->move(-0.3f,0);
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->move(-0.4f,0);*/
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
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

void World::LoadTextures()
{
	m_textures.Load(Textures::kBruno, "Media/Textures/idle.png");
	m_textures.Load(Textures::kBackground1, "Media/Textures/plx-1.png");
	m_textures.Load(Textures::kBackground2, "Media/Textures/plx-2.png");
	m_textures.Load(Textures::kBackground3, "Media/Textures/plx-3.png");
	m_textures.Load(Textures::kBackground4, "Media/Textures/plx-4.png");
	m_textures.Load(Textures::kBackground5, "Media/Textures/plx-5.png");
}

void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{
		Category::Type category = (i == static_cast<int>(Layers::kAir)) ? Category::Type::kScene : Category::Type::kNone;
		SceneNode::Ptr layer(new SceneNode(category));
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture1 = m_textures.Get(Textures::kBackground1);
	sf::Texture& texture2 = m_textures.Get(Textures::kBackground2);
	sf::Texture& texture3 = m_textures.Get(Textures::kBackground3);
	sf::Texture& texture4 = m_textures.Get(Textures::kBackground4);
	sf::Texture& texture5 = m_textures.Get(Textures::kBackground5);
	sf::IntRect textureRect1(m_world_bounds);
	sf::IntRect textureRect2(m_world_bounds);
	sf::IntRect textureRect3(m_world_bounds);
	sf::IntRect textureRect4(m_world_bounds);
	sf::IntRect textureRect5(m_world_bounds);
	//Tile the texture to cover our world
	texture1.setRepeated(true);
	texture2.setRepeated(true);
	texture3.setRepeated(true);
	texture4.setRepeated(true);
	texture5.setRepeated(true);

	////Add the background sprite to our scene
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture1, textureRect1));
	background_sprite->setScale(1.f, 3.f);
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground1)]->AttachChild(std::move(background_sprite));

	std::unique_ptr<SpriteNode> background_sprite2(new SpriteNode(texture2, textureRect2));
	background_sprite2->setScale(1.f, 3.f);
	background_sprite2->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground2)]->AttachChild(std::move(background_sprite2));

	std::unique_ptr<SpriteNode> background_sprite3(new SpriteNode(texture3, textureRect3));
	background_sprite3->setScale(1.f, 3.f);
	background_sprite3->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground3)]->AttachChild(std::move(background_sprite3));

	std::unique_ptr<SpriteNode> background_sprite4(new SpriteNode(texture4, textureRect4));
	background_sprite4->setScale(1.f, 3.f);
	background_sprite4->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground4)]->AttachChild(std::move(background_sprite4));

	std::unique_ptr<SpriteNode> background_sprite5(new SpriteNode(texture5, textureRect5));
	background_sprite5->setScale(1.f, 3.f);
	background_sprite5->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground5)]->AttachChild(std::move(background_sprite5));

	std::unique_ptr<GridNode> grid(new GridNode(m_world_bounds.width, m_world_bounds.height, 28));
	grid->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(grid));

	////Add player's aircraft
	//std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::kEagle, m_textures, m_fonts));
	//m_player_aircraft = leader.get();
	//m_player_aircraft->setPosition(m_spawn_position);
	//m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(leader));

	// //Add two escorts
	// std::unique_ptr<Aircraft> leftEscort(new Aircraft(AircraftType::kRaptor, m_textures, m_fonts));
	// leftEscort->setPosition(-80.f, 50.f);
	// m_player_aircraft->AttachChild(std::move(leftEscort));
	//
	// std::unique_ptr<Aircraft> rightEscort(new Aircraft(AircraftType::kRaptor, m_textures, m_fonts));
	// rightEscort->setPosition(80.f, 50.f);
	// m_player_aircraft->AttachChild(std::move(rightEscort));

}

CommandQueue& World::getCommandQueue()
{
	return m_command_queue;
}

void World::AdaptPlayerPosition()
{
	////Keep the player on the screen
	//sf::FloatRect view_bounds(m_camera.getCenter() - m_camera.getSize() / 2.f, m_camera.getSize());
	//const float border_distance = 40.f;
	//sf::Vector2f position = m_player_aircraft->GetWorldPosition();
	//position.x = std::max(position.x, view_bounds.left + border_distance);
	//position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
	//position.y = std::max(position.y, view_bounds.top + border_distance);
	//position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
	//m_player_aircraft->setPosition(position);

}

void World::AdaptPlayerVelocity()
{
	//sf::Vector2f velocity = m_player_aircraft->GetVelocity();
	////if moving diagonally then reduce velocity
	//if (velocity.x != 0.f && velocity.y != 0.f)
	//{
	//	m_player_aircraft->SetVelocity(velocity / std::sqrt(2.f));
	//}
	////Add scrolling velocity
	//m_player_aircraft->Accelerate(0.f, m_scrollspeed);
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

void World::SpawnEnemies()
{
	////Spawn an enemy when they are relevant - they are relevant when they enter the battlefield bounds
	//while(!m_enemy_spawn_points.empty() && m_enemy_spawn_points.back().m_y > GetBattlefieldBounds().top)
	//{
	//	SpawnPoint spawn = m_enemy_spawn_points.back();
	//	std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.m_type, m_textures, m_fonts));
	//	enemy->setPosition(spawn.m_x, spawn.m_y);
	//	enemy->setRotation(180.f);
	//	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(enemy));

	//	m_enemy_spawn_points.pop_back();
	//	
	//}
}

//void World::AddEnemy(AircraftType type, float relX, float relY)
//{
//	SpawnPoint spawn(type, m_spawn_position.x + relX, m_spawn_position.y - relY);
//	m_enemy_spawn_points.emplace_back(spawn);
//}

//void World::AddEnemies()
//{
//	//Add all enemies
//	AddEnemy(AircraftType::kRaptor, 0.f, 500.f);
//	AddEnemy(AircraftType::kRaptor, 0.f, 1000.f);
//	AddEnemy(AircraftType::kRaptor, 100.f, 1100.f);
//	AddEnemy(AircraftType::kRaptor, -100.f, 1100.f);
//	AddEnemy(AircraftType::kAvenger, -70.f, 1400.f);
//	AddEnemy(AircraftType::kAvenger, 70.f, 1400.f);
//	AddEnemy(AircraftType::kAvenger, 70.f, 1600.f);
//
//	//Sort according to y value so that lower enemies are checked first
//	std::sort(m_enemy_spawn_points.begin(), m_enemy_spawn_points.end(), [](SpawnPoint lhs, SpawnPoint rhs)
//	{
//		return lhs.m_y < rhs.m_y;
//	});
//}

void World::GuideMissiles()
{
	//// Setup command that stores all enemies in mActiveEnemies
	//Command enemyCollector;
	//enemyCollector.category = Category::kEnemyAircraft;
	//enemyCollector.action = DerivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
	//{
	//	if (!enemy.IsDestroyed())
	//		m_active_enemies.push_back(&enemy);
	//});

	//// Setup command that guides all missiles to the enemy which is currently closest to the player
	//Command missileGuider;
	//missileGuider.category = Category::kAlliedProjectile;
	//missileGuider.action = DerivedAction<Projectile>([this](Projectile& missile, sf::Time)
	//{
	//	// Ignore unguided bullets
	//	if (!missile.IsGuided())
	//		return;

	//	float minDistance = std::numeric_limits<float>::max();
	//	Aircraft* closestEnemy = nullptr;

	//	// Find closest enemy
	//	for(Aircraft * enemy :  m_active_enemies)
	//	{
	//		float enemyDistance = distance(missile, *enemy);

	//		if (enemyDistance < minDistance)
	//		{
	//			closestEnemy = enemy;
	//			minDistance = enemyDistance;
	//		}
	//	}

	//	if (closestEnemy)
	//		missile.GuideTowards(closestEnemy->GetWorldPosition());
	//});

	//// Push commands, reset active enemies
	//m_command_queue.Push(enemyCollector);
	//m_command_queue.Push(missileGuider);
	//m_active_enemies.clear();
}

bool MatchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->GetCategory();
	unsigned int category2 = colliders.second->GetCategory();

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


	//}
}

//void World::DestroyEntitiesOutsideView()
//{
//	Command command;
//	command.category = Category::Type::kAircraft | Category::Type::kProjectile;
//	command.action = DerivedAction<Entity>([this](Entity& e, sf::Time)
//	{
//		//Does the object intersect with the battlefield
//		if (!GetBattlefieldBounds().intersects(e.GetBoundingRect()))
//		{
//			e.Destroy();
//		}
//	});
//	m_command_queue.Push(command);
//}
