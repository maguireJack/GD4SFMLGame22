#include "AnimatedSpriteArtist.hpp"

#include <cassert>
#include <utility>

#include "ResourceHolder.hpp"

AnimatedSpriteArtist::AnimatedSpriteArtist(std::vector<AnimatedSprite> sprite_states)
	: m_sprite_states(std::move(sprite_states))
	, m_current_sprite_index(0)
{
}

AnimatedSpriteArtist::AnimatedSpriteArtist(const std::vector<AnimationData>& data, const TextureHolder& textures)
	: m_current_sprite_index(0)
{
	for (auto& i : data)
	{
		m_sprite_states.emplace_back(i, textures.Get(i.m_texture));
	}
}

void AnimatedSpriteArtist::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	CurrentSpriteState().DrawCurrent(target, states);
}

void AnimatedSpriteArtist::UpdateCurrent(sf::Time dt)
{
	CurrentSpriteState().UpdateCurrent(dt);
}

AnimatedSprite AnimatedSpriteArtist::CurrentSpriteState() const
{
	return m_sprite_states[m_current_sprite_index];
}

sf::FloatRect AnimatedSpriteArtist::GetBoundingRect() const
{
	return CurrentSpriteState().GetBoundingRect();
}

void AnimatedSpriteArtist::ChangeState(int index)
{
	assert(index > 0 && index < m_sprite_states.size());

	CurrentSpriteState().Reset();
	m_current_sprite_index = index;
}

void AnimatedSpriteArtist::Pause()
{
	CurrentSpriteState().Pause();
}

void AnimatedSpriteArtist::Play()
{
	CurrentSpriteState().Play();
}

AnimatedSprite& AnimatedSpriteArtist::CurrentSpriteState()
{
	return m_sprite_states[m_current_sprite_index];
}