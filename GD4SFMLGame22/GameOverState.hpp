#pragma once
#include <SFML/Graphics/Text.hpp>

#include "State.hpp"

class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);

	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Text m_game_over_text;
	sf::Time m_elapsed_time;
};

