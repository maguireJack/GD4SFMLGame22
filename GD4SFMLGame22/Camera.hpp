#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/View.hpp>

class Camera : public sf::Transformable
{
public:
	Camera(sf::View view, sf::FloatRect bounds_constraint);
	const sf::View& GetView() const;

	void SetCenter(sf::Vector2f position);
	void SetPosition(sf::Vector2f position);
	void SetSize(sf::Vector2f size);
	void SetSize(float width, float height);

	sf::Vector2f GetCenter() const;
	sf::Vector2f GetSize() const;
	sf::FloatRect GetBoundingRect() const;

private:
	sf::View m_view;
	sf::FloatRect m_bounds_constraint;
};