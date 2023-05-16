#ifndef SCRIPTS_HPP

#define SCRIPTS_HPP

#include<functional>
#include "Definitions.hpp"
#include "Entity.hpp"


class Script
{
public:

	Name_Variable_Tuple_Map m_Properties;
	Name_LinkedVariable_Tuple_Map m_LinkedProperties;

	explicit Script() : Script({}, {})
	{
	}
	explicit Script(const std::initializer_list < std::pair<std::string, std::any>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto &pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto &pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}

	virtual void UpdateProperties() {}

	virtual void AddProperties(const std::initializer_list < std::pair<std::string, std::string>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto &pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto &pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}
};

//Behaviour
class CollisionEvents
{

public:

	virtual void On_Enter()
	{
	}
	virtual void On_Stay()
	{
	}
	virtual void On_Exit()
	{
	}
};

class BehaviourScript : public Script, public CollisionEvents
{
	

protected:
	
	std::unordered_map<std::string, std::function<void(Entity& entity)>> m_actions;

public:

	using Script::Script;

	BehaviourScript() : Script() , CollisionEvents()
	{
	}

	virtual void On_Action(Entity& entity, const std::string& action)
	{
		if (m_actions.find(action) == m_actions.end()) return;
		m_actions[action](entity);
	}
};



class SpaceShipScript : public BehaviourScript
{



public:

	using BehaviourScript::BehaviourScript;

	
	
	void UpdateProperties() override
	{
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		m_Properties.ChangeVariableByName("frameSpeed", *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed")));
	}
	std::function<void(Entity& entity)> MoveRight = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.x += behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> MoveLeft = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.x -= behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	SpaceShipScript() : BehaviourScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);

		//emplace functions
		m_actions.emplace("move_left", MoveLeft);
		m_actions.emplace("move_right", MoveRight);
	}
	//
	//colision
	void On_Enter() override
	{
		std::cout << "enter" << std::endl;
	}
	void On_Stay()
	{
		std::cout << "stay" << std::endl;
	}
	void On_Exit()
	{
		std::cout << "exit" << std::endl;
	}


};



// Animation
class AnimationScript : public Script
{

public:

	using Script::Script;

	virtual void Animate(SpriteComponent& sprite) {}

};

class LoopAnimationScript : public AnimationScript
{
public:

	float m_frameSpeed = 0.f;
	int m_currentFrame = 0;
	int m_frameCounter = 0;

	using AnimationScript::AnimationScript;

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

	void UpdateProperties() override
	{
		//frameSpeed
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		//mem_frameSpeed = mem_LinkedProperties.GetVariablePtr<float>("frameSpeed");
		m_frameSpeed = *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed"));
	}

};

class AdvancedLoopAnimationScript : public AnimationScript
{
public:


	int m_currentFrame = 0;
	int m_frameCounter = 0;

	//
	float m_frameSpeed = 0.f;

	using AnimationScript::AnimationScript;


	void Animate(SpriteComponent& sprite) override
	{
		int currentFrameMin = (m_frameSpeed >= 4.f) ? sprite.m_frameCountX : 0;
		int currentFrameMax = (m_frameSpeed >= 4.f) ? sprite.m_frameCountX * sprite.m_frameCountY : sprite.m_frameCountX;
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
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		//mem_frameSpeed = mem_LinkedProperties.GetVariable<float>("frameSpeed");
		m_frameSpeed = *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed"));

	}

};

//Input

class InputScript : public Script
{

public:

	using Script::Script;

	virtual void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity) {}

};
/*
class RotateInputScript : public InputScript
{
public:

	using InputScript::InputScript;

	void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity)
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		if (mappings->m_Map.GetActionState("rotate_right"))
			transform.m_rotation += 2.f;
		if (mappings->m_Map.GetActionState("rotate_left"))
			transform.m_rotation -= 2.f;
	}

};

*/
class MoveInputScript : public InputScript
{

public:

	using InputScript::InputScript;

	void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity)
	{
		if (!entity.HasComponent<BehaviourComponent>()) return;

		BehaviourComponent& behaviour = entity.GetComponent<BehaviourComponent>();

		if (mappings->m_Map.GetActionState("move_right"))
			behaviour.GetScript()->On_Action(entity, "move_right");
			//std::cout << "lol" << std::endl;
		if (mappings->m_Map.GetActionState("move_left"))
			behaviour.GetScript()->On_Action(entity, "move_left");

			
	}


};




#endif // !SCRIPTS_HPP

