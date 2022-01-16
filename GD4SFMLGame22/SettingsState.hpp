#pragma once
#include "State.hpp"

#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);

	void Draw() override;
	bool Update(sf::Time dt) override;
	bool HandleEvent(const sf::Event& event) override;


private:
	void UpdateLabels();
	void AddButtonLabel(PlayerAction action, float y, const std::string& text, Context context);


private:
	sf::Sprite m_background_sprite;
	GUI::Container m_gui_container;
	std::array<GUI::Button::Ptr, static_cast<int>(PlayerAction::kActionCount)> m_binding_buttons;
	std::array<GUI::Label::Ptr, static_cast<int>(PlayerAction::kActionCount)> 	m_binding_labels;
};


