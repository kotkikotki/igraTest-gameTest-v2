#ifndef ENEMY_SPAWNER_HPP

#define ENEMY_SPAWNER_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"
#include "Scene.hpp"



class EnemySpawnerScript : public BehaviourScript
{

	
	Entity* player;

	const double c_durationToSpawnBomber = 1.f;
	const double c_durationToSpawnBoss = 10.f;
	int bomberMultiplier = 0;

	timepoint lastSpawnBomber;
	timepoint lastSpawnBoss;



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
			bomberMultiplier = 1;
		}
		
		if (player != nullptr)
		{
			bomberMultiplier = 
				player->GetComponent<BehaviourComponent>().GetScript()->m_Properties.GetVariableT<int>("score")/10 + 1;
		}
		timepoint now = GetCurrentTime();
		if (std::chrono::duration_cast<seconds_d>(now - lastSpawnBomber).count() >= c_durationToSpawnBomber)
		{
			
			for(int i = 0; i<max<int>(bomberMultiplier, 3); i++)
				SpawnBomber(owner.GetOwner());
			lastSpawnBomber = GetCurrentTime();
		}
		if (std::chrono::duration_cast<seconds_d>(now - lastSpawnBoss).count() >= c_durationToSpawnBoss)
		{
			if (!owner.GetOwner().HasEntityByTag("enemyBoss"));
			SpawnBoss(owner.GetOwner());
			lastSpawnBoss = GetCurrentTime();
		}
	}

	EnemySpawnerScript() : BehaviourScript()
	{
		lastSpawnBomber = GetCurrentTime();
		lastSpawnBoss = GetCurrentTime();
		
	}

	void SpawnBoss(Scene& scene)
	{
		Entity& enemy = scene.AddEntity({ "enemy" , "enemyBoss"});
		enemy.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, -(float)GetScreenHeight() / 2.f }, 180.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("EnemyBoss_base"), { 1 }, 3.f);
		enemy.AddComponent<SpriteComponent>
			(base1);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBoss_engine"), {12}, 3.f }, "engine", 0);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBoss_shield"), {16}, 3.f }, "shield", 0);
		enemy.AddComponent<AnimationComponent>
			(std::make_shared<EnemyBossAnimationScript>());
		enemy.AddComponent<CollisionComponent>(base1.m_currentFrameRectangle,
			base1.m_textureScale, COLLISION_CIRCLE,
			Vector2{ 0.f, 0.f }, 0.f, 0.56f);
		enemy.AddComponent<BehaviourComponent>(std::make_shared<EnemyBossScript>());
		enemy.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f }, false);
	}

	void SpawnBomber(Scene& scene)
	{
		Entity& enemy = scene.AddEntity({"enemy", "enemyBomber"});
		enemy.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, -(float)GetScreenHeight() / 2.f }, 180.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("EnemyBomber_base"), {1}, 2.f);
		enemy.AddComponent<SpriteComponent>
			(base1);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBomber_engine"), {10}, 2.f }, "engine", 0);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBomber_shield"), {6}, 2.f }, "shield", 0);
		enemy.AddComponent<AnimationComponent>
			(std::make_shared<EnemyBomberAnimationScript>());
		enemy.AddComponent<CollisionComponent>(base1.m_currentFrameRectangle,
			base1.m_textureScale, COLLISION_CIRCLE,
			Vector2{ 0.f, 0.f }, 0.f, 0.56f);
		enemy.AddComponent<BehaviourComponent>(std::make_shared<EnemyBomberScript>());
		enemy.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f }, false);
	}

	//
	//colision
	

	void On_Draw(Entity& owner) override
	{

	}


};



#endif // !ENEMY_SPAWNER_HPP

