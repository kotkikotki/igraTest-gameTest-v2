#ifndef SCRIPTS_HPP

#define SCRIPTS_HPP

#include "Definitions.hpp"

//Animation

class AnimationScript
{

public:

	Name_Variable_Tuple_Map m_Properties;
	Name_LinkedVariable_Tuple_Map m_LinkedProperties;

	//Rectangle mem_currentFrameRectangle = {};
	explicit AnimationScript(const std::initializer_list < std::pair<std::string, std::any>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}

	virtual void Animate(SpriteComponent& sprite) {}

	virtual void UpdateProperties() {}

	virtual void AddProperties(const std::initializer_list < std::pair<std::string, std::string>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}


};

class LoopAnimationScript : public AnimationScript
{
public:

	float m_frameSpeed = 1.f;
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


	int mem_currentFrame = 0;
	int mem_frameCounter = 0;

	//
	float mem_frameSpeed = 1.f;

	using AnimationScript::AnimationScript;


	void Animate(SpriteComponent& sprite) override
	{
		int currentFrameMin = (mem_frameSpeed >= 4.f) ? sprite.m_frameCountX : 0;
		int currentFrameMax = (mem_frameSpeed >= 4.f) ? sprite.m_frameCountX * sprite.m_frameCountY : sprite.m_frameCountX;
		mem_frameCounter++;
		if (mem_frameCounter >= (GetFPS() / mem_frameSpeed))
		{
			mem_frameCounter = 0;
			mem_currentFrame++;

			if (mem_currentFrame >= currentFrameMax) mem_currentFrame = currentFrameMin;
		}

		int currentFrameX = mem_currentFrame % sprite.m_frameCountX,
			currentFrameY = mem_currentFrame / sprite.m_frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(sprite.m_frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(sprite.m_frameCountY);
	}

	void UpdateProperties() override
	{
		if (!m_LinkedProperties.HasVariable("frameSpeed")) return;

		//mem_frameSpeed = mem_LinkedProperties.GetVariable<float>("frameSpeed");
		mem_frameSpeed = *std::static_pointer_cast<float>(m_LinkedProperties.GetVariablePtr("frameSpeed"));

	}

};

//Input
#include "Entity.hpp"
class InputScript
{

public:

	virtual void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity)
	{

	}

};

class RotateInputScript : public InputScript
{
public:

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



#endif // !SCRIPTS_HPP

