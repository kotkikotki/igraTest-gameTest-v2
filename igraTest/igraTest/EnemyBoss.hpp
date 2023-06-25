#ifndef ENEMY_BOSS_HPP

#define ENEMY_BOSS_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"
#include"EnemyBossProjectile.hpp"

class EnemyBossScript : public BehaviourScript
{

	float velocityScalar = 5.f;

	Texture2D particleTexture = { 0 };
	Texture2D projectileTexture = { 0 };

	std::vector<Vector2> introLocations =
	{ 
		{ GetScreenWidth() / 2.f, GetScreenHeight() / 5.f },
	};

	Vector2 engineVelocity = { 0.f, 0.f };
	Vector2 maxVelocity = { 0.f, 7.f };

	float rotationRate = 1.f;
	//
	Entity* player;

	float health = 400.f;
	const float maxHealth = 400.f;

	const double c_durationToSpawnProjectile = 3.f;
	timepoint lastSpawnProjectile;




public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		//m_LinkedProperties.ChangeVariableByName<float>("frameSpeed", m_Properties.GetVariableT<float>("frameSpeed"));
	}

	void On_Update(Entity& owner) override
	{
		if (owner.GetOwner().HasEntityByTag("player"))
		{
			player = &owner.GetOwner().GetEntity("player");
		}
		else
		{
			player = nullptr;
		}
		if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<PhysicsComponent>())) return;

		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();
		
		
		if (owner.HasComponentDisabled<CollisionComponent>())
		{
			owner.GetComponent<CollisionComponent>().m_enabled = true;
		}
		if (owner.HasComponent<SpriteComponent>())
		{
			owner.GetComponent<SpriteComponent>().GetSprite("shield").m_visible = false;
		}

		if (!introLocations.empty())
		{
			if (owner.HasComponent<CollisionComponent>())
			{
				owner.GetComponent<CollisionComponent>().m_enabled = false;
			}
			if (owner.HasComponent<SpriteComponent>())
			{
				owner.GetComponent<SpriteComponent>().GetSprite("shield").m_visible = true;
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

				


			}
			timepoint now = GetCurrentTime();
			if (std::chrono::duration_cast<seconds_d>(now - lastSpawnProjectile).count() >= c_durationToSpawnProjectile)
			{

				
				//SpawnBomber(owner.GetOwner());
				//lastSpawnBomber = GetCurrentTime();
				Shoot(owner);
				lastSpawnProjectile = GetCurrentTime();
			}
		}
		
		
		m_Properties.ChangeVariableByName("frameSpeed", (velocityScalar));
		if (!(owner.HasComponent<AnimationComponent>())) return;
		AnimationComponent& animation = owner.GetComponent<AnimationComponent>();

		float value = (m_Properties.GetVariableT<float>("frameSpeed"));
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", value);
		
		//owner.GetComponent<CollisionComponent>().m_enabled = false;
	}

	EnemyBossScript() : BehaviourScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
		//m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));

		//if()
		particleTexture = LoadedTextures::GetTexture("EnemyBoss_particle");
		projectileTexture = LoadedTextures::GetTexture("EnemyBoss_projectile");
		
		lastSpawnProjectile = GetCurrentTime();
		//this->player = &player;
	}

	void Shoot(Entity& entity)
	{
		if (!(entity.HasComponent<TransformComponent>())) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		Entity& projectile = entity.GetOwner().AddEntity({ "projectileEnemy" });

		float offsetHeight = 0.f;
		float offsetWidth = 0.f;
		if (entity.HasComponent<SpriteComponent>())
		{
			Sprite& sprite = entity.GetComponent<SpriteComponent>().GetSprite("base");
			offsetHeight = sprite.m_currentFrameRectangle.height * 0.75f;
			//

			//offsetWidth = sprite.m_currentFrameRectangle.width * 0.58f;
			offsetWidth = 0.f;
		}

		projectile.AddComponent<TransformComponent>
			(
				GetRotatedPoint(Vector2{ transform.m_position.x + offsetWidth, transform.m_position.y - offsetHeight },
					transform.m_position, transform.m_rotation),
				transform.m_rotation, false, false, 1.f);
		projectile.AddComponent<SpriteComponent>
			(Sprite{ projectileTexture, {6}, 3.f });
		projectile.AddComponent<AnimationComponent>
			(std::make_shared<EnemyBossProjectileAnimationScript>());
		//.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", 5.f);

		projectile.AddComponent<CollisionComponent>(projectile.GetComponent<SpriteComponent>().GetSprite("base").m_currentFrameRectangle,
			projectile.GetComponent<SpriteComponent>().GetSprite("base").m_textureScale, COLLISION_CIRCLE,
			Vector2{ -1.f, 5.f }, 0.f, 0.42f);
		projectile.AddComponent<BehaviourComponent>
			(std::make_shared<EnemyBossProjectileScript>(entity));
		projectile.AddComponent<PhysicsComponent>(3000.f, Vector2{ 0.f, 0.f }, false);


		PlaySound(LoadedSounds::GetSound("laser"));

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
				int value = script->m_Properties.GetVariableT<int>("score") + 1;

				script->m_Properties.ChangeVariableByName("score", (int)value);

				if (!(owner.HasComponent<TransformComponent>() && owner.HasComponent<SpriteComponent>())) return;
				TransformComponent& transform = owner.GetComponent<TransformComponent>();
				SpriteComponent& sprite = owner.GetComponent<SpriteComponent>();
				AddParticle(owner.GetOwner(),
					TransformComponent(transform.m_position, transform.m_rotation, false, false, transform.m_scale),
					SpriteComponent(Sprite(particleTexture, { 14 }, sprite.GetSprite("base").m_textureScale)), 1.f);

				PlaySound(LoadedSounds::GetSound("explosion"));
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
			transform.m_position.y - sprite.m_currentFrameRectangle.height * sprite.m_textureScale / 2.f + 60.f + offsetY,
			(size-offsetX*2.f)*(health/maxHealth),
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
		std::vector<Sprite*> spritesToAnimate =
		{
			&sprites.GetSprite("engine"),
			&sprites.GetSprite("shield")
		};
		for(auto &a: spritesToAnimate)
		{
			Sprite& sprite = *a;
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
	}

	void UpdateProperties() override
	{
		m_frameSpeed = m_Properties.GetVariableT<float>("frameSpeed");
		//std::cout << m_frameSpeed << std::endl;
	}

};



#endif // !ENEMY_BOSS_HPP

