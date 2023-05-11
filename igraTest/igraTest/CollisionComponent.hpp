#ifndef COLLISION_COMPONENT_HPP

#define COLLISION_COMPONENT_HPP

#include "Component.hpp"

using collision_area_var_t = std::variant<Collision_Box, Collision_Circle>;

enum CollisionStates
{
	ENTERING,
	STAYING,
	NOT_COLLIDING
};


class CollisionComponent : public Component
{
	collision_area_var_t collision_Area;

public:

	

	using Component::Component;

	CollisionComponent(float width, float height)
	{
		collision_Area = Collision_Box{ width, height };
	}
	CollisionComponent(float radius)
	{
		collision_Area = Collision_Circle{radius};
	}

	void UpdatePosition(Vector2 position)
	{
		if (auto area = std::get_if<Collision_Box>(&collision_Area))
		{
			area->postition = position;
		}
		if (auto area = std::get_if<Collision_Circle>(&collision_Area))
		{
			area->postition = position;
		}
	}

	const collision_area_var_t& GetAreaRef()
	{
		return collision_Area;
	}


};



#endif // !COLLISION_COMPONENT_HPP

