#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

#include "Container.hpp"
#include "State.hpp"
#include "World.hpp"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);
	virtual void Draw();
	virtual bool Update(sf::Time dt);
	virtual bool HandleEvent(const sf::Event& event);

private:
	World m_world;
	Player& m_player;
	Grid& m_grid;

	GUI::Container m_gui_container;
	sf::RectangleShape m_background;
	sf::Vector2f m_background_position;
};

