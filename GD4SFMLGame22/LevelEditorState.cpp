#include "LevelEditorState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

LevelEditorState::LevelEditorState(StateStack& stack, const Context& context) : GameState(stack, context), m_world(*context.window, *context.fonts)
, m_player(*context.player)
{
	
}

void LevelEditorState::Draw()
{
	m_world.Draw();
}

bool LevelEditorState::Update(sf::Time dt)
{
	return false;
}

bool LevelEditorState::HandleEvent(const sf::Event& event)
{
	return false;
}
