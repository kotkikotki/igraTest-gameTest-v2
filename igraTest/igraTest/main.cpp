#include<iostream>
#include<vector>
#include<cmath>

#include "raylib.h"
#include "MusicSystem.hpp"
#include "BackgroundManager_Horizontal.hpp"
#include "BackgroundManager_Vertical.hpp"
#include "Definitions.hpp"
#include "Scene.hpp"

#include "Components.h"
#include "Systems.h"

#include "UsedScripts.h"
#include "UsedEntities.h"

#include"LoadedTextures.hpp"

int score = 0;
float backgroundSpeed = 0.f;

std::shared_ptr<InputMappings> mappings1 = std::make_shared<InputMappings>(InputMappings{});


void LoadTextures()
{
	//SpaceShip
	Texture2D SpaceShip_base = 
		LoadTexture("..\\..\\res\\assets\\used\\player-ship\\base\\Main Ship - Base - Full health.png");
	Texture2D SpaceShip_engineEffects =
		LoadTexture("..\\..\\res\\assets\\used\\player-ship\\engine effects\\Main Ship - Engines - Base Engine - Spritesheet.png");
	Texture2D SpaceShip_engine =
		LoadTexture("..\\..\\res\\assets\\used\\player-ship\\engine\\Main Ship - Engines - Base Engine.png");
	Texture2D SpaceShip_weapon =
		LoadTexture("..\\..\\res\\assets\\used\\player-ship\\weapons\\Main Ship - Weapons - Auto Cannon.png");
	Texture2D SpaceShip_projectile =
		LoadTexture("..\\..\\res\\assets\\used\\player-ship\\projectile\\Main ship weapon - Projectile - Rocket.png");
	//EnemyBoss
	Texture2D EnemyBoss_base =
		LoadTexture("..\\..\\res\\assets\\used\\enemy1\\Kla'ed - Battlecruiser - Base.png");
	Texture2D EnemyBoss_engine =
		LoadTexture("..\\..\\res\\assets\\used\\enemy1\\Kla'ed - Battlecruiser - Engine12.png");
	Texture2D EnemyBoss_shield =
		LoadTexture("..\\..\\res\\assets\\used\\enemy1\\Kla'ed - Battlecruiser - Shield.png");
	Texture2D EnemyBoss_particle =
		LoadTexture("..\\..\\res\\assets\\used\\enemy1\\Kla'ed - Battlecruiser - Destruction.png");
	//enemybomber
	Texture2D EnemyBomber_base =
		LoadTexture("..\\..\\res\\assets\\used\\enemy2\\Kla'ed - Bomber - Base.png");
	Texture2D EnemyBomber_engine =
		LoadTexture("..\\..\\res\\assets\\used\\enemy2\\Kla'ed - Bomber - Engine.png");
	Texture2D EnemyBomber_shield =
		LoadTexture("..\\..\\res\\assets\\used\\enemy2\\Kla'ed - Bomber - Shield.png");
	Texture2D EnemyBomber_particle =
		LoadTexture("..\\..\\res\\assets\\used\\enemy2\\Kla'ed - Bomber - Destruction.png");
	//PlayerProjectile
	Texture2D Explosion_particle =
		LoadTexture("..\\..\\res\\assets\\used\\explosion\\Explosion-duplicate frames.png");


	LoadedTextures::loadedTextures =
	{
		//spaceship
		{"SpaceShip_base",
		SpaceShip_base},
		{"SpaceShip_engineEffects",
		SpaceShip_engineEffects},
		{"SpaceShip_engine",
		SpaceShip_engine},
		{"SpaceShip_weapon",
		SpaceShip_weapon},
		{"SpaceShip_projectile",
		SpaceShip_projectile},
		{"SpaceShip_particle",
		Explosion_particle},
		//enemyboss
		{"EnemyBoss_base",
		EnemyBoss_base},
		{"EnemyBoss_engine",
		EnemyBoss_engine},
		{"EnemyBoss_particle",
		EnemyBoss_particle},
		{"EnemyBoss_shield",
		EnemyBoss_shield},
		//enemybomber
		{"EnemyBomber_base",
		EnemyBomber_base},
		{"EnemyBomber_engine",
		EnemyBomber_engine},
		{"EnemyBomber_particle",
		EnemyBomber_particle},
		{"EnemyBomber_shield",
		EnemyBomber_shield},
		//playerprojectile
		{"PlayerProjectile_particle",
		Explosion_particle}

	};

	for (auto& a : LoadedTextures::loadedTextures)
	{
		SpriteTextureUnloadHelper::AddTexture(a.second);
	}
}

