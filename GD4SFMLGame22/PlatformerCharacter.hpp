#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "AnimatedSpriteArtist.hpp"
#include "Entity.hpp"
#include "PlatformerCharacterType.hpp"
#include "ResourceIdentifiers.hpp"
#include "TextNode.hpp"

class PlatformerCharacter : public Entity
{
public:
	PlatformerCharacter(PlatformerCharacterType, const TextureHolder& textures, const FontHolder& fonts);
	unsigned GetCategory() const override;

	void UpdateTexts() const;
	sf::FloatRect GetBoundingRect() const override;

private:
	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	PlatformerCharacterType m_type;
	AnimatedSpriteArtist m_artist;

	TextNode* m_health_display;
};
