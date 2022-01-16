#include "Texture.hpp"

#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Utility.hpp"

namespace GUI
{
	Texture::Texture(const sf::Texture& texture)
		: m_texture(texture)
		, m_sprite(texture)
	{
		Utility::CentreOrigin(m_sprite);
	}

	Texture::Texture(std::string path)
	{
		m_texture.loadFromFile(path);
		m_sprite.setTexture(m_texture);
		Utility::CentreOrigin(m_sprite);
	}

	bool Texture::IsSelectable() const
	{
		return false;
	}

	void Texture::HandleEvent(const sf::Event& event)
	{
	}

	sf::FloatRect Texture::GetBoundingRect() const
	{
		return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
	}

	void Texture::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_sprite, states);
	}
}

