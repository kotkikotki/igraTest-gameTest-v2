#ifndef ENTITY_DRAWER_HPP

#define ENTITY_DRAWER_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"

class EntityDrawer : System<EntityDrawer>
{


public:

	bool drawCollision = false;

	using System :: System;

	void On_Update_Draw()
	{
		for (int i : m_scene->GetIds())
		{
			//test collision
			if(drawCollision)
			{
				//if (!(e->HasComponent<SpriteComponent>() && e->HasComponent<TransformComponent>())) return;
				if (!(m_scene->HasComponentById<CollisionComponent>(i))) continue;
				CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);




				if (auto area1 = std::get_if<Collision_Box>(&collision.GetAreaRef()))
				{
					//DrawRectangle(area1->postition.x - area1->width / 2.f, area1->postition.y - area1->height / 2.f, area1->width, area1->height, RED);
					//DrawRectangle(area1->postition.x, area1->postition.y, area1->width, area1->height, RED);
					Rectangle dest = { area1->position.x, area1->position.y, area1->width * area1->scale, area1->height * area1->scale };
					Vector2 origin = { dest.width / 2.f, dest.height / 2.f };
					DrawRectanglePro(dest, origin, area1->rotation, RED);
					
				}
				if (auto area1 = std::get_if<Collision_Circle>(&collision.GetAreaRef()))
				{
					DrawCircle(area1->position.x, area1->position.y, area1->radius*area1->scale, RED);
				}
			}

			//if (!(e->HasComponent<SpriteComponent>() && e->HasComponent<TransformComponent>())) return;
			if (!(m_scene->HasComponentById<SpriteComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) continue;

			SpriteComponent& sprite = m_scene->GetComponentById<SpriteComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);
			
			Rectangle dest = {transform.m_position.x, transform.m_position.y, sprite.m_currentFrameRectangle.width * sprite.m_textureScale, sprite.m_currentFrameRectangle.height * sprite.m_textureScale};
			Vector2 origin = { dest.width / 2.f, dest.height / 2.f };

			DrawTexturePro(sprite.m_texture, sprite.m_currentFrameRectangle, dest, origin, transform.m_rotation, WHITE);

			//std::cout << transform.m_rotation << std::endl;
			/*DrawTexturePro(sprite.m_texture, sprite.m_currentFrameRectangle,
				{ transform.m_position.x, transform.m_position.y, sprite.m_currentFrameRectangle.width * sprite.m_textureScale, sprite.m_currentFrameRectangle.height * sprite.m_textureScale },
				{ sprite.m_currentFrameRectangle.width * SQRT2, sprite.m_currentFrameRectangle.height * SQRT2 }, transform.m_rotation, WHITE);
			*/
			
			//DrawCircle(transform.m_position.x, transform.m_position.y, 70.f, BLUE);
		}
	}
	
};


#endif // !ENTITY_DRAWER_HPP
