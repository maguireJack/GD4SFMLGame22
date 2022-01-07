#pragma once
#include <functional>

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"

#include <memory>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class TexturedButton : public Component
	{
	public:
		typedef std::shared_ptr<TexturedButton> Ptr;
		typedef std::function<void()> Callback;

	public:
		TexturedButton(const FontHolder& fonts, const TextureHolder& textures, Textures texture);
		void SetCallback(Callback callback);
		void SetTextObject(const sf::Text& text);
		void SetText(const std::string& text);
		void SetToggle(bool flag);

		sf::FloatRect GetBoundingRect() const override;
		bool IsSelectable() const override;
		void Activate() override;
		void HandleEvent(const sf::Event& event) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		Callback m_callback;
		sf::Sprite m_sprite;
		sf::Text m_text;
		bool m_is_toggle;
	};
}

