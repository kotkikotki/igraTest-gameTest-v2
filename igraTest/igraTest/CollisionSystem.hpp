#ifndef COLLISION_SYSTEM_HPP

#define COLLISION_SYSTEM_HPP


#include <vector>
#include "Definitions.hpp"
#include "System.hpp"

class CollisionSystem : public System<CollisionSystem>
{

	SparseArray<SparseArray<CollisionState>> m_collisionMatrix;

	bool AreColliding(int id1, int id2)
	{
		if (!(m_scene->HasComponentById<CollisionComponent>(id1) && m_scene->HasComponentById<TransformComponent>(id1))) return false;
		if (!(m_scene->HasComponentById<CollisionComponent>(id2) && m_scene->HasComponentById<TransformComponent>(id2))) return false;

		CollisionComponent& collision1 = m_scene->GetComponentById<CollisionComponent>(id1);
		CollisionComponent& collision2 = m_scene->GetComponentById<CollisionComponent>(id2);

		if (auto area1 = std::get_if<Collision_Box>(&collision1.GetAreaRef()))
		{
			Rectangle rect1 = { area1->width, area1->height };
			if (auto area2 = std::get_if<Collision_Box>(&collision2.GetAreaRef()))
			{
				return CheckCollisionRecs(rect1, { area2->width, area2->height });
			}
			if (auto area2 = std::get_if<Collision_Circle>(&collision2.GetAreaRef()))
			{
				return CheckCollisionCircleRec(area2->postition, area2->radius, rect1);
			}
		}
		if (auto area1 = std::get_if<Collision_Circle>(&collision1.GetAreaRef()))
		{
			if (auto area2 = std::get_if<Collision_Box>(&collision2.GetAreaRef()))
			{
				return CheckCollisionCircleRec(area1->postition, area1->radius, {area2->width, area2->height});
			}
			if (auto area2 = std::get_if<Collision_Circle>(&collision2.GetAreaRef()))
			{
				return CheckCollisionCircles(area1->postition, area1->radius, area2->postition, area2->radius);

			}
		}

		return false;

	}


public:

	using System::System;

	void On_Update()
	{
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<CollisionComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) continue;

			CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);

			Vector2 position;
			if (!(m_scene->HasComponentById<SpriteComponent>(i)))
			{

			}


			collision.UpdatePosition(transform.m_position);

		}
		
		/*
		//updating collision positions
		for (int i : m_scene->GetIds())
		{
			if (!(m_scene->HasComponentById<CollisionComponent>(i) && m_scene->HasComponentById<TransformComponent>(i))) return;

			CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);
			TransformComponent& transform = m_scene->GetComponentById<TransformComponent>(i);

			collision.UpdatePosition(transform.m_position);

		}
		
		SparseArray<SparseArray<CollisionState>> newMatrix;

		for (int i : m_scene->GetIds())
		{
			newMatrix.Emplace(i);

			for (int j : m_scene->GetIds())
			{
				if (i == j)
				{
					newMatrix[i].Emplace(j, NOT_COLLIDING);
					continue;
				}
				CollisionState currentState = NOT_COLLIDING;
				if (m_collisionMatrix.HasId(i) && m_collisionMatrix[i].HasId(j))
					currentState = m_collisionMatrix[i][j];

				bool areColliding = AreColliding(i, j);

				if (areColliding)
				{
					if (currentState == NOT_COLLIDING)
						currentState = ENTERING;
					else
						currentState = STAYING;
				}
				else
				{
					currentState = NOT_COLLIDING;
				}
				newMatrix[i].Emplace(j, currentState);
			}
		}
		//m_collisionMatrix = std::move(newMatrix);
		m_collisionMatrix = newMatrix;
		*/
	}
	void On_UpdateDrawTest()
	{
		for (int i : m_scene->GetIds())
		{
			//if (!(e->HasComponent<SpriteComponent>() && e->HasComponent<TransformComponent>())) return;
			if (!(m_scene->HasComponentById<CollisionComponent>(i))) continue;
			CollisionComponent& collision = m_scene->GetComponentById<CollisionComponent>(i);




			if (auto area1 = std::get_if<Collision_Box>(&collision.GetAreaRef()))
			{
				DrawRectangle(area1->postition.x, area1->postition.y, area1->width, area1->height, RED);
			}
			if (auto area1 = std::get_if<Collision_Circle>(&collision.GetAreaRef()))
			{
				DrawCircle(area1->postition.x, area1->postition.y, area1->radius, RED);
			}

		}
	}
	//helper method
	void PrintCurrent()
	{
		/*
		for (int i: m_scene->GetIds())
		{
			for (int j : m_scene->GetIds())
			{
				std::cout << m_collisionMatrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
		*/
	}

};


#endif // !ANIMATION_SYSTEM_HPP
