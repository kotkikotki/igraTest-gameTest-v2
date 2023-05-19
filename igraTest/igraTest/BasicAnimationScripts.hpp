#ifndef BASIC_ANIMATION_SCRIPTS_HPP

#define BASIC_ANIMATION_SCRIPTS_HPP

#include<raylib.h>
#include "Definitions.hpp"
#include "Components.h"

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


#endif // !BASIC_ANIMATION_SCRIPTS_HPP
