#include "Player.hpp"
#include <algorithm>

#include "Entity.hpp"

struct EntityDirectionAdder
{
	EntityDirectionAdder(int vx, int vy) : velocity(vx, vy)
	{
	}

	void operator()(Entity& entity, sf::Time) const
	{
		entity.AddDirection(velocity);
	}

	sf::Vector2i velocity;
};

struct EntityDirectionRemover
{
	EntityDirectionRemover(int vx, int vy) : velocity(vx, vy)
	{
	}

	void operator()(Entity& entity, sf::Time) const
	{
		entity.RemoveDirection(velocity);
	}

	sf::Vector2i velocity;
};

Player::Player() : m_current_mission_status(MissionStatus::kMissionRunning)
{
	//Set initial key bindings
	m_key_binding[sf::Keyboard::A] = PlayerAction::kMoveLeft;
	m_key_binding[sf::Keyboard::D] = PlayerAction::kMoveRight;
	m_key_binding[sf::Keyboard::W] = PlayerAction::kMoveUp;
	m_key_binding[sf::Keyboard::S] = PlayerAction::kMoveDown;

	//Set initial action bindings
	InitialiseActions();

	//Assign all categories to the player's character
	for(auto& pair : m_action_binding)
	{
		pair.second.category = Category::kPlayerCharacter;
	}

	for(auto& pair : m_on_release_action_binding)
	{
		pair.second.category = Category::kPlayerCharacter;
	}
}


void Player::HandleEvent(const sf::Event& event, CommandQueue& commands)
{
	if(event.type == sf::Event::KeyPressed)
	{
		auto found = m_key_binding.find(event.key.code);
		if(found != m_key_binding.end() && !IsRealtimeAction(found->second))
		{
			m_active_actions.emplace(found->second);
			commands.Push(m_action_binding[found->second]);
		}
	}
}

void Player::HandleRealtimeInput(CommandQueue& commands)
{
	//Check if any keybinding keys are pressed
	for(auto pair: m_key_binding)
	{
		if(IsRealtimeAction(pair.second))
		{
			if (sf::Keyboard::isKeyPressed(pair.first))
			{
				commands.Push(m_action_binding[pair.second]);
				m_active_actions.emplace(pair.second);
			}
			else if(m_active_actions.count(pair.second))
			{
				m_active_actions.erase(pair.second);

				if (m_on_release_action_binding.count(pair.second))
				{
					commands.Push(m_on_release_action_binding[pair.second]);
				}
			}
		}
	}
}

void Player::AssignKey(PlayerAction action, sf::Keyboard::Key key)
{
	//Remove all keys that are already bound to action
	for(auto itr = m_key_binding.begin(); itr != m_key_binding.end();)
	{
		if(itr->second == action)
		{
			m_key_binding.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
	m_key_binding[key] = action;
}

sf::Keyboard::Key Player::GetAssignedKey(PlayerAction action) const
{
	for(auto pair : m_key_binding)
	{
		if(pair.second == action)
		{
			return pair.first;
		}
	}
	return sf::Keyboard::Unknown;
}

void Player::SetMissionStatus(MissionStatus status)
{
	m_current_mission_status = status;
}

MissionStatus Player::GetMissionStatus() const
{
	return m_current_mission_status;
}

void Player::InitialiseActions()
{
	m_action_binding[PlayerAction::kMoveLeft].action = DerivedAction<Entity>(EntityDirectionAdder(-1, 0));
	m_action_binding[PlayerAction::kMoveRight].action = DerivedAction<Entity>(EntityDirectionAdder(1, 0));
	m_action_binding[PlayerAction::kMoveUp].action = DerivedAction<Entity>(EntityDirectionAdder(0, -1));
	m_action_binding[PlayerAction::kMoveDown].action = DerivedAction<Entity>(EntityDirectionAdder(0, 1));

	m_on_release_action_binding[PlayerAction::kMoveLeft].action = DerivedAction<Entity>(EntityDirectionRemover(-1, 0));
	m_on_release_action_binding[PlayerAction::kMoveRight].action = DerivedAction<Entity>(EntityDirectionRemover(1, 0));
	m_on_release_action_binding[PlayerAction::kMoveUp].action = DerivedAction<Entity>(EntityDirectionRemover(0.f, -1));
	m_on_release_action_binding[PlayerAction::kMoveDown].action = DerivedAction<Entity>(EntityDirectionRemover(0, 1));
}

bool Player::IsRealtimeAction(PlayerAction action)
{
	switch(action)
	{
	case PlayerAction::kMoveLeft:
	case PlayerAction::kMoveRight:
	case PlayerAction::kMoveUp:
	case PlayerAction::kMoveDown:
		return true;
	default:
		return false;
	}
}
