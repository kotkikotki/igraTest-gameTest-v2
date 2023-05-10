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
		//
		
		//
		for (int i = 0; i < m_scene->GetIdCount(); i++)
		{
			if (!(m_scene->HasComponentById<CollisionComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) return;

			CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);

			//
			animation.GetScript()->UpdateProperties();
			animation.GetScript()->Animate(sprite);
			//

		}
	}

};


#endif // !ANIMATION_SYSTEM_HPP
