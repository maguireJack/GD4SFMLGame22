#include "LevelEditorState.hpp"

#include "Button.hpp"
#include "TexturedButton.hpp"
#include "Player.hpp"

LevelEditorState::LevelEditorState(StateStack& stack, Context context)
	: State(stack, context)
	, m_world(*context.window, *context.textures, *context.fonts, *context.camera, *context.grid)
	, m_player(*context.player)
	, m_grid(*context.grid)
	, m_gui_container(*context.window, *context.fonts, *context.camera, sf::FloatRect(0, 276, 576, 48), 0.5f)
{
	m_background.setFillColor(sf::Color(0, 0, 0, 150));
	m_background.setSize(sf::Vector2f(576, 48));
	m_background_position = sf::Vector2f(0, 276);

	for (int texture_index = static_cast<int>(Textures::kGrassTiles0); texture_index <= static_cast<int>(Textures::kGrassTiles24); texture_index++)
	{
		auto texture = static_cast<Textures>(texture_index);
		auto button = std::make_shared<GUI::TexturedButton>(*context.fonts, *context.textures, texture);
		button->setPosition(16, 292);
		button->SetCallback([this, texture_index, button]()
			{
				m_grid.Node().SetNewTileSettings(PlatformType::kStatic, static_cast<Textures>(texture_index));
				m_gui_container.DeactivateAllExcept(button);
			});

		m_gui_container.Pack(button, 16);
	}
}

void LevelEditorState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	m_world.Draw();

	window.draw(m_background);
	window.draw(m_gui_container);
}

bool LevelEditorState::Update(sf::Time dt)
{
	m_world.Update(dt);
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleRealtimeInput(commands);

	const sf::Vector2f camera_position = m_grid.Node().GetCamera().getPosition();
	m_background.setPosition(camera_position + m_background_position);
	m_gui_container.setPosition(camera_position);

	return true;
}

bool LevelEditorState::HandleEvent(const sf::Event& event)
{
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleEvent(event, commands);
	m_grid.HandleEvent(event, commands);
	m_gui_container.HandleEvent(event);

	//Escape should bring up the Pause Menu
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		RequestStackPush(StateID::kPause);
	}
	return true;
}
