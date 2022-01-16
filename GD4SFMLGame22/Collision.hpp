#pragma once
#include <SFML/Graphics/Rect.hpp>

#include "CollisionLocation.hpp"
#include "Entity.hpp"

//D00218436 Vilandas Morrissey
//D00219343 Jack Maguire

class Collision
{
public:
	static bool Intersects(SceneNode& a, SceneNode& b);
	static CollisionLocation CollisionLocation(SceneNode& a, SceneNode& b);
	static sf::FloatRect PredictMove(const sf::FloatRect& bounds, const sf::Vector2f& velocity);
	static sf::FloatRect PredictMove(const SceneNode& node);
};
