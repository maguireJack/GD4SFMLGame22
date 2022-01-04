#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "PlatformType.hpp"
#include "ResourceIdentifiers.hpp"

class Tile 
{
public:
	Tile(PlatformType platform, const TextureHolder& textures);
	bool IsSelected();
	Tile Select();
	void Deselect();
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const;

private:
	PlatformType m_platform;
	sf::Vector2f m_follow_target;
	bool m_selected = false;
	bool m_follow = false;
	sf::Sprite m_sprite;
	sf::Vector2f follow_mouse;
};