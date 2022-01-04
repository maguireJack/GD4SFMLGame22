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
	explicit World(sf::RenderWindow& window, FontHolder& font, sf::View& view, Grid& grid);
	void Update(sf::Time dt);
	void Draw();
	CommandQueue& GetCommandQueue();

private:
	void LoadTextures();
	void BuildScene();
	void AdaptPlayerPosition();

	sf::FloatRect GetViewBounds() const;
	sf::FloatRect GetBattlefieldBounds() const;
	void SelectTiles();
	void HandleCollisions();
	void DestroyEntitiesOutsideView();

private:
	struct SpawnPoint
	{
		
	};

public:
	std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& GetSceneLayers();
	

private:
	sf::RenderWindow& m_window;
	Camera m_camera;
	TextureHolder m_textures;
	FontHolder& m_fonts;
	Grid& m_grid;
	SceneNode m_scenegraph;
	std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)> m_scene_layers;
	CommandQueue m_command_queue;

	sf::FloatRect m_world_bounds;
	sf::Vector2f m_spawn_position;
	float m_scrollspeed;
	PlatformerCharacter* m_player;
};

