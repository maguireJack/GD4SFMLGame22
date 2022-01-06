#pragma once
#include <array>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "Category.hpp"
#include "CommandQueue.hpp"
#include <set>

#include "Layers.hpp"

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef  std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	explicit SceneNode(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		Category::Type category = Category::kNone);

	void AttachChild(Ptr child);
	Ptr DetachChild(const SceneNode& node);

	void Update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f GetWorldPosition() const;
	sf::Transform GetWorldTransform() const;
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& GetSceneLayers() const;

	void OnCommand(const Command& command, sf::Time dt);
	virtual unsigned int GetCategory() const;
	virtual sf::FloatRect GetBoundingRect() const;
	virtual sf::Vector2f GetVelocity() const;

	virtual void HandleCollisions();
	void PredictCollisionsWithScene(SceneNode& scene_graph, std::set<SceneNode*>& collisions);
	void RemoveWrecks();


private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	void UpdateChildren(sf::Time dt, CommandQueue& commands);

	//Note draw is from sf::Drawable hence the name, lower case d
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void DrawCurrent(sf::RenderTarget&, sf::RenderStates states) const;
	void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

	void DrawBoundingRect(sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect& bounding_rect) const;

	virtual bool IsDestroyed() const;
	virtual bool IsMarkedForRemoval() const;


private:
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& m_scene_layers;
	std::vector<Ptr> m_children;
	SceneNode* m_parent;
	Category::Type m_default_category;
};
bool Collision(const SceneNode& lhs, const SceneNode& rhs);
float Distance(const SceneNode& lhs, const SceneNode& rhs);
