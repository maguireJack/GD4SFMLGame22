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
	sf::FloatRect GetBoundingRect() const override;

	void Jump();
	bool IsGrounded() const;
	bool IsJumping() const;

protected:
	void HandleCollisions() override;

private:
	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateCurrent(sf::Time dt, CommandQueue& commands) override;

	void UpdateAnimationState();
	void UpdateCamera(sf::Time dt);
	void UpdateTexts() const;

private:
	PlatformerCharacterType m_type;
	Camera& m_camera;
	AnimatedSpriteArtist m_artist;
	TextNode* m_health_display;
	bool m_jumping;
	bool m_camera_move_constraint;
	float m_coyote_time;
	float m_air_time;
};
