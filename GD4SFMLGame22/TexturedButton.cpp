#include "TexturedButton.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include "Button.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace GUI
{
	TexturedButton::TexturedButton(const FontHolder& fonts, const TextureHolder& textures, Textures texture)
		: m_text("", fonts.Get(Fonts::Main), 16)
		, m_is_toggle(true)
	{
		m_sprite.setTexture(textures.Get(texture));
		sf::FloatRect bounds = m_sprite.getLocalBounds();
		m_text.setPosition(bounds.width / 2, bounds.height / 2);

	}

	void TexturedButton::SetCallback(Callback callback)
	{
		m_callback = std::move(callback);
	}

	void TexturedButton::SetTextObject(const sf::Text& text)
	{
		m_text = text;
	}

	void TexturedButton::SetText(const std::string& text)
	{
		m_text.setString(text);
		Utility::CentreOrigin(m_text);
	}

	void TexturedButton::SetToggle(bool flag)
	{
		m_is_toggle = flag;
	}

	sf::FloatRect TexturedButton::GetBoundingRect() const
	{
		return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
	}

	bool TexturedButton::IsSelectable() const
	{
		return true;
	}

	void TexturedButton::Activate()
	{
		if (m_is_toggle)
		{
			if (IsActive())
			{
				Deactivate();
			}
			else
			{
				Component::Activate();

				if (m_is_toggle)
				{
					m_callback();
				}
			}
		}
		else
		{
			Component::Activate();
			if (m_callback)
			{
				m_callback();
			}

			Deactivate();
		}
	}

	void TexturedButton::HandleEvent(const sf::Event& event)
	{
	}

	void TexturedButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_sprite, states);
		target.draw(m_text, states);

		if (IsActive())
		{
			sf::RectangleShape shape;
			shape.setFillColor(sf::Color(68, 160, 253, 150));
			shape.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
			target.draw(shape, states);
		}
		else if (IsSelected())
		{
			sf::RectangleShape shape;
			shape.setFillColor(sf::Color(68, 160, 253, 60));
			shape.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
			target.draw(shape, states);
		}
	}
}
