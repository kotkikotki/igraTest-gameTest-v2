#ifndef SPACE_SHIP_HPP

#define SPACE_SHIP_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"

class SpaceShipScript : public BehaviourScript
{

	float velocityScalar = 5.f;

	float accelerationX = 2.f;
	float brakeX = 0.1f;

	Vector2 engineVelocity = { 0.f, 0.f };

	Vector2 maxVelocity = { -7.f, -15.f };
	Vector2 minVelocity = { 7.f, 4.f };

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

		Vector2 p1 = { transform.m_position.x - (float)GetMouseX(), 0.f };
		Vector2 p2 = { transform.m_position.x - (float)GetMouseX(), 
			max(0.f, transform.m_position.y - (float)GetMouseY()) };
		
		float rotation = -GetAngleOfPoints(p1, p2);
		{
			if (rotation > 0.f)
			{
				rotation = 90.f - rotation;
			}
			else if (rotation < 0.f)
			{
				rotation = -90.f - rotation;
			}
			else
			{
				rotation = transform.m_rotation;
			}

			if (rotation >= 0.f)
			{
				rotation = min(rotation, 45.f);
			}
			else
			{
				rotation = max(rotation, -45.f);
			}
		}
		transform.m_rotation = rotation;
		//std::cout << rotation << std::endl;
		//
		{
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
	
	
			physics.m_velocityVector = { newX, physics.m_velocityVector.y };

			transform.m_position = { transform.m_position.x + physics.m_velocityVector.x,transform.m_position.y };
			
			if (physics.m_velocityVector.x > 0.f)
			{
				physics.m_velocityVector.x -= min(0.1f, physics.m_velocityVector.x);
			}
			if (physics.m_velocityVector.x < 0.f)
			{
				physics.m_velocityVector.x -= max(-0.1f, physics.m_velocityVector.x);
			}
		}	


		m_Properties.ChangeVariableByName("frameSpeed", (velocityScalar));
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
		float finalAcceleration = accelerationX * massAffection;

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
		float finalAcceleration = -accelerationX * massAffection;

		//
		Vector2 addition = { GetFrameTime() * finalAcceleration, 0.f };



		engineVelocity = { v0.x + addition.x, v0.y + addition.y };
	};
	
	SpaceShipScript() : BehaviourScript()
	{
		tags = { "player" };

		m_Properties.AddVariable("frameSpeed", 0.f);
		m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

		//emplace functions
		m_actions.emplace("move_left", MoveLeft);
		m_actions.emplace("move_right", MoveRight);
		/*
		m_actions.emplace("move_up", MoveUp);
		m_actions.emplace("move_down", MoveDown);
		m_actions.emplace("rotate_left", RotateLeft);
		m_actions.emplace("rotate_right", RotateRight);
		*/
	}
	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "enter" << owner.GetId() << "-" << hit.GetId() << std::endl;
		//TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();
		if (!(owner.HasComponent<PhysicsComponent>())) return;
		PhysicsComponent& ownerPhysics = owner.GetComponent<PhysicsComponent>();

		ownerPhysics.m_velocityVector.x = 2.f * collisionInfo.separation.x;
		ownerPhysics.m_velocityVector.y = 2.f * collisionInfo.separation.y;

		
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "stay" << owner.GetId() << "-" << hit.GetId() << std::endl;
		//TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();
		if (!(owner.HasComponent<PhysicsComponent>())) return;
		PhysicsComponent& ownerPhysics = owner.GetComponent<PhysicsComponent>();

		ownerPhysics.m_velocityVector.x = 2.f * collisionInfo.separation.x;
		ownerPhysics.m_velocityVector.y = 2.f * collisionInfo.separation.y;
		
	}
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "exit" << owner.GetId() << "-" << hit.GetId() << std::endl;
		//hit.GetComponent<TransformComponent>().m_position = collisionInfo.separation;

	}


};

class SpaceShipAnimationScript : public AnimationScript
{
public:


	int m_currentFrame = 0;
	int m_frameCounter = 0;

	//
	float m_frameSpeed = 0.f;

	using AnimationScript::AnimationScript;

	SpaceShipAnimationScript() : AnimationScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
	}

	void Animate(SpriteComponent& sprite) override
	{

		m_frameSpeed = abs(m_frameSpeed);
		int currentFrameMin = (m_frameSpeed >= 7.f) ? sprite.m_frameCountX : 0;
		int currentFrameMax = (m_frameSpeed >= 7.f) ? sprite.m_frameCountX * sprite.m_frameCountY : sprite.m_frameCountX;
		m_frameCounter++;
		if (m_frameCounter >= (GetFPS() / m_frameSpeed))
		{
			m_frameCounter = 0;
			m_currentFrame++;

			if (m_currentFrame >= currentFrameMax) m_currentFrame = currentFrameMin;
		}

		int currentFrameX = m_currentFrame % sprite.m_frameCountX,
			currentFrameY = m_currentFrame / sprite.m_frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(sprite.m_frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(sprite.m_frameCountY);
	}

	void UpdateProperties() override
	{
		/*
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		//mem_frameSpeed = mem_LinkedProperties.GetVariable<float>("frameSpeed");
		m_frameSpeed = *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed"));
		*/
		m_frameSpeed = m_Properties.GetVariableT<float>("frameSpeed");
	}

};

class SpaceShipInputScript : public InputScript
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

#endif // !SPACE_SHIP_HPP

