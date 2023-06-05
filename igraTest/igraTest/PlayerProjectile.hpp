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

	Texture2D particleTexture = { 0 };


	Vector2 engineVelocity = { 0.f, 0.f };
	/*
	Vector2 maxVelocity = { -7.f, -7.f };
	Vector2 minVelocity = { 7.f, 7.f };
	*/
	Vector2 maxVelocity = { 0.f, 7.f };
	

	//Texture2D projectileTexture = { 0 };
	
	Entity* player;

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

		
		Vector2 maxVelocityRotated = GetRotatedPoint(maxVelocity, { 0.f, 0.f }, -transform.m_rotation);

		float newX = physics.m_velocityVector.x;
		if (newX >= 0.f)
		{
			newX = min(newX, maxVelocityRotated.x);
		}
		else
		{
			newX = max(newX, maxVelocityRotated.x);
		}
		float newY = physics.m_velocityVector.y;
		if (newY >= 0.f)
		{
			newY = min(newY, -maxVelocityRotated.y);
		}
		else
		{
			newY = max(newY, -maxVelocityRotated.y);
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

	PlayerProjectileScript(const Texture2D& texture, Entity& owner) : BehaviourScript()
	{

		m_Properties.AddVariable("frameSpeed", 0.f);
		m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

		m_Properties.AddVariable("damage", 20.f);

		//emplace functions
		player = &owner;
		//projectileTexture = texture;

		particleTexture = LoadTexture
		("..\\..\\res\\assets\\used\\player-ship\\projectile\\Explosion-duplicate frames.png");
		SpriteTextureUnloadHelper::AddTexture(particleTexture);
	}


	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		if (hit.HasTag("enemy"))
		{
			if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<SpriteComponent>())) return;
			TransformComponent& transform = owner.GetComponent<TransformComponent>();
			SpriteComponent& sprite = owner.GetComponent<SpriteComponent>();
			AddParticle(owner.GetOwner(),
				TransformComponent(transform.m_position, transform.m_rotation, false, false, transform.m_scale),
				SpriteComponent(Sprite(particleTexture, { 14 }, sprite.GetSprite("base").m_textureScale/2.f)), 
				0.5f);

			owner.Destroy();
		}
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		//if (!hit.HasTag("player"));
		//	owner.Destroy();
	}
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{

	}

	Entity& GetPlayer()
	{
		return *player;
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
	
	void Animate(SpriteComponent& sprites) override
	{
		Sprite& sprite = sprites.GetSprite("base");
		auto frameCountX = sprite.m_framesOnRow;
		auto frameCountY = sprite.m_frameCount.size();
		m_frameCounter++;
		if (m_frameCounter >= (GetFPS() / m_frameSpeed))
		{
			m_frameCounter = 0;
			m_currentFrame++;

			if (m_currentFrame >= (frameCountX * frameCountY)) m_currentFrame = 0;
		}

		int currentFrameX = m_currentFrame % frameCountX,
			currentFrameY = m_currentFrame / frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(frameCountY);
	}
	void UpdateProperties() override
	{
		m_frameSpeed = m_Properties.GetVariableT<float>("frameSpeed");
	}

};



#endif // !PLAYER_PROJECTILE_HPP

