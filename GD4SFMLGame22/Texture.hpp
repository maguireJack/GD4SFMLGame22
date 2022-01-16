#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Component.hpp"


namespace GUI
{
	class Texture : public GUI::Component
	{
	public:
		typedef std::shared_ptr<Texture> Ptr;
	public:
		Texture(const sf::Texture& texture);
		Texture(std::string path);
		bool IsSelectable() const override;
		void HandleEvent(const sf::Event& event) override;
		sf::FloatRect GetBoundingRect() const override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Texture m_texture;
		sf::Sprite m_sprite;
	};
}

