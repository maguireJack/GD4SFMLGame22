#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

World::World(sf::RenderWindow& window)
	: m_window(window)
	, m_camera(window.getDefaultView())
	, m_textures()
	, m_scenegraph()
	, m_scene_layers()
	, m_world_bounds(0.f, 0.f, m_camera.getSize().x, 2000.f)
	, m_spawn_position(m_camera.getSize().x/2.f, m_world_bounds.height - m_camera.getSize().y /2.f)
	, m_scrollspeed(-50.f)
	, m_player_aircraft(nullptr)
{
	LoadTextures();
	BuildScene();
	std::cout << m_camera.getSize().x << m_camera.getSize().y << std::endl;
	m_camera.setCenter(m_spawn_position);
}

void World::Update(sf::Time dt)
{
	//Scroll the world
	m_camera.move(0, m_scrollspeed * dt.asSeconds());
	sf::Vector2f position = m_player_aircraft->getPosition();
	std::cout << "X: " <<position.x << "Y: " << position.y << std::endl;
	sf::Vector2f velocity = m_player_aircraft->GetVelocity();

	//If the player touche the x borders, flip velocity
	if(position.x <= m_world_bounds.left +150.f || position.x >= m_world_bounds.left + m_world_bounds.width -150.f)
	{
		velocity.x = -velocity.x;
		m_player_aircraft->SetVelocity(velocity);
	}

	//Apply movement
	m_scenegraph.Update(dt);
}

void World::Draw()
{
	m_window.setView(m_camera);
	m_window.draw(m_scenegraph);
}

void World::LoadTextures()
{
	m_textures.Load(Textures::kEagle, "Media/Textures/Eagle.png");
	m_textures.Load(Textures::kRaptor, "Media/Textures/Raptor.png");
	m_textures.Load(Textures::kDesert, "Media/Textures/Desert.png");
}

void World::BuildScene()
{
	//Initialize the different layers
	for (std::size_t i = 0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		m_scene_layers[i] = layer.get();
		m_scenegraph.AttachChild(std::move(layer));
	}

	//Prepare the background
	sf::Texture& texture = m_textures.Get(Textures::kDesert);
	sf::IntRect textureRect(m_world_bounds);
	//Tile the texture to cover our world
	texture.setRepeated(true);

	//Add the background sprite to our scene
	std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(texture, textureRect));
	background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
	m_scene_layers[static_cast<int>(Layers::kBackground)]->AttachChild(std::move(background_sprite));

	//Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(AircraftType::kEagle, m_textures));
	m_player_aircraft = leader.get();
	m_player_aircraft->setPosition(m_spawn_position);
	m_player_aircraft->SetVelocity(40.f, m_scrollspeed);
	m_scene_layers[static_cast<int>(Layers::kAir)]->AttachChild(std::move(leader));

	//Add two escorts
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(AircraftType::kRaptor, m_textures));
	leftEscort->setPosition(-80.f, 50.f);
	m_player_aircraft->AttachChild(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(AircraftType::kRaptor, m_textures));
	rightEscort->setPosition(80.f, 50.f);
	m_player_aircraft->AttachChild(std::move(rightEscort));
}

