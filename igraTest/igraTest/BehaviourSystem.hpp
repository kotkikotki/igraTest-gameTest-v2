#ifndef BEHAVIOUR_SYSTEM_HPP

#define BEHAVIOUR_SYSTEM_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"


class BehaviourSystem : public System<BehaviourSystem>
{


public:

	using System::System;

	void On_Update()
	{
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<BehaviourComponent>(i))) continue;

			BehaviourComponent& behaviour = m_scene->GetComponentById<BehaviourComponent>(i);

			
			behaviour.GetScript()->On_Update(m_scene->GetEntity(i));
			behaviour.GetScript()->UpdateProperties();
		}

	}

};

#endif // !INPUT_SYSTEM_HPP

