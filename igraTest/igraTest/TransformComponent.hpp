#ifndef TRANSFORM_COMPONENT_HPP

#define TRANSFORM_COMPONENT_HPP

#include "Component.hpp"

class TransformComponent : public Component
{
public:

	Vector2 m_position = {0};
	float m_rotation = 0.f;
	bool m_flipVertical = false;
	bool m_flipHorizontal = false;
	float m_scale = 1.f;

	//using Component::Component;
	

	void Initialize(const Vector2& position, float rotation, bool flipVertical, bool flipHorizontal, float scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_flipVertical = flipVertical;
		m_flipHorizontal = flipHorizontal;
		m_scale = scale;
	}
};


#endif // !TRANSFORM_COMPONENT_HPP

