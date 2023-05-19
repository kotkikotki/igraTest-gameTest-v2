#ifndef BASIC_INPUT_SCRIPTS_HPP

#define BASIC_INPUT_SCRIPTS_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"

class MoveInputScript : public InputScript
{

public:

	using InputScript::InputScript;

	void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity)
	{
		if (!entity.HasComponent<BehaviourComponent>()) return;

		BehaviourComponent& behaviour = entity.GetComponent<BehaviourComponent>();

		if (mappings->m_Map.GetActionState("move_right"))
			behaviour.GetScript()->On_Action(entity, "move_right");

		if (mappings->m_Map.GetActionState("move_left"))
			behaviour.GetScript()->On_Action(entity, "move_left");

		if (mappings->m_Map.GetActionState("move_up"))
			behaviour.GetScript()->On_Action(entity, "move_up");

		if (mappings->m_Map.GetActionState("move_down"))
			behaviour.GetScript()->On_Action(entity, "move_down");

		if (mappings->m_Map.GetActionState("rotate_right"))
			behaviour.GetScript()->On_Action(entity, "rotate_right");

		if (mappings->m_Map.GetActionState("rotate_left"))
			behaviour.GetScript()->On_Action(entity, "rotate_left");


	}


};

#endif // !BASIC_INPUT_SCRIPTS_HPP


