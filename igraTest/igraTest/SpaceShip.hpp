#ifndef SPACE_SHIP_HPP

#define SPACE_SHIP_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"

class SpaceShipScript : public BehaviourScript
{

	//float acceleration = 5000.f / 1000.f;
	float acceleration = 0.5f;

	Vector2 maxVelocity = {-3.f, -15.f};
	Vector2 minVelocity = { 3.f, 4.f };

	Vector2 engineVelocity = { 0.f, 0.f };

public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		m_LinkedProperties.ChangeVariableByName<float>("frameSpeed", m_Properties.GetVariableT<float>("frameSpeed"));
	}
	
	void On_Update(Entity& owner) override
	{
		
		if (!(owner.HasComponent<TransformComponent>()&& owner.HasComponent<PhysicsComponent>())) return;

		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();

		engineVelocity = GetRotatedPoint(engineVelocity, { 0.f, 0.f }, transform.m_rotation);

		physics.m_velocityVector = { physics.m_velocityVector.x + engineVelocity.x,
			physics.m_velocityVector.y + engineVelocity.y };
		engineVelocity = { 0.f, 0.f };
		//

		float newX = physics.m_velocityVector.x;
		if (newX >= 0.f)
		{
			newX = min(newX, minVelocity.x);
		}
		else
		{
			newX = max(newX, maxVelocity.x);
		}
		float newY = physics.m_velocityVector.y;
		if (newY >= 0.f)
		{
			newY = min(newY, minVelocity.y);
		}
		else
		{
			newY = max(newY, maxVelocity.y);
		}

		
		physics.m_velocityVector = { newX, newY };
		
		transform.m_position = { transform.m_position.x + physics.m_velocityVector.x,transform.m_position.y };
		//std::cout << newY << std::endl;

		m_Properties.ChangeVariableByName("frameSpeed", -(physics.m_velocityVector.y));

		if (!(owner.HasComponent<AnimationComponent>())) return;
		AnimationComponent& animation = owner.GetComponent<AnimationComponent>();
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", (m_Properties.GetVariableT<float>("frameSpeed")));
	}

	std::function<void(Entity& entity)> MoveRight = [&](Entity& entity) ->void
	{
		if (!(entity.HasComponent<PhysicsComponent>() && entity.HasComponent<TransformComponent>())) return;
		PhysicsComponent& physics = entity.GetComponent<PhysicsComponent>();
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		float massAffection = sqrtf(sqrtf(physics.m_mass));
		Vector2 v0 = engineVelocity;
		float finalAcceleration = acceleration * massAffection;

		//
		Vector2 addition = { GetFrameTime() * finalAcceleration, 0.f };



		engineVelocity = { v0.x + addition.x, v0.y + addition.y };
	};
	std::function<void(Entity& entity)> MoveLeft = [&](Entity& entity) ->void
	{
		if (!(entity.HasComponent<PhysicsComponent>() && entity.HasComponent<TransformComponent>())) return;
		PhysicsComponent& physics = entity.GetComponent<PhysicsComponent>();
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		float massAffection = sqrtf(sqrtf(physics.m_mass));
		Vector2 v0 = engineVelocity;
		float finalAcceleration = -acceleration * massAffection;

		//
		Vector2 addition = { GetFrameTime() * finalAcceleration, 0.f };



		engineVelocity = { v0.x + addition.x, v0.y + addition.y };
	};
	std::function<void(Entity& entity)> MoveUp = [&](Entity& entity) ->void
	{
		if (!(entity.HasComponent<PhysicsComponent>()&& entity.HasComponent<TransformComponent>())) return;
		PhysicsComponent& physics = entity.GetComponent<PhysicsComponent>();
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		float massAffection = sqrtf(sqrtf(physics.m_mass));
		Vector2 v0 = engineVelocity;
		float finalAcceleration = -acceleration * massAffection;

		//
		Vector2 addition = { 0.f, GetFrameTime() * finalAcceleration };
		

	
		engineVelocity = {v0.x + addition.x, v0.y + addition.y };

		
	};
	std::function<void(Entity& entity)> MoveDown = [&](Entity& entity) ->void
	{
		if (!(entity.HasComponent<PhysicsComponent>() && entity.HasComponent<TransformComponent>())) return;
		PhysicsComponent& physics = entity.GetComponent<PhysicsComponent>();
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		float massAffection = sqrtf(sqrtf(physics.m_mass));
		Vector2 v0 = engineVelocity;
		float finalAcceleration = acceleration * massAffection;

		//
		Vector2 addition = { 0.f, GetFrameTime() * finalAcceleration };
		
		engineVelocity = { v0.x + addition.x, v0.y + addition.y };
		
	};
	std::function<void(Entity& entity)> RotateRight = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		transform.m_rotation += 1.f;

		
	};
	std::function<void(Entity& entity)> RotateLeft = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		transform.m_rotation -= 1.f;

		
	};
	SpaceShipScript() : BehaviourScript()
	{
		tags = { "player" };

		m_Properties.AddVariable("frameSpeed", 0.f);
		m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

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

