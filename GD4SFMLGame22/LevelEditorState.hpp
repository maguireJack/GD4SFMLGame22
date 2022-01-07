#pragma once
#include "GameState.hpp"
#include "World.hpp"

class LevelEditorState : public GameState
{
public:
	LevelEditorState(StateStack& stack, const Context& context);
	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;
};
