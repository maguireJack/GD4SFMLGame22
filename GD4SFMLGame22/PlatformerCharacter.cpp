#include "PlatformerCharacter.hpp"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Collision.hpp"
#include "DataTables.hpp"
#include "PlatformerAnimationState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

enum class CollisionLocation;

namespace
{
	const std::vector<PlatformerCharacterData> Table = InitializePlatformerCharacterData();
}


PlatformerCharacter::PlatformerCharacter(
	const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers, 
	PlatformerCharacterType type,
	Camera& camera,
	const TextureHolder&
	textures,
	const FontHolder& fonts)
	: Entity(
		scene_layers,
		Table[static_cast<int>(type)].m_health,
		Table[static_cast<int>(type)].m_acceleration,
		Table[static_cast<int>(type)].m_max_velocity,
		Table[static_cast<int>(type)].m_deceleration,
		Table[static_cast<int>(type)].m_gravity)
	, m_type(type)
	, m_camera(camera)
	, m_artist(Table[static_cast<int>(type)].m_animation_data.ToVector(), textures)
	, m_health_display(nullptr)
{
	std::unique_ptr<TextNode> health_display(new TextNode(scene_layers, fonts, ""));
	m_health_display = health_display.get();
	AttachChild(std::move(health_display));
}

unsigned PlatformerCharacter::GetCategory() const
{
	return Category::kPlayerCharacter;
}

void PlatformerCharacter::HandleCollisions()
{
	std::set<SceneNode*> collisions;

	PredictCollisionsWithScene(*GetSceneLayers()[static_cast<int>(Layers::kPlatforms)], collisions);

	for (SceneNode* node : collisions)
	{
		if (Category::kPlatform & node->GetCategory())
		{
			const CollisionLocation location = Collision::CollisionLocation(*this, *node);
			const sf::Vector2f velocity = GetVelocity();

			switch (location)
			{
			case CollisionLocation::kLeft:
				std::cout << "Left" << std::endl;
				if (velocity.x < 0)
				{
					SetVelocity(0, velocity.y);
				}
				return;

			case CollisionLocation::kRight:
				std::cout << "Right" << std::endl;
				if (velocity.x > 0)
				{
					SetVelocity(0, velocity.y);
				}
				return;

			case CollisionLocation::kTop:
				std::cout << "Top" << std::endl;
				if (velocity.y < 0)
				{
					SetVelocity(velocity.x, 0);
				}
				return;

			case CollisionLocation::kBottom:
				std::cout << "Bottom" << std::endl;
				if (velocity.y > 0)
				{
					SetVelocity(velocity.x, 0);
					SetGrounded(true);
				}
				return;

			case CollisionLocation::kNone:
				return;
			}
		}
		
	}
}

sf::FloatRect PlatformerCharacter::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_artist.GetBoundingRect());
}

void PlatformerCharacter::Jump()
{
	AddVelocity(0, -Table[static_cast<int>(m_type)].m_jump_height);
}

bool PlatformerCharacter::IsGrounded()
{
	return m_grounded;
}

void PlatformerCharacter::SetGrounded(bool grounded)
{
	m_grounded = grounded;
}

void PlatformerCharacter::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_artist.DrawCurrent(target, states);
}

void PlatformerCharacter::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::UpdateCurrent(dt, commands);
	m_artist.UpdateCurrent(dt);

	UpdateAnimationState();
	UpdateCamera(dt);
	UpdateTexts();
}

void PlatformerCharacter::UpdateAnimationState()
{
	const sf::Vector2f velocity = GetVelocity();

	if (Utility::Length(velocity) == 0.f)
	{
		m_artist.ChangeState(static_cast<int>(PlatformerAnimationState::kIdle));
	}
	else
	{
		m_artist.ChangeState(static_cast<int>(PlatformerAnimationState::kRun));

		if (velocity.x > 0)
		{
			m_artist.Flipped(false);
		}
		else if(velocity.x < 0)
		{
			m_artist.Flipped(true);
		}
		
	}
}

void PlatformerCharacter::UpdateCamera(sf::Time dt) const
{
	const sf::FloatRect bounds = m_camera.GetBoundingRect();
	const float right = bounds.left + bounds.width;
	const float bottom = bounds.top + bounds.height;

	sf::Vector2f new_position;

	if (getPosition().x > right - 100)
	{
		new_position.x += 100;
	}
	else if (getPosition().x < bounds.left + 100)
	{
		new_position.x -= 100;
	}

	if (getPosition().y > bottom - 100)
	{
		new_position.y += 100;
	}
	else if (getPosition().y < bounds.top + 100)
	{
		new_position.y -= 100;
	}

	m_camera.SetPosition(m_camera.getPosition() + new_position * 0.01f);
}

void PlatformerCharacter::UpdateTexts() const
{
	m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	m_health_display->setPosition(0.f, 50.f);
	m_health_display->setRotation(-getRotation());
}