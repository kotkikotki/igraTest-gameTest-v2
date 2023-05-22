#ifndef PHYSICS_COMPONENT_HPP

#define PHYSICS_COMPONENT_HPP

#include "Component.hpp"

class PhysicsComponent : public Component
{
public:

	float m_mass = 0.f; // in kg
	//float m_velocity = 0.f;

	bool m_gravity = false;

	Vector2 m_velocityVector = {0.f, 0.f};

	using Component::Component;

	PhysicsComponent(float mass, const Vector2& velocityVector, bool gravity) :
		m_mass(mass),
		m_velocityVector(velocityVector),
		m_gravity(gravity) {}

};


#endif // !PHYSICS_COMPONENT_HPP


