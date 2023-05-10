#ifndef COLLISION_COMPONENT_HPP

#define COLLISION_COMPONENT_HPP

#include "Component.hpp"

class CollisionComponent : public Component
{

public:

	Vector2 m_position;

	Collision_Rectangle m_CollisionBox;
	Collision_Circle m_CollisionCircle;

	using Component::Component;

	CollisionComponent(float width, float height) :
		m_CollisionBox(width, height),
		m_CollisionCircle(0.f){ }
	CollisionComponent(float radius) :
		m_CollisionBox(0.f, 0.f),
		m_CollisionCircle(radius) { }

};



#endif // !COLLISION_COMPONENT_HPP

