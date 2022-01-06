#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "AnimatedSpriteArtist.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "PlatformerCharacterType.hpp"
#include "ResourceIdentifiers.hpp"
#include "TextNode.hpp"

class PlatformerCharacter : public Entity
{
public:
	PlatformerCharacter(
		const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
		PlatformerCharacterType type,
		Camera& camera,
		const TextureHolder& textures,
		const FontHolder& fonts);

	unsigned GetCategory() const override;

	void HandleCollisions() override;
	sf::FloatRect GetBoundingRect() const override;

	void Jump();
	bool IsGrounded();
	void SetGrounded(bool grounded);

private:
	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

	void UpdateAnimationState();
	void UpdateCamera(sf::Time dt) const;
	void UpdateTexts() const;

private:
	PlatformerCharacterType m_type;
	Camera& m_camera;
	AnimatedSpriteArtist m_artist;
	TextNode* m_health_display;
	bool m_grounded = true;
};
