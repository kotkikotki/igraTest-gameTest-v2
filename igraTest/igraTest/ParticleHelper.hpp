#ifndef PARTICLE_HELPER_HPP

#define PARTICLE_HELPER_HPP


#include "Definitions.hpp"
//#include "System.hpp"

#include "Entity.hpp"
#include "Components.h"
#include "Scene.hpp"
#include"BasicAnimationScripts.hpp"

class ParticleScript : public BehaviourScript
{

	

public:

	using BehaviourScript::BehaviourScript;

	void On_Update(Entity& owner) override
	{
		if (!owner.HasComponent<AnimationComponent>()) { owner.Destroy(); return; }
		auto& sequence = *owner.GetComponent<AnimationComponent>().GetScript();
		if (sequence.GetSequences().empty())
		{
			owner.Destroy();
		}
	}
	
	ParticleScript(Entity& owner, float duration) : BehaviourScript()
	{
		if (!owner.HasComponent<AnimationComponent>()) { owner.Destroy(); return; }
		
		auto& sequence = *owner.GetComponent<AnimationComponent>().GetScript();
		sequence.AddSequence(owner.GetComponent<SpriteComponent>().GetSprite("base"), duration);
	}

	//
	//colision
	


};

void AddParticle(Scene& scene, const TransformComponent& transform, const SpriteComponent& sprite, float duration)
{
	Entity& particle = scene.AddEntity({ "projectile" });


	particle.AddComponent<TransformComponent>
		(transform);
	particle.AddComponent<SpriteComponent>
		(sprite);
	particle.AddComponent<AnimationComponent>
		(std::make_shared<SequenceOnlyAnimationScript>());

	particle.AddComponent<BehaviourComponent>
		(std::make_shared<ParticleScript>(particle, duration));
	particle.AddComponent<PhysicsComponent>(5.f, Vector2{ 0.f, 0.f }, false);
}

#endif // !PARTICLE_HELPER_HPP

