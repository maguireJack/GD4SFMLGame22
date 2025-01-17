#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>


namespace sf
{
	class Sprite;
	class Text;
}

class Utility
{
public:
	static void CentreOrigin(sf::Sprite& sprite);
	static void CentreOrigin(sf::Text& text);
	static std::string toString(sf::Keyboard::Key key);
	static double ToRadians(int degrees);
	static sf::Vector2f UnitVector(sf::Vector2f vector);
	static float Length(sf::Vector2f vector);
	static float ToDegrees(float angle);
};

