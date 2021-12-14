#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "PlatformType.hpp"
#include "ResourceIdentifiers.hpp"

class Tile : public Entity
{
public:
	Tile(PlatformType platform, const TextureHolder& textures);
	Tile(PlatformType platform, const TextureHolder& textures, int hitpoints);
	bool IsSelected() const;
	bool HasSelected() const;
	virtual unsigned int GetCategory() const override;
	virtual sf::FloatRect GetBoundingRect();

private:
	PlatformType m_platform;
	sf::Sprite m_sprite;
	bool m_selected;
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

};

