#pragma once
#include "ParticleNode.hpp"
#include "ParticleType.hpp"
#include "SceneNode.hpp"

class EmitterNode : public SceneNode
{
public:
	explicit EmitterNode(const std::array<SceneNode*, static_cast<int>(Layers::kLayerCount)>& scene_layers, ParticleType type);


private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);

	void EmitParticles(sf::Time dt);


private:
	sf::Time m_accumulated_time;
	ParticleType m_type;
	ParticleNode* m_particle_system;
};
