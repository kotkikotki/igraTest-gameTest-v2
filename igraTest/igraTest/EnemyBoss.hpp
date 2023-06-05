#ifndef ENEMY_BOSS_HPP

#define ENEMY_BOSS_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"

class EnemyBossScript : public BehaviourScript
{

	float velocityScalar = 5.f;

	Texture2D particleTexture = { 0 };

	std::vector<Vector2> introLocations =
	{ 
		{ GetScreenWidth() / 2.f, GetScreenHeight() / 5.f },
	};

	Vector2 engineVelocity = { 0.f, 0.f };
	Vector2 maxVelocity = { 0.f, 7.f };

	float rotationRate = 5.f;
	//
	Entity* player;

	float health = 100.f;

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
		
		
		if (owner.HasComponentDisabled<CollisionComponent>())
		{
			owner.GetComponent<CollisionComponent>().m_enabled = true;
		}

		if (!introLocations.empty())
		{
			if (owner.HasComponent<CollisionComponent>())
			{
				owner.GetComponent<CollisionComponent>().m_enabled = false;
			}
			Vector2 location = introLocations[0];

			Vector2 direction = { location.x - transform.m_position.x,
				location.y - transform.m_position.y };

			float rotation = GetAngleOfPoint_Vertical(direction);
			bool flag = false;
			if (!(direction.x == 0.f && direction.y == 0.f))
			{
				//transform.m_rotation = rotation;
				float delta = rotation - transform.m_rotation;

				if (delta > 180.f)
				{
					delta = -(360.f - delta);
				}
				if (delta < -180.f)
				{
					delta = (360.f + delta);
				}
				//std::cout << delta << std::endl;
				if (delta > 0.f)
				{
					delta = min(rotationRate, delta);
				}
				else if (delta < 0.f)
				{
					delta = max(-rotationRate, delta);
				}

				transform.m_rotation += delta;
			}
				
			else
			{
				if (transform.m_rotation < 0.f)
				{
					transform.m_rotation = max(-180.f, transform.m_rotation - rotationRate);
				}
				else
				{
					transform.m_rotation = min(180.f, transform.m_rotation + rotationRate);
				}
				if (abs(transform.m_rotation) == 180.f)
				{
					//std::cout << "completed\n";
					flag = true;
					introLocations.erase(introLocations.begin());
				}

			}

			if(!flag)
			{
				engineVelocity = direction;

				physics.m_velocityVector = { physics.m_velocityVector.x + engineVelocity.x,
					physics.m_velocityVector.y + engineVelocity.y };

				Vector2 maxVelocityRotated = GetRotatedPoint(maxVelocity, { 0.f, 0.f }, -transform.m_rotation);

				{
					float newX = physics.m_velocityVector.x;
					if (newX >= 0.f)
					{
						newX = min(newX, direction.x);
					}
					else
					{
						newX = max(newX, direction.x);
					}
					float newY = physics.m_velocityVector.y;
					if (newY >= 0.f)
					{
						newY = min(newY, direction.y);
					}
					else
					{
						newY = max(newY, direction.y);
					}
					physics.m_velocityVector = { newX, newY };

				}
				if (physics.m_velocityVector.x != 0.f || physics.m_velocityVector.y != 0.f)
				{
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

				}

				transform.m_position = { transform.m_position.x + physics.m_velocityVector.x,
				transform.m_position.y + physics.m_velocityVector.y };



			}
		}

