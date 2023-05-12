#ifndef INPUT_SYSTEM_HPP

#define INPUT_SYSTEM_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"


class InputSystem : public System<InputSystem>
{


public:

	using System::System;

	void On_Update()
	{
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<InputComponent>(i))) continue;

			InputComponent& input = m_scene->GetComponentById<InputComponent>(i);

			input.GetScript()->UpdateProperties();
			input.GetScript()->ProcessInput(input.GetMappings(), m_scene->GetEntity(i));


		}

	}
	
};

#endif // !INPUT_SYSTEM_HPP

