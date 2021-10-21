#include "Game.hpp"

const float Game::kPlayerSpeed = 100;
const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game(ResourceHolder<sf::Texture, Textures>& game_textures):m_window(sf::VideoMode(640, 480), "Starting"), m_textures(game_textures), m_texture(), m_player(), m_font(), m_statistics_text(), m_statistics_updatetime(), m_statistics_numframes(0),  m_is_moving_up(false), m_is_moving_down(false), m_is_moving_left(false), m_is_moving_right(false)
{
	m_player.setTexture(m_textures.Get(Textures::kAircraft));
	m_player.setPosition(100.f, 100.f);
	m_font.loadFromFile("Media/Fonts/Sansation.ttf");
	m_statistics_text.setFont(m_font);
	m_statistics_text.setPosition(5.f, 5.f);
	m_statistics_text.setCharacterSize(10);
}

void Game::Run()
{
	sf::Clock clock;
	sf::Time time_since_last_update = sf::Time::Zero;
	while(m_window.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		time_since_last_update += elapsedTime;

		while (time_since_last_update > kTimePerFrame)
		{
			time_since_last_update -= kTimePerFrame;
			ProcessEvents();
			Update(kTimePerFrame);
		}
		UpdateStatistics(elapsedTime);
		Render();
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				HandlePlayerInput(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				HandlePlayerInput(event.key.code, false);
				break;
			case sf::Event::Closed:
				m_window.close();
				break;
		}
	}
}

void Game::Update(sf::Time delta_time)
{
	sf::Vector2f movement(0.f, 0.f);
	if(m_is_moving_up)
	{
		movement.y -= 1.f;
	}
	if (m_is_moving_down)
	{
		movement.y += 1.f;
	}
	if (m_is_moving_left)
	{
		movement.x -= 1.f;
	}
	if (m_is_moving_right)
	{
		movement.x += 1.f;
	}

	//TODO normalise movement vector
	sf::Vector2f normalised_movement = Normalise(movement);

	m_player.move(normalised_movement * delta_time.asSeconds() * kPlayerSpeed);
}

sf::Vector2f Game::Normalise(const sf::Vector2f input_vector)
{
	float length = Length(input_vector);
	if (length > 0)
	{
		return input_vector / length;
	}
	return input_vector;

}

float Game::Length(const sf::Vector2f& input_vector)
{
	return sqrt(pow(input_vector.x, 2) + pow(input_vector.y, 2));
}

void Game::Render()
{
	m_window.clear();
	m_window.draw(m_player);
	m_window.draw(m_statistics_text);
	m_window.display();
}

void Game::HandlePlayerInput(sf::Keyboard::Key key, bool is_pressed)
{
	if(key == sf::Keyboard::W)
	{
		m_is_moving_up = is_pressed;
	}
	else if (key == sf::Keyboard::S)
	{
		m_is_moving_down = is_pressed;
	}
	else if (key == sf::Keyboard::A)
	{
		m_is_moving_left = is_pressed;
	}
	else if (key == sf::Keyboard::D)
	{
		m_is_moving_right = is_pressed;
	}
}

void Game::UpdateStatistics(sf::Time elapsed_time)
{
	m_statistics_updatetime += elapsed_time;
	m_statistics_numframes += 1;

	if (m_statistics_updatetime >= sf::seconds(1.0f))
	{
		m_statistics_text.setString(
			"Frames / Second = " + std::to_string(m_statistics_numframes) + "\n" +
			"Time / Update = " + std::to_string(m_statistics_updatetime.asMicroseconds() / m_statistics_numframes) + "us");

		m_statistics_updatetime -= sf::seconds(1.0f);
		m_statistics_numframes = 0;
	}
}





