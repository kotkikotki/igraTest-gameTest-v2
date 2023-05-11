#ifndef COLLISION_SYSTEM_HPP

#define COLLISION_SYSTEM_HPP


#include <vector>
#include "Definitions.hpp"
#include "System.hpp"

class CollisionSystem : public System<CollisionSystem>
{

	std::vector<std::vector<bool>> collisionMatrix;

public:

	using System::System;

	void On_Update()
	{
		//updating collision positions
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<CollisionComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) return;

			CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);

			collision.UpdatePosition(transform.m_position);

		}
	}

};


#endif // !ANIMATION_SYSTEM_HPP
