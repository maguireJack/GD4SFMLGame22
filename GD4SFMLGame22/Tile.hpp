#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.hpp"
#include "PlatformType.hpp"
#include "ResourceIdentifiers.hpp"

class Tile : public Entity
{
public:
	Tile(PlatformType platform, const TextureHolder& textures);
	bool IsSelected() const;
	bool HasSelected() const;
	virtual unsigned int GetCategory() const override;
	virtual sf::FloatRect GetBoundingRect();

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const override;

private:
	PlatformType m_platform;
	bool m_selected;
	sf::Sprite m_sprite;
};