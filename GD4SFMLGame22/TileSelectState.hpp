#pragma once
#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TileSelectState : public State 
{
	public:
		TileSelectState(StateStack& stack, Context context);
		virtual void		Draw();
		virtual bool		Update(sf::Time dt);
		virtual bool		HandleEvent(const sf::Event& event);

	private:
		sf::Sprite				m_background_sprite;
		sf::Text				m_paused_text;
		std::vector<sf::Sprite> m_sprite_list;
};