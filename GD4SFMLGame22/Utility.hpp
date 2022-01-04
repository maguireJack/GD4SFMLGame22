#pragma once
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


namespace sf
{
	class Texture;
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
	static sf::IntRect GetIntRect(sf::Texture& texture);
	static int RandomInt(int exclusive_max);
	static sf::Vector2i GetCellPosition(sf::Vector2i position, const sf::RenderWindow& window, const sf::View& camera_view);
};

