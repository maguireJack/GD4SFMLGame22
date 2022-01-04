#include "Camera.hpp"

Camera::Camera(sf::View& view)
	: m_view(view)
{
	SetCenter(view.getCenter());
}

sf::View& Camera::GetView() const
{
	return m_view;
}

void Camera::SetCenter(sf::Vector2f position)
{
	m_view.setCenter(position);
	setPosition(position - m_view.getSize() / 2.f);
}

void Camera::SetPosition(sf::Vector2f position)
{
	setPosition(position);
	m_view.setCenter(position + (m_view.getSize() / 2.f));
}

void Camera::SetSize(sf::Vector2f size)
{
	m_view.setSize(size);
	SetCenter(m_view.getCenter());
}

void Camera::SetSize(float width, float height)
{
	m_view.setSize(width, height);
	SetCenter(m_view.getCenter());
}

sf::Vector2f Camera::GetCenter() const
{
	return m_view.getCenter();
}

sf::Vector2f Camera::GetSize() const
{
	return m_view.getSize();
}

sf::FloatRect Camera::GetBoundingRect() const
{
	return { getPosition(), m_view.getSize() };
}