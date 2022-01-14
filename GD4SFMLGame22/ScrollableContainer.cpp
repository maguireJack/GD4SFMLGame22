#include "ScrollableContainer.hpp"

#include "ResourceHolder.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include "Fonts.hpp"

namespace GUI
{
	ScrollableContainer::ScrollableContainer(sf::RenderWindow& window, const FontHolder& fonts, Camera& camera, sf::FloatRect bounds, float text_scale)
		: Container(window, camera)
		, m_bounds(bounds)
		, m_vertical_scroll(true)
		, m_current_page(0)
		, m_page_text("Page 1/1", fonts.Get(Fonts::Main), 16)
	{
		m_page_start_indexes.emplace_back(-1);
		m_page_text.scale(text_scale, text_scale);

		constexpr float offset = 6;

		sf::FloatRect text_bounds = m_page_text.getGlobalBounds();
		sf::FloatRect container_bounds = GetBoundingRect();
		float container_right = container_bounds.left + container_bounds.width;
		float container_bottom = container_bounds.top + container_bounds.height;

		m_page_text.setPosition(container_right - text_bounds.width - offset, container_bottom - text_bounds.height - offset);
	}

	bool ScrollableContainer::Pack(Component::Ptr component)
	{
		return Pack(component, 0);
	}

	/// <summary>
	/// </summary>
	/// <param name="component"></param>
	/// <param name="spacing"></param>
	/// <returns>Added component to new page</returns>
	bool ScrollableContainer::Pack(Component::Ptr component, float spacing)
	{
		if (m_page_start_indexes[0] == -1) //first page is empty
		{
			Container::Pack(component);

			m_page_start_indexes[0]++;
			return false;
		}

		const sf::FloatRect last_bounds = m_children.back()->GetBoundingRect();
		const sf::FloatRect c_bounds = component->GetBoundingRect();
		const sf::FloatRect bounds = GetBoundingRect();
		bool exceeds_page_bounds = false;

		if (m_vertical_scroll)
		{
			const float last_bounds_right = last_bounds.left + last_bounds.width;
			const float c_bounds_left = last_bounds_right + spacing;
			const float c_bounds_right = c_bounds_left + c_bounds.width;
			const float bounds_right = bounds.left + bounds.width;

			if (c_bounds_right >= bounds_right)
			{
				exceeds_page_bounds = true;
				component->setPosition(c_bounds.left, c_bounds.top);
			}
			else
			{
				component->setPosition(c_bounds_left, c_bounds.top);
			}
		}
		else
		{
			const float last_bounds_bottom = last_bounds.top + last_bounds.height;
			const float c_bounds_top = last_bounds_bottom + spacing;
			const float c_bounds_bottom = c_bounds_top + c_bounds.height;
			const float bounds_bottom = bounds.top + bounds.height;

			if (c_bounds_bottom > bounds_bottom)
			{
				exceeds_page_bounds = true;
				component->setPosition(c_bounds.left, c_bounds.top);
			}
			else
			{
				component->setPosition(c_bounds.left, c_bounds_top);
			}
		}

		Container::Pack(component);

		if (exceeds_page_bounds)
		{
			m_page_start_indexes.emplace_back(m_children.size() - 1); //end index of new page
			UpdateCurrentPageText();
			return true;
		}

		return false;
	}

	void ScrollableContainer::NextPage()
	{
		if (!IsLastPage())
		{
			m_current_page++;
			UpdateCurrentPageText();
		}
	}

	void ScrollableContainer::PreviousPage()
	{
		if (m_current_page > 0)
		{
			m_current_page--;
			UpdateCurrentPageText();
		}
	}

	bool ScrollableContainer::IsLastPage() const
	{
		return m_current_page >= m_page_start_indexes.size() - 1;
	}

	sf::FloatRect ScrollableContainer::GetBoundingRect() const
	{
		return GetWorldTransform().transformRect(m_bounds);
	}

	void ScrollableContainer::HandleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			const int end_index = EndIndexOfCurrentPage();
			for (size_t i = m_page_start_indexes[m_current_page]; i <= end_index; i++)
			{
				const sf::Vector2f mouse = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_camera.GetView());
				const auto child = m_children[i];
				if (child->IsSelectable() && child->GetBoundingRect().contains(mouse.x, mouse.y))
				{
					Select(i);
				}
			}
		}
		else
		{
			Container::HandleEvent(event);
		}

		if (event.type == sf::Event::MouseWheelScrolled)
		{
			float delta = event.mouseWheelScroll.delta;

			if (delta > 0)
			{
				NextPage();
			}
			else
			{
				PreviousPage();
			}
		}
	}

	void ScrollableContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		const int end_index = EndIndexOfCurrentPage();
		for (int i = m_page_start_indexes[m_current_page]; i <= end_index; i++)
		{
			target.draw(*m_children[i], states);
		}

		target.draw(m_page_text, states);

		sf::FloatRect rect = GetBoundingRect();
		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		target.draw(shape);
	}

	int ScrollableContainer::EndIndexOfCurrentPage() const
	{
		return IsLastPage()
			? m_children.size() - 1
			: m_page_start_indexes[m_current_page + 1] - 1;
	}

	void ScrollableContainer::UpdateCurrentPageText()
	{
		m_page_text.setString("Page " + std::to_string(m_current_page + 1) + "/" + std::to_string(m_page_start_indexes.size()));
	}
}
