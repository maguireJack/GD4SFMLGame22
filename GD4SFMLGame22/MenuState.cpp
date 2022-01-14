#include "MenuState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "Button.hpp"



MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, m_gui_container(*context.window, *context.camera)
{
	*context.camera = Camera(context.window->getDefaultView());

	sf::Texture& texture = context.textures->Get(Textures::kTitleScreen);
	m_background_sprite.setTexture(texture);

	auto play_button = std::make_shared<GUI::Button>(context);
	play_button->setPosition(100, 250);
	play_button->SetText("Play");
	play_button->SetCallback([this]()
	{
		RequestStackPop();
		RequestStackPush(StateID::kGame);
	});

	auto settings_button = std::make_shared<GUI::Button>(context);
	settings_button->setPosition(100, 300);
	settings_button->SetText("Settings");
	settings_button->SetCallback([this]()
	{
		RequestStackPush(StateID::kSettings);
	});

	auto level_editor_button = std::make_shared<GUI::Button>(context);
	level_editor_button->setPosition(100, 350);
	level_editor_button->SetText("Level Editor");
	level_editor_button->SetCallback([this]()
	{
		RequestStackPop();
		RequestStackPush(StateID::kLevelEditor);
	});

	auto exit_button = std::make_shared<GUI::Button>(context);
	exit_button->setPosition(100, 400);
	exit_button->SetText("Exit");
	exit_button->SetCallback([this]()
	{
		RequestStackPop();
	});

	m_gui_container.Pack(play_button);
	m_gui_container.Pack(settings_button);
	m_gui_container.Pack(level_editor_button);
	m_gui_container.Pack(exit_button);
}

void MenuState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	auto x = window.getDefaultView();
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);
	
}

bool MenuState::Update(sf::Time dt)
{
	return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}

