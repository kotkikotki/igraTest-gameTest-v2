#ifndef COLLISION_COMPONENT_HPP

#define COLLISION_COMPONENT_HPP

#include "Component.hpp"

using collision_area_var_t = std::variant<Collision_Box, Collision_Circle>;

enum CollisionState
{
	NOT_COLLIDING,
	ENTERING,
	STAYING
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
	CollisionComponent(Rectangle frameBox, float scale, Collision_Area_Type type)
	{
		if (type == COLLISION_BOX)
			collision_Area = Collision_Box{ frameBox.width * scale, frameBox.height * scale };
		else collision_Area = Collision_Circle{ sqrt(frameBox.width * frameBox.height / 4.f) * scale };
	}
	CollisionComponent(float radius)
	{
		collision_Area = Collision_Circle{radius};
	}

	//helper method, not a real functionality
	void UpdatePosition(Vector2 position)
	{
		if (auto area = std::get_if<Collision_Box>(&collision_Area))
		{
			//DrawRectangle(area1->postition.x - area1->width / 2.f, area1->postition.y - area1->height / 2.f
			area->postition = {position.x - area->width / 2.f, position.y - area->height / 2.f};
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

