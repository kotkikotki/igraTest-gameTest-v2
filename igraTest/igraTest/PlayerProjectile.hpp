#ifndef PLAYER_PROJECTILE_HPP

#define PLAYER_PROJECTILE_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"

class PlayerProjectileScript : public BehaviourScript
{

	float velocityScalar = 5.f;

	float acceleration = 1.f;

	Vector2 engineVelocity = { 0.f, 0.f };

	Vector2 maxVelocity = { -7.f, -7.f };
	Vector2 minVelocity = { 7.f, 7.f };

	Texture2D projectileTexture = { 0 };

public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		m_LinkedProperties.ChangeVariableByName<float>("frameSpeed", m_Properties.GetVariableT<float>("frameSpeed"));
	}

	void On_Update(Entity& owner) override
	{

		if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<PhysicsComponent>())) return;

		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();

		float offsetX = 0.f;
		float offsetY = 0.f;
		
		//if (owner.HasComponent<SpriteComponent>())
		if(owner.HasComponent<SpriteComponent>())
		{
			//SpriteComponent& sprite = owner.GetComponent<SpriteComponent>();
			Sprite& sprite = owner.GetComponent<SpriteComponent>().GetSprite("base");
			offsetX = sprite.m_currentFrameRectangle.width / 2.f;
			offsetY = sprite.m_currentFrameRectangle.height / 2.f;
		}

		if (transform.m_position.x > (float)GetScreenWidth() + offsetX || transform.m_position.x + offsetX < 0.f
			|| transform.m_position.y >(float)GetScreenWidth() + offsetY || transform.m_position.y + offsetY < 0.f)
		{
			owner.Destroy();
			return;
		}

		
		engineVelocity = GetRotatedPoint(Vector2{ 0.f, -acceleration }, Vector2{ 0.f, 0.f }, transform.m_rotation);
		physics.m_velocityVector = { physics.m_velocityVector.x + engineVelocity.x,
			physics.m_velocityVector.y + engineVelocity.y };


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

		transform.m_position = { transform.m_position.x + physics.m_velocityVector.x,
			transform.m_position.y + physics.m_velocityVector.y};

		m_Properties.ChangeVariableByName("frameSpeed", (velocityScalar));
		if (!(owner.HasComponent<AnimationComponent>())) return;
		AnimationComponent& animation = owner.GetComponent<AnimationComponent>();

		float value = (m_Properties.GetVariableT<float>("frameSpeed"));
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", value);
	}

	PlayerProjectileScript(const Texture2D& texture) : BehaviourScript()
	{
		m_tags = { "projectile" };

		m_Properties.AddVariable("frameSpeed", 0.f);
		m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

		//emplace functions

		projectileTexture = texture;
	}


	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		if (!hit.HasTag("player"))
			owner.Destroy();
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		//if (!hit.HasTag("player"));
		//	owner.Destroy();
	}
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{

	}


};

class PlayerProjectileAnimationScript : public AnimationScript
{
public:


	int m_currentFrame = 0;
	int m_frameCounter = 0;

	//
	float m_frameSpeed = 0.f;

	using AnimationScript::AnimationScript;

	PlayerProjectileAnimationScript() : AnimationScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
	}
	/*
	void Animate(SpriteComponent& sprite) override
	{
		m_frameCounter++;
		if (m_frameCounter >= (GetFPS() / m_frameSpeed))
		{
			m_frameCounter = 0;
			m_currentFrame++;

			if (m_currentFrame >= (sprite.m_frameCountX * sprite.m_frameCountY)) m_currentFrame = 0;
		}

		int currentFrameX = m_currentFrame % sprite.m_frameCountX,
			currentFrameY = m_currentFrame / sprite.m_frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(sprite.m_frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(sprite.m_frameCountY);
	}
	*/
	void Animate(SpriteComponent& sprites) override
	{
		Sprite& sprite = sprites.GetSprite("base");
		m_frameCounter++;
		if (m_frameCounter >= (GetFPS() / m_frameSpeed))
		{
			m_frameCounter = 0;
			m_currentFrame++;

			if (m_currentFrame >= (sprite.m_frameCountX * sprite.m_frameCountY)) m_currentFrame = 0;
		}

		int currentFrameX = m_currentFrame % sprite.m_frameCountX,
			currentFrameY = m_currentFrame / sprite.m_frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(sprite.m_frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(sprite.m_frameCountY);
	}
	void UpdateProperties() override
	{
		m_frameSpeed = m_Properties.GetVariableT<float>("frameSpeed");
		//sstd::cout << m_frameSpeed << std::endl;
	}

};



#endif // !PLAYER_PROJECTILE_HPP

