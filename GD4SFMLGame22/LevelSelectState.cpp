#include "LevelSelectState.hpp"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ResourceHolder.hpp"
#include "Button.hpp"
#include <filesystem>

#include "Grid.hpp"
#include "Texture.hpp"
namespace fs = std::filesystem;

LevelSelectState::LevelSelectState(StateStack& stack, Context context)
	: State(stack, context)
	, m_gui_container(*context.window, *context.fonts, *context.camera, context.camera->GetBoundingRect())
{
	sf::Texture& texture = context.textures->Get(Textures::kTitleScreen);
	m_background_sprite.setTexture(texture);
	bool first_done = false;

	for (const auto& entry : fs::directory_iterator("Levels/"))
	{
		if (entry.path().has_extension() && entry.path().extension() == ".sav")
		{
			std::string path = entry.path().string().erase(entry.path().string().size() - 4);

			auto level_image = std::make_shared<GUI::Texture>(path + ".png");
			level_image->setScale(0.6f, 0.6f);
			level_image->setPosition(context.camera->GetBoundingRect().width / 2, (context.camera->GetBoundingRect().height - 200.f) / 2);

			sf::FloatRect bounds = level_image->GetBoundingRect();

			auto play_button = std::make_shared<GUI::Button>(context);
			play_button->SetText("Play");
			play_button->scale(1.92f, 1.92f); // 1/3 of 1152 (1152 is 60% of 1920)
			play_button->setPosition(bounds.left, bounds.top + bounds.height + 10);
			play_button->SetCallback([this, path]()
				{
					GetContext().grid->SetPathToLoad(path + ".sav");
					RequestStackPop();
					RequestStackPush(StateID::kGame);
				});

			auto edit_button = std::make_shared<GUI::Button>(context);
			edit_button->SetText("Edit");
			edit_button->scale(1.92f, 1.92f);
			edit_button->setPosition(bounds.left + play_button->GetBoundingRect().width, bounds.top + bounds.height + 10);

			auto delete_button = std::make_shared<GUI::Button>(context);
			delete_button->SetText("Delete");
			delete_button->scale(1.92f, 1.92f);
			delete_button->setPosition(edit_button->GetBoundingRect().left + edit_button->GetBoundingRect().width, bounds.top + bounds.height + 10);

			auto new_level_button = std::make_shared<GUI::Button>(context);
			new_level_button->SetText("Create New Level");
			new_level_button->scale(1.92f, 1.92f);
			new_level_button->setPosition(bounds.left + play_button->GetBoundingRect().width, bounds.top + bounds.height + 150);
			new_level_button->SetCallback([this]()
			{
				RequestStackPop();
				RequestStackPush(StateID::kLevelEditor);
			});

			m_gui_container.PackManual(level_image, first_done);
			m_gui_container.PackManual(play_button);
			m_gui_container.PackManual(edit_button);
			m_gui_container.PackManual(delete_button);
			m_gui_container.PackManual(new_level_button);

			first_done = true;
		}
	}

	//auto play_button = std::make_shared<GUI::Button>(context);
	//play_button->setPosition(100, 250);
	//play_button->SetText("Play");
	//play_button->SetCallback([this]()
	//	{
	//		RequestStackPop();
	//		RequestStackPush(StateID::kGame);
	//	});

	//auto settings_button = std::make_shared<GUI::Button>(context);
	//settings_button->setPosition(100, 300);
	//settings_button->SetText("Settings");
	//settings_button->SetCallback([this]()
	//	{
	//		RequestStackPush(StateID::kSettings);
	//	});

	//auto level_editor_button = std::make_shared<GUI::Button>(context);
	//level_editor_button->setPosition(100, 350);
	//level_editor_button->SetText("Level Editor");
	//level_editor_button->SetCallback([this]()
	//	{
	//		RequestStackPop();
	//		RequestStackPush(StateID::kLevelEditor);
	//	});

	//auto exit_button = std::make_shared<GUI::Button>(context);
	//exit_button->setPosition(100, 400);
	//exit_button->SetText("Exit");
	//exit_button->SetCallback([this]()
	//	{
	//		RequestStackPop();
	//	});

	//m_gui_container.Pack(play_button);
	//m_gui_container.Pack(settings_button);
	//m_gui_container.Pack(level_editor_button);
	//m_gui_container.Pack(exit_button);
}

void LevelSelectState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_gui_container);

}

bool LevelSelectState::Update(sf::Time dt)
{
	return true;
}

bool LevelSelectState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}

