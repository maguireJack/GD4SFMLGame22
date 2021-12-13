#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "Layers.hpp"
#include <SFML/Graphics/View.hpp>
#include <array>
#include <SFML/Graphics/Sprite.hpp>

#include "CommandQueue.hpp"
#include "GameState.hpp"
#include "World.hpp"

class LevelEditorState : public GameState
{
public:
	LevelEditorState(StateStack& stack, const Context& context);
	virtual void Draw();
	virtual bool Update(sf::Time dt);
	virtual bool HandleEvent(const sf::Event& event);

private:
	World m_world;
	Player& m_player;
};
