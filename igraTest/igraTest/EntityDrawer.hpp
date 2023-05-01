#ifndef ENTITY_DRAWER_HPP

#define ENTITY_DRAWER_HPP

#include "Entity.hpp"
#include "Components.h"
#include <vector>
#include "Definitions.hpp"


class EntityDrawer
{

	Scene* m_scene;

public:
	EntityDrawer(Scene& scene)
	{
		m_scene = &scene;
	}
	void On_Update_Draw()
	{
		for (int i = 0; i<m_scene->GetIdCount(); i++)
		{
			//if (!(e->HasComponent<SpriteComponent>() && e->HasComponent<TransformComponent>())) return;
			if (!(m_scene->HasComponentById<SpriteComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) return;
			
			SpriteComponent& sprite = m_scene->GetComponentById<SpriteComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);



			DrawTexturePro(sprite.m_texture, sprite.m_currentFrameRectangle,
				{ transform.m_position.x, transform.m_position.y, sprite.m_currentFrameRectangle.width * sprite.m_textureScale, sprite.m_currentFrameRectangle.height * sprite.m_textureScale },
				{ sprite.m_currentFrameRectangle.width* SQRT2, sprite.m_currentFrameRectangle.height* SQRT2 }, transform.m_rotation, WHITE);
			
		}
	}
};


#endif // !ENTITY_DRAWER_HPP
