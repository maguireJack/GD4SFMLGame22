#include "State.hpp"

#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, sf::View& camera, Player& player, Grid& grid)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, camera(&camera)
, player(&player)
, grid(&grid)
{
}

State::State(StateStack& stack, Context context)
:m_stack(&stack)
, m_context(context)
{
}

State::~State()
{
}


void State::RequestStackPush(StateID state_id)
{
	m_stack->PushState(state_id);
}

void State::RequestStackPop()
{
	m_stack->PopState();
}

void State::RequestStackClear()
{
	m_stack->ClearStates();
}

State::Context State::GetContext() const
{
	return m_context;
}
