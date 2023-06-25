#ifndef ENEMY_TEST_HPP

#define ENEMY_TEST_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"

class EnemyTestScript : public BehaviourScript
{

	float velocityScalar = 5.f;
	
	float acceleration = 7.f;

	bool moveLeft = false;
public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		//m_LinkedProperties.ChangeVariableByName<float>("frameSpeed", m_Properties.GetVariableT<float>("frameSpeed"));
	}

	void On_Update(Entity& owner) override
	{
		
		if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<PhysicsComponent>())) return;
		
		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();
		
		if (moveLeft)
			transform.m_position = { transform.m_position.x - acceleration, transform.m_position.y };
		else
			transform.m_position = { transform.m_position.x + acceleration, transform.m_position.y };

		if (transform.m_position.x > (float)GetScreenWidth() / 1.25f) moveLeft = true;
		if (transform.m_position.x < (float)GetScreenWidth() * 0.25f) moveLeft = false;

		m_Properties.ChangeVariableByName("frameSpeed", (velocityScalar));
		if (!(owner.HasComponent<AnimationComponent>())) return;
		AnimationComponent& animation = owner.GetComponent<AnimationComponent>();

		float value = (m_Properties.GetVariableT<float>("frameSpeed"));
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", value);
		
		//owner.GetComponent<CollisionComponent>().m_enabled = false;
	}

	EnemyTestScript() : BehaviourScript()
	{
		

		m_Properties.AddVariable("frameSpeed", 0.f);
		//m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));
	}


	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		if (hit.HasTag("projectile"))
		{
			BehaviourScript& base = *hit.GetComponent<BehaviourComponent>().GetScript();
			PlayerProjectileScript& scriptProjectile = static_cast<PlayerProjectileScript&>(base);
			Entity& player = scriptProjectile.GetPlayer();

			auto& script = player.GetComponent<BehaviourComponent>().GetScript();

			int value = script->m_Properties.GetVariableT<int>("score") + 1;

			script->m_Properties.ChangeVariableByName("score", (int)value);

			PlaySound(LoadedSounds::GetSound("explosion"));
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


};

class EnemyTestAnimationScript : public AnimationScript
{
public:


	int m_currentFrame = 0;
	int m_frameCounter = 0;

	//
	float m_frameSpeed = 0.f;

	using AnimationScript::AnimationScript;

	EnemyTestAnimationScript() : AnimationScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
	}

	void Animate(SpriteComponent& sprites) override
	{
		Sprite& sprite = sprites.GetSprite("engine");
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
		//std::cout << m_frameSpeed << std::endl;
	}

};



#endif // !ENEMY_TEST_HPP

