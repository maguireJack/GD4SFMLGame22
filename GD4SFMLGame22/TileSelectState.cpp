#include "TileSelectState.hpp"

#include "ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Utility.hpp"

TileSelectState::TileSelectState(StateStack& stack, Context context) : State(stack, context), m_background_sprite(), m_paused_text()
{
	sf::Font& font = context.fonts->Get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize();
	

	m_paused_text.setFont(font);
	m_paused_text.setString("Tile Select");
	m_paused_text.setCharacterSize(70);
	Utility::CentreOrigin(m_paused_text);
	m_paused_text.setPosition(0.5f * viewSize.x, 0.1f * viewSize.y);


}

void TileSelectState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(m_paused_text);
}

bool TileSelectState::Update(sf::Time)
{
	return false;
}

bool TileSelectState::HandleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::L)
	{
		// Escape pressed, remove itself to return to the game
		RequestStackPop();
	}

	if (event.key.code == sf::Keyboard::Escape)
	{
		// Escape pressed, remove itself to return to the game
		RequestStackPop();
	}

	return false;
}