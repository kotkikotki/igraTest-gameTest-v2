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
#include "EntityDrawer.hpp"



void Update(MusicSystem &musicSystem, BackgroundManager_Vertical &backgroundManagerV, Scene &scene, 
	EntityDrawer &entityDrawer, AnimationSystem &animationSystem, InputSystem &inputSystem,
	CollisionSystem &collisionSystem, BehaviourSystem &behaviourSystem)
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
		collisionSystem.PrintCurrent();
	}
	//scene.GetEntity(1).GetComponent<TransformComponent>().m_position = GetMousePosition();
	
	//audio
	musicSystem.UpdateMusic();
	//!audio

	backgroundManagerV.UpdateTexturePositionValues();

	//test
	
	behaviourSystem.On_Update();
	inputSystem.On_Update();
	collisionSystem.On_Update();
	animationSystem.On_Update();
	//!test
	
	//video
	BeginDrawing();

		ClearBackground(WHITE);

		backgroundManagerV.UpdateTexturePositions();
		//scene.GetEntity(0).GetComponent<TransformComponent>().m_position.x = GetMouseX();

		//test
		
		entityDrawer.On_Update_Draw();
		//collisionSystem.On_UpdateDrawTest();
		
		//!test
		
		DrawText("Hayo", GetScreenWidth() / 2, GetScreenHeight() / 2, 36, BLACK);

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
	std::vector<TextureFilePath_ScrollingSpeed_Tuple> backgroundTuples = { {"..\\..\\res\\assets\\used\\Blue_Nebula_02-1024x1024.png", 1.f} };
	BackgroundManager_Vertical backgorundManagerV;
	backgorundManagerV.SetCurrentSpeed(3.f);
	backgorundManagerV.SetPrioritizeHeight(false);
	backgorundManagerV.LoadTextures(backgroundTuples);
	//!background

	//sprite

	std::shared_ptr<InputMappings> mappings1 = std::make_shared<InputMappings>(InputMappings{});
	mappings1->m_Map.AddAction("move_right", KEY_D, Down);
	mappings1->m_Map.AddAction("move_left", KEY_A, Down);
	mappings1->m_Map.AddAction("move_up", KEY_W, Down);
	mappings1->m_Map.AddAction("move_down", KEY_S, Down);
	mappings1->m_Map.AddAction("rotate_right", KEY_T, Down);
	mappings1->m_Map.AddAction("rotate_left", KEY_R, Down);
	std::shared_ptr<InputMappings> mappings2 = std::make_shared<InputMappings>(InputMappings{});
	mappings2->m_Map.AddAction("move_right", KEY_RIGHT, Down);
	mappings2->m_Map.AddAction("move_left", KEY_LEFT, Down);
	mappings2->m_Map.AddAction("move_up", KEY_UP, Down);
	mappings2->m_Map.AddAction("move_down", KEY_DOWN, Down);
	mappings2->m_Map.AddAction("rotate_right", KEY_KP_6, Down);
	mappings2->m_Map.AddAction("rotate_left", KEY_KP_4, Down);

	Scene s1;
	

	{
		Entity& e1 = s1.AddEntity();

		e1.AddComponent<TransformComponent>
			(Vector2{ GetScreenWidth() / 2.f, GetScreenHeight() / 1.25f }, 0.f, false, false, 1.f);
		e1.AddComponent<SpriteComponent>
			(LoadTexture("..\\..\\res\\assets\\used\\edited\\base.png"), 4, 2, 3.f);
		e1.AddComponent<AnimationComponent>
			(std::make_shared<AdvancedLoopAnimationScript>(AdvancedLoopAnimationScript()));
		e1.GetComponent<AnimationComponent>().
			GetScript()->m_LinkedProperties.AddVariable("frameSpeed", backgorundManagerV.GetCurrentSpeedPtr());
		e1.AddComponent<InputComponent>(std::make_shared<MoveInputScript>(MoveInputScript()), mappings1);
		e1.AddComponent<CollisionComponent>(e1.GetComponent<SpriteComponent>().m_currentFrameRectangle, 
			e1.GetComponent<SpriteComponent>().m_textureScale, COLLISION_CIRCLE,
			Vector2{0.f, 0.f}, 0.f, 0.77f);
		e1.AddComponent<BehaviourComponent>(std::make_shared<SpaceShipScript>(SpaceShipScript()));
		e1.GetComponent<BehaviourComponent>().
			GetScript()->m_LinkedProperties.AddVariable("frameSpeed", backgorundManagerV.GetCurrentSpeedPtr());
		//
		Entity& e2 = s1.AddEntity();

		e2.AddComponent<TransformComponent>
			(Vector2{ GetScreenWidth() / 2.5f, GetScreenHeight() / 1.25f }, 0.f, false, false, 1.f);
		e2.AddComponent<SpriteComponent>(LoadTexture("..\\..\\res\\assets\\used\\edited\\base.png"), 4, 2, 3.f);
		e2.AddComponent<AnimationComponent>
			(std::make_shared<AdvancedLoopAnimationScript>(AdvancedLoopAnimationScript()));
		e2.GetComponent<AnimationComponent>().
			GetScript()->m_LinkedProperties.AddVariable("frameSpeed", backgorundManagerV.GetCurrentSpeedPtr());
		e2.AddComponent<InputComponent>(std::make_shared<MoveInputScript>(MoveInputScript()), mappings2);
		e2.AddComponent<CollisionComponent>(e2.GetComponent<SpriteComponent>().m_currentFrameRectangle,
			e1.GetComponent<SpriteComponent>().m_textureScale, COLLISION_CIRCLE,
			Vector2{ 0.f, 0.f }, 0.f, 0.77f);
		e2.AddComponent<BehaviourComponent>(std::make_shared<SpaceShipScript>(SpaceShipScript()));
		e2.GetComponent<BehaviourComponent>().
			GetScript()->m_LinkedProperties.AddVariable("frameSpeed", backgorundManagerV.GetCurrentSpeedPtr());
	}
	EntityDrawer entityDrawer(s1);
	entityDrawer.drawCollision = true;
	AnimationSystem animationSystem(s1);
	InputSystem inputSystem(s1);
	CollisionSystem collisionSystem(s1);
	BehaviourSystem behaviourSystem(s1);
	//!sprite

	//!initialize

	//HideCursor();
	//game loop
	while (!WindowShouldClose())
	{
		//Update(musicSystem, backgorundManagerV, s1, entityDrawer, animationSystem, inputSystem);
		Update(musicSystem, backgorundManagerV, s1, entityDrawer, animationSystem, inputSystem, collisionSystem, behaviourSystem);
	}
	//!game loop


	//de-initialize
	CloseWindow();
	musicSystem.De_initMusics();
	backgorundManagerV.De_init();
	
	CloseAudioDevice();
	//!de-initialize

	return 0;
}