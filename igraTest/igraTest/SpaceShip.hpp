#ifndef SPACE_SHIP_HPP

#define SPACE_SHIP_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"
#include "PlayerProjectile.hpp"

class SpaceShipScript : public BehaviourScript
{

	float velocityScalar = 5.f;
	//
	float velocityScalarAcceleration = 0.07f;
	float velocityScalarMax = 15.f;

	float accelerationX = 2.f;
	float brakeX = 0.1f;

	Vector2 engineVelocity = { 0.f, 0.f };

	//Vector2 maxVelocity = { -7.f, -15.f };
	//Vector2 minVelocity = { 7.f, 4.f };
	float maxX = 7.f;

	Texture2D projectileTexture = {0};
	Texture2D particleTexture = {0};

	bool shootingLeft = false;

	float health = 1000.f;
	const float maxHealth = 1000.f;

public:

	using BehaviourScript::BehaviourScript;


	void UpdateProperties() override
	{
		//m_LinkedProperties.ChangeVariableByName<float>("frameSpeed", m_Properties.GetVariableT<float>("frameSpeed"));
	}
	
	void On_Update(Entity& owner) override
	{
		
		if (!(owner.HasComponent<TransformComponent>()&& owner.HasComponent<PhysicsComponent>())) return;

		TransformComponent& transform = owner.GetComponent<TransformComponent>();
		PhysicsComponent& physics = owner.GetComponent<PhysicsComponent>();
		
		float mouseX = (float)GetMouseX(),
			  mouseY = (float)GetMouseY();
		
		Vector2 direction = { mouseX - transform.m_position.x, 
			mouseY - transform.m_position.y };
		
	
		float rotation = GetAngleOfPoint_Vertical(direction);
		
		if(rotation>-90.f&&rotation<90.f)
		{
			if (rotation > 0.f)
			{
				rotation = min(60.f, rotation);
			}
			else if (rotation < 0.f)
			{
				rotation = max(-60.f, rotation);
			}
			transform.m_rotation = rotation;
		}
		
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
				newX = min(newX, maxX);
			}
			else
			{
				newX = max(newX, -maxX);
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

		float value = (m_Properties.GetVariableT<float>("frameSpeed"));
		animation.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", value);
		
		//transform.m_position = { (float)GetMouseX(), (float)GetMouseY() };
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

	std::function<void(Entity& entity)> MoveUp = [&](Entity& entity) ->void
	{
		velocityScalar = min(velocityScalar + velocityScalarAcceleration, velocityScalarMax);
	};
	std::function<void(Entity& entity)> MoveDown = [&](Entity& entity) ->void
	{
		velocityScalar = max(velocityScalar - velocityScalarAcceleration, 0.f);
	};
	
	std::function<void(Entity& entity)> Shoot = [&](Entity& entity) ->void
	{
		
		if (!(entity.HasComponent<TransformComponent>())) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		Entity& projectile = entity.GetOwner().AddEntity({ "projectile" });
		
		float offsetHeight = 0.f;
		float offsetWidth = 0.f;
		if (entity.HasComponent<SpriteComponent>())
		{
			Sprite& sprite = entity.GetComponent<SpriteComponent>().GetSprite("base");
			offsetHeight = sprite.m_currentFrameRectangle.height *0.75f;
			//
			
			offsetWidth = sprite.m_currentFrameRectangle.width * 0.58f;
			if (shootingLeft) offsetWidth *= -1.f;
			shootingLeft = !shootingLeft;
		}

		projectile.AddComponent<TransformComponent>
			(
				GetRotatedPoint(Vector2{ transform.m_position.x + offsetWidth, transform.m_position.y - offsetHeight },
					transform.m_position, transform.m_rotation),
				transform.m_rotation, false, false, 1.f);
		projectile.AddComponent<SpriteComponent>
			(Sprite{ projectileTexture, {3}, 3.f });
		projectile.AddComponent<AnimationComponent>
			(std::make_shared<PlayerProjectileAnimationScript>());
		//.GetScript()->m_Properties.ChangeVariableByName("frameSpeed", 5.f);

		projectile.AddComponent<CollisionComponent>(projectile.GetComponent<SpriteComponent>().GetSprite("base").m_currentFrameRectangle,
			projectile.GetComponent<SpriteComponent>().GetSprite("base").m_textureScale, COLLISION_CIRCLE,
			Vector2{ -1.f, 5.f }, 0.f, 0.42f);
		projectile.AddComponent<BehaviourComponent>
			(std::make_shared<PlayerProjectileScript>(entity));
		projectile.AddComponent<PhysicsComponent>(3000.f, Vector2{0.f, 0.f}, false);
		

		auto& weapon = *entity.GetComponent<AnimationComponent>().GetScript();
		weapon.AddSequence(entity.GetComponent<SpriteComponent>().GetSprite("weapon"), 0.3f);
		//

		//
		PlaySound(LoadedSounds::GetSound("laser"));
	};

	SpaceShipScript() : BehaviourScript()
	{


		m_Properties.AddVariable("frameSpeed", 0.f);
		//m_LinkedProperties.AddVariable("frameSpeed", std::make_shared<std::any>(0.f));
		
		//m_LinkedProperties.AddVariable("score", std::make_shared<std::any>((int)0));
		m_Properties.AddVariable("score", (int)0);

		//emplace functions
		m_actions.emplace("move_left", MoveLeft);
		m_actions.emplace("move_right", MoveRight);
		m_actions.emplace("move_up", MoveUp);
		m_actions.emplace("move_down", MoveDown);
		m_actions.emplace("shoot", Shoot);

		projectileTexture = LoadedTextures::GetTexture("SpaceShip_projectile");
		particleTexture = LoadedTextures::GetTexture("SpaceShip_particle");
	}

	

	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{

		if (!(owner.HasComponent<PhysicsComponent>())) return;
		PhysicsComponent& ownerPhysics = owner.GetComponent<PhysicsComponent>();

		if (hit.HasTag("blocking"))
		{
			ownerPhysics.m_velocityVector.x = 2.f * collisionInfo.separation.x;
			ownerPhysics.m_velocityVector.y = 2.f * collisionInfo.separation.y;
		}



		
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		
		if (!(owner.HasComponent<PhysicsComponent>())) return;
		PhysicsComponent& ownerPhysics = owner.GetComponent<PhysicsComponent>();
		if(hit.HasTag("blocking"))
		{
			ownerPhysics.m_velocityVector.x = 2.f * collisionInfo.separation.x;
			ownerPhysics.m_velocityVector.y = 2.f * collisionInfo.separation.y;
		}

		if (hit.HasTag("enemyBomber") || hit.HasTag("projectileEnemy"))
		{
			float maxDistance = 0.f;
			if (owner.HasComponent<SpriteComponent>())
			{
				Sprite& sprite = owner.GetComponent<SpriteComponent>().GetSprite("base");
				maxDistance = sqrtf(sprite.m_texture.width * sprite.m_texture.height) * sprite.m_textureScale / 5.f;
			}
			if (collisionInfo.distance > maxDistance) return;
			
			BehaviourScript& base = *hit.GetComponent<BehaviourComponent>().GetScript();
			float damage = base.m_Properties.GetVariableT<float>("damage");



			health -= damage;
			if (health <= 0.f)
			{
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
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{

	}

	void On_Draw(Entity& owner) override
	{

		const float size = 200.f;

		DrawRectangle(GetScreenWidth()/8.f,
			GetScreenHeight()*8.f/9.f,
			size, size/10.f, WHITE);
		const float offsetX = 2.f;
		const float offsetY = 2.f;
		DrawRectangle(GetScreenWidth() / 8.f + offsetX,
			GetScreenHeight() * 8.f / 9.f + offsetY,
			(size - offsetX * 2.f) * (health / maxHealth),
			size/10.f - offsetY * 2.f,
			RED);
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
	
	void Animate(SpriteComponent& sprites) override
	{
		
		Sprite& sprite = sprites.GetSprite("engineEffects");
		
		int frameCountY = sprite.m_frameCount.size();
		int frameCountX = sprite.m_framesOnRow;
			//sprite.m_frameCount[frameCountY-1];
		m_frameSpeed = abs(m_frameSpeed);
		int currentFrameMin = (m_frameSpeed >= 7.f) ? frameCountX : 0;
		int currentFrameMax = (m_frameSpeed >= 7.f) ? frameCountX * frameCountY : frameCountX;
		m_frameCounter++;
		if (m_frameCounter >= (GetFPS() / m_frameSpeed))
		{
			m_frameCounter = 0;
			m_currentFrame++;

			if (m_currentFrame >= currentFrameMax) m_currentFrame = currentFrameMin;
		}

		int currentFrameY = m_currentFrame / frameCountX,
			currentFrameX = m_currentFrame % frameCountX;
		if (currentFrameX >= sprite.m_frameCount[currentFrameY])
		{
			m_currentFrame++;

			if (m_currentFrame >= currentFrameMax) m_currentFrame = currentFrameMin;
			currentFrameY = m_currentFrame / frameCountX;
			currentFrameX = m_currentFrame % frameCountX;
		}
		

		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(frameCountY);
	}

	void UpdateProperties() override
	{
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

		if (mappings->m_Map.GetActionState("shoot"))
			behaviour.GetScript()->On_Action(entity, "shoot");


	}


};

#endif // !SPACE_SHIP_HPP

