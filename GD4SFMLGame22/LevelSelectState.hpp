#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "ScrollableContainer.hpp"
#include "State.hpp"

class LevelSelectState : public State
{
public:
	LevelSelectState(StateStack& stack, Context context);

	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	GUI::ScrollableContainer m_gui_container;
};

