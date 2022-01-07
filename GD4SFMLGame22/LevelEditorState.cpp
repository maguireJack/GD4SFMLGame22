#include "LevelEditorState.hpp"

LevelEditorState::LevelEditorState(StateStack& stack, const Context& context)
	: GameState(stack, context)
{
}

void LevelEditorState::Draw()
{
	GameState::Draw();
}

bool LevelEditorState::Update(sf::Time dt)
{
	return GameState::Update(dt);
}

bool LevelEditorState::HandleEvent(const sf::Event& event)
{
	return GameState::HandleEvent(event);
}
