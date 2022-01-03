#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "PlatformType.hpp"
#include "ResourceIdentifiers.hpp"

class Tile 
{
public:
	Tile(PlatformType platform, const TextureHolder& textures);
	bool IsSelected() const;
	bool HasSelected() const;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const;

private:
	PlatformType m_platform;
	bool m_selected;
	sf::Sprite m_sprite;
};