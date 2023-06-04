#ifndef ENTITY_DRAWER_HPP

#define ENTITY_DRAWER_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include <map>
#include "Definitions.hpp"
#include <unordered_set>


class EntityDrawer : System<EntityDrawer>
{
private:


public:

	bool drawCollision = false;

	using System :: System;
	

	void On_Update_Draw()
	{

		std::multimap<int, int> layeredIds;

		for (int i : m_scene->GetIds())
		{
			if (!((m_scene->HasComponentById<SpriteComponent>(i))
				&& m_scene->HasComponentById<TransformComponent>(i))) continue;
			
			int layer = m_scene->GetComponentById<SpriteComponent>(i).m_layer;
			//TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);
			
			layeredIds.emplace(std::make_pair(layer, i));
		}
		//test collision
		if (drawCollision)
		{
			for (int i: m_scene->GetIds())
			{//if (!(e->HasComponent<SpriteComponent>() && e->HasComponent<TransformComponent>())) return;
				if ((m_scene->HasComponentById<CollisionComponent>(i)))
				{
					CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);
					//std::cout << i << std::endl;

					if (auto area1 = std::get_if<Collision_Box>(&collision.GetAreaRef()))
					{
						
						Rectangle dest = { area1->position.x, area1->position.y, area1->width * area1->scale, area1->height * area1->scale };
						Vector2 origin = { dest.width / 2.f, dest.height / 2.f };
						DrawRectanglePro(dest, origin, area1->rotation, RED);

					}
					if (auto area1 = std::get_if<Collision_Circle>(&collision.GetAreaRef()))
					{
						DrawCircle(area1->position.x, area1->position.y, area1->radius * area1->scale, RED);
					}
				}
			}
		}

		//for (int i : m_scene->GetIds())
		for(auto& a: layeredIds)
		{
			//check already done above
			int i = a.second;

			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);
			SpriteComponent& sprites = m_scene->GetComponentById<SpriteComponent>(i);
			for (auto& a : sprites.m_layeredSprites)
			{
				auto& sprite = a.second.second;
				Rectangle dest = { transform.m_position.x, transform.m_position.y, sprite.m_currentFrameRectangle.width * sprite.m_textureScale, sprite.m_currentFrameRectangle.height * sprite.m_textureScale };
				Vector2 origin = { dest.width / 2.f, dest.height / 2.f };

				DrawTexturePro(sprite.m_texture, sprite.m_currentFrameRectangle, dest, origin, transform.m_rotation, WHITE);
				//
				
				
				//
			}
			
			//DrawCircle(transform.m_position.x, transform.m_position.y, 70.f, BLUE);
			
		}
	}
	
};


#endif // !ENTITY_DRAWER_HPP
