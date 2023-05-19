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

	CollisionComponent(float width, float height, const Vector2& position_r = { 0.f, 0.f }, float rotation_r  = 0.f, float scale_r = 1.f)
	{
		collision_Area = Collision_Box( width, height, position_r, rotation_r, scale_r);
	}
	CollisionComponent(Rectangle frameBox, float frameBoxScale, Collision_Area_Type type, const Vector2& position_r = { 0.f, 0.f }, float rotation_r = 0.f, float scale_r = 1.f)
	{
		if (type == COLLISION_BOX)
			collision_Area = Collision_Box( frameBox.width * frameBoxScale, frameBox.height * frameBoxScale, position_r, rotation_r, scale_r);
		else collision_Area = Collision_Circle( sqrt(frameBox.width * frameBox.height)/2.f* frameBoxScale, position_r, rotation_r, scale_r);
	}
	CollisionComponent(float radius, const Vector2& position_r = { 0.f, 0.f }, float rotation_r = 0.f, float scale_r = 1.f)
	{
		collision_Area = Collision_Circle(radius, position_r, rotation_r, scale_r);
	}

	//helper method, not a real functionality
	void UpdateMembers(const Vector2& position, float rotation, float scale)
	{
		if (auto area = std::get_if<Collision_Box>(&collision_Area))
		{
			//DrawRectangle(area1->postition.x - area1->width / 2.f, area1->postition.y - area1->height / 2.f
			area->position = { position.x + area->position_r.x, position.y + area->position_r.y };
			area->rotation = rotation + area->rotation_r;
			area->scale = scale * area->scale_r;
			
		}
		if (auto area = std::get_if<Collision_Circle>(&collision_Area))
		{
			area->position = { position.x + area->position_r.x, position.y + area->position_r.y };
			area->rotation = rotation + area->rotation_r;
			area->scale = scale * area->scale_r;
		}
	}

	const collision_area_var_t& GetAreaRef()
	{
		return collision_Area;
	}


};



#endif // !COLLISION_COMPONENT_HPP

