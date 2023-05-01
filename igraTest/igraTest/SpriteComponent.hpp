#ifndef SPRITE_COMPONENT_HPP

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

	//using Component::Component;
	

	void Initialize(const Texture2D& texture, int frameCountX, int frameCountY, float textureScale)
	{
		m_texture = texture;
		m_frameCountX = frameCountX;
		m_frameCountY = frameCountY;
		m_textureScale = textureScale;

		m_currentFrameRectangle = { 0.f, 0.f, (float)m_texture.width / (float)m_frameCountX, (float)m_texture.height / (float)m_frameCountY};
	}
};



#endif // !SPRITE_COMPONENT_HPP

