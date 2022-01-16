#include "EmitterNode.hpp"
#include "ParticleNode.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"

/// <summary>
/// Emitter Node
/// </summary>
/// <param name="scene_layers">Scene Layers</param>
/// <param name="type">Particle Type to emit</param>
EmitterNode::EmitterNode(const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers,
	ParticleType type)
	: SceneNode(scene_layers)
	, m_accumulated_time(sf::Time::Zero)
	, m_type(type)
	, m_particle_system(nullptr)
{
}

/// <summary>
/// Update
/// </summary>
/// <param name="dt"></param>
/// <param name="commands"></param>
void EmitterNode::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (m_particle_system)
	{
		EmitParticles(dt);
	}
	else
	{
		// Find particle node with the same type as emitter node
		auto finder = [this](ParticleNode& container, sf::Time)
		{
			if (container.GetParticleType() == m_type)
				m_particle_system = &container;
		};

		Command command;
		command.category = Category::kParticleSystem;
		command.action = DerivedAction<ParticleNode>(finder);

		commands.Push(command);
	}
}

/// <summary>
/// Emits the particles over time relative to world position
/// </summary>
/// <param name="dt"></param>
void EmitterNode::EmitParticles(sf::Time dt)
{
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	m_accumulated_time += dt;

	while (m_accumulated_time > interval)
	{
		m_accumulated_time -= interval;
		m_particle_system->AddParticle(GetWorldPosition());
	}
}

