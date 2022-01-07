#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Layers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

#include "Camera.hpp"
#include "CommandQueue.hpp"
#include "Grid.hpp"
#include "PlatformerCharacter.hpp"

//Foward
namespace sf
{
	class RenderWindow;
}


class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& font, Camera& camera, Grid& grid);
	void Update(sf::Time dt);
	void Draw();
	CommandQueue& GetCommandQueue();

private:
	void BuildScene();

	sf::IntRect GetBackgroundRect(sf::Texture& texture) const;

	void DestroyEntitiesOutsideView();

private:
	struct SpawnPoint
	{
		
	};

private:
	sf::RenderWindow& m_window;
	TextureHolder& m_textures;
	FontHolder& m_fonts;
	Camera& m_camera;
	Grid& m_grid;
	std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)> m_scene_layers;
	SceneNode m_scenegraph;
	CommandQueue m_command_queue;

	sf::FloatRect m_world_bounds;
	sf::Vector2f m_spawn_position;
	float m_scrollspeed;
	PlatformerCharacter* m_player;
};

