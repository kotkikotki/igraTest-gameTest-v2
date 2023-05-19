#ifndef PHYSICS_COMPONENT_HPP

#define PHYSICS_COMPONENT_HPP

#include "Component.hpp"

class PhysicsComponent : public Component
{
public:

	float m_mass = 0.f; // in kg
	float m_velocity = 0.f;

	bool m_gravity = false;

	using Component::Component;

	PhysicsComponent(float mass, float velocity, bool gravity) :
		m_mass(mass),
		m_velocity(velocity),
		m_gravity(gravity) {}

};


#endif // !PHYSICS_COMPONENT_HPP


