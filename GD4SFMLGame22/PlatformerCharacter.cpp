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
	const TextureHolder& textures,
	const FontHolder& fonts,
	SoundPlayer& sounds)
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
	, m_sounds(sounds)
	, m_jumping(false)
	, m_camera_move_constraint(false)
	, m_coyote_time(Table[static_cast<int>(type)].m_coyote_time)
	, m_air_time(0)
	, m_collected_coins(0)
{
}

unsigned PlatformerCharacter::GetCategory() const
{
	return Category::kPlayerCharacter;
}

sf::FloatRect PlatformerCharacter::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_artist.GetSmallestBounds());
}

int PlatformerCharacter::GetCollectedCoins() const
{
	return m_collected_coins;
}

/// <summary>
/// Vilandas
/// </summary>
void PlatformerCharacter::Jump()
{
	if (!IsJumping() && m_air_time <= m_coyote_time)
	{
		m_jumping = true;
		const sf::Vector2f velocity = GetVelocity();

		if (velocity.y < 0) // if moving upwards, allow momentum jump
		{
			SetVelocity(GetVelocity().x, velocity.y - Table[static_cast<int>(m_type)].m_jump_force);
		}
		else
		{
			SetVelocity(GetVelocity().x, -Table[static_cast<int>(m_type)].m_jump_force);
		}
	}
}

void PlatformerCharacter::ResetJump()
{
	m_air_time = 0.f;
	m_jumping = false;
}

bool PlatformerCharacter::IsGrounded() const
{
	return m_air_time > 0;
}

bool PlatformerCharacter::IsJumping() const
{
	return m_jumping;
}
/// <summary>
/// Vilandas/Jack - Handle Collisions between the Player Character and any SceneNode with a collider
/// </summary>
void PlatformerCharacter::HandleCollisions()
{
	std::set<SceneNode*> collisions;

	PredictCollisionsWithScene(*GetSceneLayers()[static_cast<int>(Layers::kPlatforms)], collisions);

	for (SceneNode* node : collisions)
	{
		if (Category::kPlatform & node->GetCategory())
		{
			const CollisionLocation location = Collision::CollisionLocation(*this, *node);
			const auto tile = dynamic_cast<TileNode*>(node);
			std::unordered_set<PlatformEffects> effects = tile->Data().GetEffects();

			if (effects.empty())
			{
				BlockingCollision(location);
			}
			else
			{
				if (effects.count(Bouncy))
				{
					BouncyCollision(location);
				}
				if (effects.count(VerticalMovement))
				{
					VerticalMovementCollision(location, tile);
				}
			}
		}
		else if (Category::kPickup & node->GetCategory()) 
		{
			m_sounds.Play(SoundEffect::kCollectPickup);
			auto coin = dynamic_cast<TileNode*>(node);
			coin->Destroy();
			m_collected_coins++;
		}

	}
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="location"></param>
void PlatformerCharacter::BlockingCollision(CollisionLocation location)
{
	const sf::Vector2f velocity = GetVelocity();

	switch (location)
	{
	case CollisionLocation::kLeft:
		if (velocity.x < 0)
		{
			SetVelocity(0, velocity.y);
		}
		break;

	case CollisionLocation::kRight:
		if (velocity.x > 0)
		{
			SetVelocity(0, velocity.y);
		}
		break;

	case CollisionLocation::kTop:
		if (velocity.y < 0)
		{
			SetVelocity(velocity.x, 0);
		}
		break;

	case CollisionLocation::kBottom:
		ResetJump();

		if (velocity.y > 0)
		{
			SetVelocity(velocity.x, 0);
		}
		break;

	case CollisionLocation::kNone:
		break;
	}
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="location"></param>
void PlatformerCharacter::BouncyCollision(CollisionLocation location)
{
	const sf::Vector2f velocity = GetVelocity();

	if (static_cast<int>(location) & static_cast<int>(CollisionLocation::kXAxis))
	{
		SetVelocity(velocity.x * -2.f, velocity.y);
	}
	else
	{
		SetVelocity(velocity.x, velocity.y * -2.f);
	}
}

/// <summary>
/// Vilandas
/// </summary>
/// <param name="location"></param>
/// <param name="tile"></param>
void PlatformerCharacter::VerticalMovementCollision(CollisionLocation location, TileNode* tile)
{
	if (location == CollisionLocation::kBottom)
	{
		tile->ActivateVerticalMovement();
		SetVelocity(GetVelocity().x, tile->GetVelocity().y);
		ResetJump();
	}
	else
	{
		//BlockingCollision(location);
	}
}

void PlatformerCharacter::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_artist.DrawCurrent(target, states);
}

void PlatformerCharacter::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	m_air_time += dt.asSeconds();

	Entity::UpdateCurrent(dt, commands);
	m_artist.UpdateCurrent(dt);

	UpdateAnimationState();
	UpdateCamera(dt);

	std::cout << GetVelocity().x << std::endl;
}

void PlatformerCharacter::UpdateAnimationState()
{
	const sf::Vector2f velocity = GetVelocity();

	if (velocity.x != 0.f)
	{
		m_artist.ChangeState(static_cast<int>(PlatformerAnimationState::kRun));

		if (velocity.x > 0)
		{
			m_artist.Flipped(false);
		}
		else if (velocity.x < 0)
		{
			m_artist.Flipped(true);
		}
	}
	else
	{
		m_artist.ChangeState(static_cast<int>(PlatformerAnimationState::kIdle));
	}
}


/// <summary>
/// Vilandas
/// </summary>
/// <param name="dt"></param>
void PlatformerCharacter::UpdateCamera(sf::Time dt)
{
	const sf::Vector2f distance = getPosition() - m_camera.GetCenter();

	if (m_camera_move_constraint || Utility::Length(distance) > 100)
	{
		m_camera_move_constraint = true;

		const sf::Vector2f new_position = distance * 0.01f;
		m_camera.SetCenter(m_camera.GetCenter() + new_position);

		if (Utility::Length(new_position) <= 0.2f)
		{
			m_camera_move_constraint = false;
		}
	}
}

//void PlatformerCharacter::PlayLocalSound(CommandQueue& commands, SoundEffect effect)
//{
//	sf::Vector2f world_position = GetWorldPosition();
//
//	Command command;
//	command.category = Category::kSoundEffect;
//	command.action = DerivedAction<SoundNode>(
//		[effect, world_position](SoundNode& node, sf::Time)
//	{
//		node.PlaySound(effect, world_position);
//	});
//
//	commands.Push(command);
//}