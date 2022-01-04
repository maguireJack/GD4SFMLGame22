#pragma once
#include <vector>

#include "AnimatedSprite.hpp"

class AnimatedSpriteArtist
{
public:
	AnimatedSpriteArtist(std::vector<AnimatedSprite> sprite_states);
	AnimatedSpriteArtist(const std::vector<AnimationData>& data, const TextureHolder& textures);

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void UpdateCurrent(sf::Time dt);

	AnimatedSprite CurrentSpriteState() const;
	sf::FloatRect GetBoundingRect() const;
	void ChangeState(int index);
	void Pause();
	void Play();
	void Flipped(bool flipped);

private:
	AnimatedSprite& CurrentSpriteState();

private:
	std::vector<AnimatedSprite> m_sprite_states;
	int m_current_sprite_index;
	bool m_flipped;
}; 