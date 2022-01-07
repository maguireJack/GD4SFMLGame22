#pragma once
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Camera.hpp"
#include "Component.hpp"

namespace GUI
{
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container(sf::RenderWindow& window, Camera& camera);
		void DeactivateAllExcept(const Component::Ptr& component);
		void Pack(Component::Ptr component);
		virtual bool IsSelectable() const override;
		virtual void HandleEvent(const sf::Event& event) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		bool HasSelection() const;
		void Select(std::size_t index);
		void SelectNext();
		void SelectPrevious();

	private:
		sf::RenderWindow& m_window;
		Camera& m_camera;
		std::vector<Component::Ptr> m_children;
		int m_selected_child;
	};
}

