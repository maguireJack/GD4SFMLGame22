#include "GameState.hpp"

#include "Player.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, m_world(*context.window, *context.fonts, *context.grid)
	, m_player(*context.player)
	, m_grid(*context.grid)
{

}

void GameState::Draw()
{
	m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{
	m_world.Update(dt);
	CommandQueue& commands = m_world.GetCommandQueue();

	m_player.HandleRealtimeInput(commands);
	return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleEvent(event, commands);
	m_grid.HandleEvent(event, commands);

	//Escape should bring up the Pause Menu
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		RequestStackPush(StateID::kPause);
	}
	return true;
}
