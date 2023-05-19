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

	virtual void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
	{
	}
	virtual void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
	{
	}
	virtual void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
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
	std::function<void(Entity& entity)> MoveUp = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.y -= behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> MoveDown = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		BehaviourScript& behaviourScript = *entity.GetComponent<BehaviourComponent>().GetScript();

		transform.m_position.y += behaviourScript.m_Properties.GetVariableT<float>("frameSpeed");
	};
	std::function<void(Entity& entity)> RotateRight = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		
		transform.m_rotation += 3.f;
	};
	std::function<void(Entity& entity)> RotateLeft = [&](Entity& entity) ->void
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		transform.m_rotation -= 3.f;
	};
	SpaceShipScript() : BehaviourScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);

		//emplace functions
		m_actions.emplace("move_left", MoveLeft);
		m_actions.emplace("move_right", MoveRight);
		m_actions.emplace("move_up", MoveUp);
		m_actions.emplace("move_down", MoveDown);
		m_actions.emplace("rotate_left", RotateLeft);
		m_actions.emplace("rotate_right", RotateRight);
	}
	//
	//colision
	void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "enter" <<owner.GetId()<<"-"<<hit.GetId()<< std::endl;
		TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();

		ownerTransform.m_position.x += collisionInfo.separation.x;
		ownerTransform.m_position.y += collisionInfo.separation.y;

		/*
		TransformComponent& hitTransform = hit.GetComponent<TransformComponent>();

		hitTransform.m_position.x += collisionInfo.separation.x;
		hitTransform.m_position.y += collisionInfo.separation.y;
		*/
	}
	void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "stay" << owner.GetId() << "-" << hit.GetId()<<std::endl;
		TransformComponent& ownerTransform = owner.GetComponent<TransformComponent>();

		ownerTransform.m_position.x += collisionInfo.separation.x;
		ownerTransform.m_position.y += collisionInfo.separation.y;

		/*
		TransformComponent& hitTransform = hit.GetComponent<TransformComponent>();

		hitTransform.m_position.x += collisionInfo.separation.x;
		hitTransform.m_position.y += collisionInfo.separation.y;
		*/
	}
	void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo) override
	{
		std::cout << "exit" << owner.GetId() << "-" << hit.GetId()<<std::endl;
		//hit.GetComponent<TransformComponent>().m_position = collisionInfo.separation;
		
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
		
		if (mappings->m_Map.GetActionState("move_left"))
			behaviour.GetScript()->On_Action(entity, "move_left");

		if (mappings->m_Map.GetActionState("move_up"))
			behaviour.GetScript()->On_Action(entity, "move_up");

		if (mappings->m_Map.GetActionState("move_down"))
			behaviour.GetScript()->On_Action(entity, "move_down");
		
		if (mappings->m_Map.GetActionState("rotate_right"))
			behaviour.GetScript()->On_Action(entity, "rotate_right");
		
		if (mappings->m_Map.GetActionState("rotate_left"))
			behaviour.GetScript()->On_Action(entity, "rotate_left");

			
	}


};




#endif // !SCRIPTS_HPP

