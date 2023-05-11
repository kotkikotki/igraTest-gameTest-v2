#ifndef ANIMATION_SYSTEM_HPP

#define ANIMATION_SYSTEM_HPP


#include <vector>
#include "Definitions.hpp"
#include "System.hpp"

class AnimationSystem : public System<AnimationSystem>
{

public:

	using System::System;

	void On_Update()
	{
		//for (int i = 0; i < m_scene->GetEntityCount(); i++)
		for(int i: m_scene->GetIds())
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
