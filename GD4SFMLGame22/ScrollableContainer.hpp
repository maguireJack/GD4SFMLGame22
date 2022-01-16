#pragma once
#include <SFML/Graphics/Text.hpp>

#include "Container.hpp"
#include "ResourceIdentifiers.hpp"

//D00218436 Vilandas Morrissey
//D00219343 Jack Maguire
namespace GUI
{
	class ScrollableContainer : public Container
	{
	public:
		typedef std::shared_ptr<ScrollableContainer> Ptr;

	public:
		ScrollableContainer(sf::RenderWindow& window, const FontHolder& fonts, Camera& camera, sf::FloatRect bounds, float text_scale = 1.f);
		bool Pack(Component::Ptr component) override;
		bool Pack(Component::Ptr component, float spacing);
		void PackManual(Component::Ptr component, bool on_next_page = false);
		void NextPage();
		void PreviousPage();
		bool IsLastPage() const;

		sf::FloatRect GetBoundingRect() const override;

		void HandleEvent(const sf::Event& event) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		int EndIndexOfCurrentPage() const;
		void UpdateCurrentPageText();

	private:
		sf::FloatRect m_bounds;
		bool m_vertical_scroll;
		std::vector<int> m_page_start_indexes;
		int m_current_page;
		sf::Text m_page_text;
	};
}
