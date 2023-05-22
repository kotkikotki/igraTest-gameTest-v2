#ifndef PHYSICS_SYSTEM_HPP

#define PHYSICS_SYSTEM_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"


class PhysicsSystem : public System<PhysicsSystem>
{


public:

	float gravitationalConst = GRAVITY_CONST; //km/s2

	using System::System;

	void On_Update()
	{
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<PhysicsComponent>(i)&&m_scene->HasComponentById<TransformComponent>(i))) continue;

			PhysicsComponent& physics = m_scene->GetComponentById<PhysicsComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);
			
			if (!physics.m_gravity) continue;

			float massAffection = sqrtf(sqrtf(physics.m_mass));
			Vector2 v0 = physics.m_velocityVector;
			physics.m_velocityVector = { v0.x, v0.y + gravitationalConst * GetFrameTime() * massAffection};


		}

	}

};

#endif // !PHYSICS_SYSTEM_HPP

