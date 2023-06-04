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

	LoopAnimationScript() : AnimationScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
	}

	
	
	void Animate(SpriteComponent& sprites) override
	{
 
		Sprite& sprite = sprites.GetSprite("base");
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

	AdvancedLoopAnimationScript() : AnimationScript()
	{
		m_Properties.AddVariable("frameSpeed", 0.f);
	}
	
	void Animate(SpriteComponent& sprites) override
	{
		Sprite& sprite = sprites.GetSprite("base");
		auto frameCountX = sprite.m_framesOnRow;
		auto frameCountY = sprite.m_frameCount.size();
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

		int currentFrameX = m_currentFrame % frameCountX,
			currentFrameY = m_currentFrame / frameCountX;


		sprite.m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite.m_texture.width / (float)(frameCountX);
		sprite.m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite.m_texture.height / (float)(frameCountY);
	}
	void UpdateProperties() override
	{
		m_frameSpeed = m_Properties.GetVariableT<float>("frameSpeed");
	}

};


#endif // !BASIC_ANIMATION_SCRIPTS_HPP