		if (introLocations.empty())
		{
			if (player != nullptr && player->HasComponent<TransformComponent>())
			{

				TransformComponent& playerTransform = player->GetComponent<TransformComponent>();


				Vector2 direction = { playerTransform.m_position.x - transform.m_position.x,
					playerTransform.m_position.y - transform.m_position.y };

				float rotation = GetAngleOfPoint_Vertical(direction);

				if (!(direction.x == 0.f && direction.y == 0.f))
				{
					//transform.m_rotation = rotation;
					float delta = rotation - transform.m_rotation;

					if (delta > 180.f)
					{
						delta = -(360.f - delta);
					}
					if (delta < -180.f)
					{
						delta = (360.f + delta);
					}
					//std::cout << delta << std::endl;
					if (delta > 0.f)
					{
						delta = min(rotationRate, delta);
					}
					else if (delta < 0.f)
					{
						delta = max(-rotationRate, delta);
					}

					transform.m_rotation += delta;
				}
				else
				{
					if (abs(transform.m_rotation) != 180.f)
					{
						if (transform.m_rotation < 0.f)
						{
							transform.m_rotation = max(-180.f, transform.m_rotation - rotationRate);
						}
						else if (transform.m_rotation > 0.f)
						{
							transform.m_rotation = min(180.f, transform.m_rotation + rotationRate);
						}
					}

				}

				/*
				engineVelocity = direction;

				physics.m_velocityVector = { physics.m_velocityVector.x + engineVelocity.x,
					physics.m_velocityVector.y + engineVelocity.y };

				Vector2 maxVelocityRotated = GetRotatedPoint(maxVelocity, { 0.f, 0.f }, -transform.m_rotation);

				{
					float newX = physics.m_velocityVector.x;
					if (newX >= 0.f)
					{
						newX = min(newX, direction.x);
					}
					else
					{
						newX = max(newX, direction.x);
					}
					float newY = physics.m_velocityVector.y;
					if (newY >= 0.f)
					{
						newY = min(newY, direction.y);
					}
					else
					{
						newY = max(newY, direction.y);
					}
					physics.m_velocityVector = { newX, newY };

				}
				if(physics.m_velocityVector.x!=0.f||physics.m_velocityVector.y!=0.f)
				{
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

				}

				transform.m_position = { transform.m_position.x + physics.m_velocityVector.x,
				transform.m_position.y + physics.m_velocityVector.y };
				*/


			}
		}
		
		
		m_Properties.ChangeVariableByName("frameSpeed", (velocityScalar));
		if (!(owner.HasComponent<AnimationComponent>())) return;
		AnimationComponent& animation = owner.GetComponent<AnimationComponent>();

		float value = (m_Properties.GetVariableT<float>("frameSpeed"));
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", value);
		
		//owner.GetComponent<CollisionComponent>().m_enabled = false;
	}

	EnemyBossScript(Entity& player) : BehaviourScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
		m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

		//if()
		particleTexture = LoadTexture
		("..\\..\\res\\assets\\used\\enemy1\\Kla'ed - Battlecruiser - Destruction.png");
		SpriteTextureUnloadHelper::AddTexture(particleTexture);

		this->player = &player;
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

			float damage = base.m_Properties.GetVariableT<float>("damage");

			

			health -= damage;
			if(health<=0.f)
			{
				auto& script = player.GetComponent<BehaviourComponent>().GetScript();
				int value = script->m_LinkedProperties.GetVariable<int>("score") + 1;

				script->m_LinkedProperties.ChangeVariableByName<int>("score", value);

				if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<SpriteComponent>())) return;
				TransformComponent& transform = owner.GetComponent<TransformComponent>();
				SpriteComponent& sprite = owner.GetComponent<SpriteComponent>();
				AddParticle(owner.GetOwner(),
					TransformComponent(transform.m_position, transform.m_rotation, false, false, transform.m_scale),
					SpriteComponent(Sprite(particleTexture, { 14 }, sprite.GetSprite("base").m_textureScale)), 1.f);

				owner.Destroy();
			}

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

	void On_Draw(Entity& owner) override
	{
		if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<SpriteComponent>())) return;

		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		SpriteComponent& sprites = owner.GetComponent<SpriteComponent>();
		Sprite& sprite = sprites.GetSprite("base");

		const float size = 200.f;

		DrawRectangle(transform.m_position.x - size/2.f,
			transform.m_position.y - sprite.m_currentFrameRectangle.height * sprite.m_textureScale/2.f + 60.f ,
			size, 10.f, WHITE);
		const float offsetX = 2.f;
		const float offsetY = 2.f;
		DrawRectangle(transform.m_position.x - size / 2.f + offsetX,
			transform.m_position.y - sprite.m_currentFrameRectangle.height * sprite.m_textureScale / 2.f + 60.f + offsetX,
			(size-offsetX*2.f)*(health/100.f),
			10.f-offsetY*2.f,
			RED);
	}


};

class EnemyBossAnimationScript : public AnimationScript
{
public:


	int m_currentFrame = 0;
	int m_frameCounter = 0;

	//
	float m_frameSpeed = 0.f;

	using AnimationScript::AnimationScript;

	EnemyBossAnimationScript() : AnimationScript()
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



#endif // !ENEMY_BOSS_HPP