void Update(MusicSystem &musicSystem, BackgroundManager_Vertical &backgroundManagerV, Scene &scene, 
	EntityDrawer &entityDrawer, AnimationSystem &animationSystem, InputSystem &inputSystem,
	CollisionSystem &collisionSystem, BehaviourSystem &behaviourSystem, PhysicsSystem &physicsSystem)
{	
	if(IsKeyPressed(KEY_P))
	{
		musicSystem.SetPause();
	}
	if (IsKeyDown(KEY_V))
	{
		backgroundManagerV.SetCurrentSpeed(max(backgroundManagerV.GetCurrentSpeed() - 3.f*GetFrameTime(), 0.f));
	}
	if (IsKeyDown(KEY_F))
	{
		backgroundManagerV.SetCurrentSpeed(min(backgroundManagerV.GetCurrentSpeed() + 3.f*GetFrameTime(), 20.f));
	}
	if (IsKeyReleased(KEY_K))
	{
		//collisionSystem.PrintCurrent();
	}
	/*
	if (IsKeyReleased(KEY_M))
	{
		Entity& enemy = scene.AddEntity({ "enemy" });
		enemy.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, -(float)GetScreenHeight() / 2.f }, 180.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("EnemyBoss_base"), { 1 }, 3.f);
		enemy.AddComponent<SpriteComponent>
			(base1);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBoss_engine") , {12}, 3.f }, "engine", 0);
		enemy.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("EnemyBoss_shield"), {16}, 3.f }, "shield", 0);
		enemy.AddComponent<AnimationComponent>
			(std::make_shared<EnemyBossAnimationScript>());
		enemy.AddComponent<CollisionComponent>(base1.m_currentFrameRectangle,
			base1.m_textureScale, COLLISION_CIRCLE,
			Vector2{ 0.f, 0.f }, 0.f, 0.56f);
		enemy.AddComponent<BehaviourComponent>(std::make_shared<EnemyBossScript>());
		enemy.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f }, false);

		//
		
	}
	if (IsKeyReleased(KEY_N))
	{
		Entity& enemy = scene.AddEntity({ "enemyBomber" });
		enemy.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, -(float)GetScreenHeight() / 2.f }, 180.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("EnemyBomber_base"), { 1 }, 2.f);
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
	*/
	if(IsKeyReleased(KEY_R)  && !scene.HasEntityByTag("player"))
	{
		Entity& e1 = scene.AddEntity({ "player" });

		e1.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, (float)GetScreenHeight() / 1.25f }, 0.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("SpaceShip_base"), { 1 }, 3.f, 1);
		e1.AddComponent<SpriteComponent>
			(base1);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_engineEffects"), {3, 4}, 3.f }, "engineEffects", 0);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_engine"), {1}, 3.f }, "engine", 0);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_weapon"), {7}, 3.f }, "weapon", 0);
		e1.AddComponent<AnimationComponent>
			(std::make_shared<SpaceShipAnimationScript>());
		e1.AddComponent<InputComponent>(std::make_shared<SpaceShipInputScript>(), mappings1);
		e1.AddComponent<CollisionComponent>(base1.m_currentFrameRectangle,
			base1.m_textureScale, COLLISION_CIRCLE,
			Vector2{ 0.f, 0.f }, 0.f, 0.77f);
		e1.AddComponent<BehaviourComponent>(std::make_shared<SpaceShipScript>());
		e1.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f }, false);
	}
	
	
	//audio
	musicSystem.UpdateMusic();
	//!audio

	

	//test
	scene.On_Update();

	behaviourSystem.On_Update();
	inputSystem.On_Update();
	physicsSystem.On_Update();
	collisionSystem.On_Update();
	animationSystem.On_Update();
	

	if (scene.HasEntityByTag("player"))
	{
		Entity& e = scene.GetEntity("player");

		if (e.HasComponent<BehaviourComponent>())
		{
			score = e.GetComponent<BehaviourComponent>().GetScript()->m_Properties.GetVariableT<int>("score");
			backgroundManagerV.SetCurrentSpeed
			(e.GetComponent<BehaviourComponent>().GetScript()->m_Properties.GetVariableT<float>("frameSpeed"));
		}
	}
	backgroundManagerV.UpdateTexturePositionValues();
	//!test
	
	//video
	BeginDrawing();

		ClearBackground(WHITE);

		backgroundManagerV.UpdateTexturePositions();
		//scene.GetEntity(0).GetComponent<TransformComponent>().m_position.x = GetMouseX();

		//test
		
		entityDrawer.On_Update_Draw();
		//collisionSystem.On_UpdateDrawTest();
		//DrawCircle((float)GetScreenWidth() / 2.f, (float)GetScreenHeight() / 0.75f / 1.25f / 2.f, 5.f, RED);
		//!test
		
		//DrawText("Hayo", GetScreenWidth() / 2, GetScreenHeight() / 2, 36, BLACK);
		std::string output = "Kill count: " + std::to_string(score);
		DrawText(output.c_str(), (float)GetScreenWidth() / 1.375f, (float)GetScreenHeight() / 1.125f, 36, WHITE);

	EndDrawing();
}



