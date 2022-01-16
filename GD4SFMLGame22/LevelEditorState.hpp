#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

#include "ScrollableContainer.hpp"
#include "State.hpp"
#include "World.hpp"

class LevelEditorState : public State
{
public:
	LevelEditorState(StateStack& stack, Context context);
	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	World m_world;
	Player& m_player;
	Grid& m_grid;

	GUI::ScrollableContainer m_gui_container;
	sf::RectangleShape m_background;
	sf::Vector2f m_background_position;
};

