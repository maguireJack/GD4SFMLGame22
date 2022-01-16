#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.hpp"

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);
	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	sf::Text m_text;

	bool m_show_text;
	sf::Time m_text_effect_time;
};