int main()
{
	
	//initialize

	//window
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();

	int targetFPS = 60;
	bool shouldBeFullscreen = true;

	//InitWindow(screenWidth, screenHeight, "Game");
	InitWindow(1200, 800, "Game");
	SetTargetFPS(targetFPS);
	//ToggleFullscreen();
	//!window

	//audio
	InitAudioDevice();
	std::vector<std::string> musicFilePaths = { "..\\..\\res\\as it was.wav", "..\\..\\res\\cinema.wav", "..\\..\\res\\daydreaming.wav" };

	MusicSystem musicSystem;
	musicSystem.LoadMusicFiles(musicFilePaths);
	musicSystem.InitMusicStreams(0.f);
	//!audio

	//background
	std::vector<TextureFilePath_ScrollingSpeed_Tuple> backgroundTuples = 
	{ {"..\\..\\res\\assets\\used\\background\\Blue_Nebula_02-1024x1024.png", 1.f} };
	BackgroundManager_Vertical backgorundManagerV;
	backgorundManagerV.SetCurrentSpeed(3.f);
	backgorundManagerV.SetPrioritizeHeight(false);
	backgorundManagerV.LoadTextures(backgroundTuples);
	//!background

	//sprite
	LoadTextures();

	mappings1->m_Map.AddAction("move_right", KEY_D, Down);
	mappings1->m_Map.AddAction("move_left", KEY_A, Down);
	mappings1->m_Map.AddAction("move_up", KEY_W, Down);
	mappings1->m_Map.AddAction("move_down", KEY_S, Down);
	mappings1->m_Map.AddAction("shoot", MOUSE_BUTTON_LEFT, Pressed);
	
	Scene s1;
	
	
	{
		//Texture2D playerTexture = LoadTexture("..\\..\\res\\assets\\used\\edited\\base.png");
		Entity& e1 = s1.AddEntity({ "player" });
		
		e1.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, (float)GetScreenHeight() / 1.25f }, 0.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("SpaceShip_base"), {1}, 3.f, 1);
		e1.AddComponent<SpriteComponent>
			(base1);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_engineEffects"), {3, 4}, 3.f }, "engineEffects", 0);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_engine"), {1}, 3.f }, "engine", 0);
		e1.GetComponent<SpriteComponent>().AddSprite
		(Sprite{ LoadedTextures::GetTexture("SpaceShip_weapon"), {7}, 3.f }, "weapon", 0);
		e1.AddComponent<AnimationComponent>
			(std::make_shared<SpaceShipAnimationScript>());
		e1.AddComponent<InputComponent>(std::make_shared<SpaceShipInputScript>(), mappings1);
		e1.AddComponent<CollisionComponent>(base1.m_currentFrameRectangle, 
			base1.m_textureScale, COLLISION_CIRCLE,
			Vector2{0.f, 0.f}, 0.f, 0.77f);
		e1.AddComponent<BehaviourComponent>(std::make_shared<SpaceShipScript>());
		e1.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f}, false);
		
		//
		
		
		
		Entity& wall_l = s1.AddEntity({ "blocking" });

		wall_l.AddComponent<TransformComponent>(Vector2{ -1.f, (float)GetScreenHeight()/2.f}, 0.f, false, false, 1.f);
		wall_l.AddComponent<CollisionComponent>(3.f, (float)GetScreenHeight(), Vector2{ 0.f, 0.f }, 0.f, 1.f);
		Entity& wall_r = s1.AddEntity({ "blocking" });

		wall_r.AddComponent<TransformComponent>(Vector2{ (float)GetScreenWidth() + 2.f, (float)GetScreenHeight() / 2.f}, 0.f, false, false, 1.f);
		wall_r.AddComponent<CollisionComponent>(3.f, (float)GetScreenHeight(), Vector2{ 0.f, 0.f }, 0.f, 1.f);
		


		//backgorundManagerV.SetSpeedPtr
		//(e1.GetComponent<BehaviourComponent>().GetScript()->m_LinkedProperties.GetVariablePtr("frameSpeed"));
		//score = e1.GetComponent<BehaviourComponent>().GetScript()->m_LinkedProperties.GetVariablePtr("score");
	}
	Entity& enemySpawner = s1.AddEntity({ "enemySpawner" });
	enemySpawner.AddComponent<BehaviourComponent>(std::make_shared<EnemySpawnerScript>());
	{
		/*
		Entity& enemy = s1.AddEntity({"enemy"});
		enemy.AddComponent<TransformComponent>
			(Vector2{ (float)GetScreenWidth() / 2.f, -(float)GetScreenHeight() / 2.f }, 180.f, false, false, 1.f);
		Sprite base1(LoadedTextures::GetTexture("EnemyBoss_base"), {1}, 3.f);
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
		enemy.AddComponent<BehaviourComponent>(std::make_shared<EnemyBossScript>(s1.GetEntity(0)));
		enemy.AddComponent<PhysicsComponent>(40000.f, Vector2{ 0.f , 0.f }, false);
		*/
		//

	}
	{
		/*
		Entity& enemy = s1.AddEntity({"enemyBomber"});
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
		*/
		//

	}
	//TODO miniaudio.h exception
	
	EntityDrawer entityDrawer(s1);
	entityDrawer.drawCollision = false;
	AnimationSystem animationSystem(s1);
	InputSystem inputSystem(s1);
	CollisionSystem collisionSystem(s1);
	BehaviourSystem behaviourSystem(s1);
	PhysicsSystem physicsSystem(s1);
	//!sprite

	//!initialize

	//HideCursor();
	//game loop
	while (!WindowShouldClose())
	{
		//Update(musicSystem, backgorundManagerV, s1, entityDrawer, animationSystem, inputSystem);
		Update(musicSystem, backgorundManagerV, s1, entityDrawer, animationSystem,
			inputSystem, collisionSystem, behaviourSystem, physicsSystem);
	}
	//!game loop


	//de-initialize
	CloseWindow();
	
	backgorundManagerV.De_init();
	
	
	//!de-initialize
	
	for (auto& texturePair : SpriteTextureUnloadHelper::textures)
	{
		UnloadTexture(*texturePair.second);
	}
	musicSystem.De_initMusics();
	CloseAudioDevice();

	return 0;
}