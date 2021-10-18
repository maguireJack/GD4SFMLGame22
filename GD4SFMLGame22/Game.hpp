#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.hpp"

class Game
{
public:
	Game(TextureHolder& game_textures);
	void Run();

private:
	void HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed);
	void ProcessEvents();
	void Update(sf::Time delta_time);
	float Length(const sf::Vector2f& vector2);
	sf::Vector2f Normalise(const sf::Vector2f input_vector);
	void Render();

	void UpdateStatistics(sf::Time elapsed_time);

private:
	static const float kPlayerSpeed;
	static const sf::Time kTimePerFrame;
	sf::RenderWindow m_window;
	sf::Texture m_texture;
	sf::Sprite m_player;
	sf::Font m_font;
	sf::Text m_statistics_text;
	sf::Time m_statistics_updatetime;

	std::size_t m_statistics_numframes;
	TextureHolder& m_textures;
	bool m_is_moving_up;
	bool m_is_moving_down;
	bool m_is_moving_left;
	bool m_is_moving_right;
};
