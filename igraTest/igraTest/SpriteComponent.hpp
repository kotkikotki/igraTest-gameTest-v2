/*#ifndef SPRITE_COMPONENT_HPP

#define SPRITE_COMPONENT_HPP

#include "Component.hpp"

class SpriteComponent : public Component
{
	
public:

	

	Texture2D m_texture = { 0 };

	int m_frameCountX = 1; //static
	int m_frameCountY = 1;

	float m_textureScale = 1.f;

	Rectangle m_currentFrameRectangle = {};

	unsigned int m_layer = 0; //0 -> min

	float m_relative_rotation = 0.f;
	Vector2 m_relative_position = { 0.f, 0.f };

	using Component::Component;
	
	SpriteComponent(const Texture2D& texture, int frameCountX, int frameCountY, float textureScale, unsigned int layer = 0,
		float relative_rotation = 0.f, const Vector2& relative_position = {0.f, 0.f}) :
		m_texture(texture),
		m_frameCountX(frameCountX),
		m_frameCountY(frameCountY),
		m_textureScale(textureScale),
		m_layer(layer),
		m_relative_rotation(relative_rotation),
		m_relative_position(relative_position)
	{
		m_currentFrameRectangle = { 0.f, 0.f, (float)m_texture.width / (float)m_frameCountX, (float)m_texture.height / (float)m_frameCountY };
	}
	/*
	~SpriteComponent()
	{
		UnloadTexture(m_texture);
	}
	
};



#endif // !SPRITE_COMPONENT_HPP

*/