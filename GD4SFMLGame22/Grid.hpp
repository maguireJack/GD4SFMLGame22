#pragma once
#include <SFML/Window/Event.hpp>

#include "GridNode.hpp"

//D00218436 Vilandas Morrissey
//D00219343 Jack Maguire

class Grid
{
public:
	Grid();
	GridNode& Node() const;
	void SetNode(GridNode* grid);
	void SetPathToLoad(const std::string& path);
	void SetEditMode(bool editor_mode);
	void CreatePath();
	void HandleEvent(const sf::Event& event, CommandQueue& commands) const;

private:
	GridNode* m_grid_node;
	std::string m_path_to_load;
	bool m_editor_mode;
};
