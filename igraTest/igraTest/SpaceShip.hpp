#ifndef SPACE_SHIP_HPP

#define SPACE_SHIP_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"

class SpaceShipScript : public BehaviourScript
{



public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		m_Properties.ChangeVariableByName("frameSpeed", *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed")));
		
		//

	}

	void On_Update(Entity& owner) override
	{
		/*
		if (!owner.HasComponent<PhysicsComponent>()) return;

		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();
		physics.m_velocity += m_Properties.GetVariableT<float>("frameSpeed");
		*/
	}

	std::function<void(Entity& entity)> MoveRight = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.x += behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> MoveLeft = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.x -= behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> MoveUp = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.y -= behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> MoveDown = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.y += behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> RotateRight = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		transform.m_rotation += 3.f;
	};
	std::function<void(Entity& entity)> RotateLeft = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		transform.m_rotation -= 3.f;
	};
	SpaceShipScript() : BehaviourScript()
	{
		tags = { "player" };

		m_Properties.AddVariable("frameSpeed", 0.f);

		//emplace functions
		m_actions.emplace("move_left", MoveLeft);
		m_actions.emplace("move_right", MoveRight);
		m_actions.emplace("move_up", MoveUp);
		m_actions.emplace("move_down", MoveDown);
		m_actions.emplace("rotate_left", RotateLeft);
		m_actions.emplace("rotate_right", RotateRight);
	}
	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "enter" << owner.GetId() << "-" << hit.GetId() << std::endl;
		TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();

		ownerTransform.m_position.x += collisionInfo.separation.x;
		ownerTransform.m_position.y += collisionInfo.separation.y;

		/*
		TransformComponent& hitTransform = hit.GetComponent<TransformComponent>();

		hitTransform.m_position.x += collisionInfo.separation.x;
		hitTransform.m_position.y += collisionInfo.separation.y;
		*/
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "stay" << owner.GetId() << "-" << hit.GetId() << std::endl;
		TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();

		ownerTransform.m_position.x += collisionInfo.separation.x;
		ownerTransform.m_position.y += collisionInfo.separation.y;

		/*
		TransformComponent& hitTransform = hit.GetComponent<TransformComponent>();

		hitTransform.m_position.x += collisionInfo.separation.x;
		hitTransform.m_position.y += collisionInfo.separation.y;
		*/
	}
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "exit" << owner.GetId() << "-" << hit.GetId() << std::endl;
		//hit.GetComponent<TransformComponent>().m_position = collisionInfo.separation;

	}


};

#endif // !SPACE_SHIP_HPP

