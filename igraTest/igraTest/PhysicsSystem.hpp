#ifndef PHYSICS_SYSTEM_HPP

#define PHYSICS_SYSTEM_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"


class PhysicsSystem : public System<PhysicsSystem>
{


public:

	float gravitationalConst = G_CONST; //m/s2

	using System::System;

	void On_Update()
	{
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<PhysicsComponent>(i)&&m_scene->HasComponentById<TransformComponent>(i))) continue;

			PhysicsComponent& physics = m_scene->GetComponentById<PhysicsComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);
			
			if (!physics.m_gravity) continue;
			//
			//G = mg
			float massAffection = sqrtf(sqrtf(physics.m_mass));
			float v0 = physics.m_velocity;
			physics.m_velocity = v0 - gravitationalConst * GetFrameTime() * massAffection;

			//float distance = (v0 + physics.m_velocity) / 2.f * GetFrameTime();
			float distance = (v0 + physics.m_velocity) / 2.f * GetFrameTime()/100.f;

			transform.m_position.y -= distance;
			
			

		}

	}

};

#endif // !PHYSICS_SYSTEM_HPP

