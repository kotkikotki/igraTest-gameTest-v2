#ifndef ANIMATION_SYSTEM_HPP

#define ANIMATION_SYSTEM_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"

class AnimationSystem
{

	Scene* m_scene;

public:

	AnimationSystem(Scene& scene)
	{
		m_scene = &scene;
	}
	void On_Update_Animate()
	{
		for (int i = 0; i < m_scene->GetIdCount(); i++)
		{
			if (!(m_scene->HasComponentById<AnimationComponent>(i) && m_scene->HasComponentById<SpriteComponent>(i))) return;

			AnimationComponent& animation = m_scene->GetComponentById<AnimationComponent>(i);
			SpriteComponent& sprite = m_scene->GetComponentById<SpriteComponent>(i);

			//
			animation.GetScript()->UpdateProperties();
			animation.GetScript()->Animate(sprite);
			//

		}
		
	}
};


#endif // !ANIMATION_SYSTEM_HPP
