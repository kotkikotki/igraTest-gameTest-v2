#ifndef COLLISION_SYSTEM_HPP

#define COLLISION_SYSTEM_HPP

#include<cassert>
#include <vector>
#include "Definitions.hpp"
#include "System.hpp"

class CollisionSystem : public System<CollisionSystem>
{

	SparseArray<SparseArray<std::pair<CollisionState, CollisionInfo>>> m_collisionMatrix;

	std::optional<CollisionInfo> AreColliding(int id1, int id2)
	{
		if (!(m_scene->HasComponentById<CollisionComponent>(id1) && m_scene->HasComponentById<TransformComponent>(id1))) 
			return std::optional<CollisionInfo>{};
		if (!(m_scene->HasComponentById<CollisionComponent>(id2) && m_scene->HasComponentById<TransformComponent>(id2))) 
			return std::optional<CollisionInfo>{};

		CollisionComponent& collision1 = m_scene->GetComponentById<CollisionComponent>(id1);
		CollisionComponent& collision2 = m_scene->GetComponentById<CollisionComponent>(id2);

		if (auto area1 = std::get_if<Collision_Box>(&collision1.GetAreaRef()))
		{
			//Rectangle rect1 = { area1->postition.x, area1->postition.y, area1->width, area1->height};
			if (auto area2 = std::get_if<Collision_Box>(&collision2.GetAreaRef()))
			{
				std::optional<CollisionInfo> collision1check = CheckCollisionBoxes(*area1, *area2);
				std::optional<CollisionInfo> collision2check = CheckCollisionBoxes(*area2, *area1);

				if (!(collision1check.has_value() && collision2check.has_value())) return std::optional<CollisionInfo>{};

				CollisionInfo collision1 = *collision1check;
				CollisionInfo collision2 = *collision2check;

				CollisionInfo result = abs(collision1.distance) < abs(collision2.distance) ? collision1 : collision2;

				// hack the contained flag to be the union of the two
				result.ownerContained = collision1.ownerContained && collision2.ownerContained;
				result.hitContained = collision1.hitContained && collision2.hitContained;

				return result;
			}
			if (auto area2 = std::get_if<Collision_Circle>(&collision2.GetAreaRef()))
			{
				std::optional<CollisionInfo> collisionCheck = CheckCollisionBoxCircle(*area1, *area2);

				if (!collisionCheck.has_value()) return std::optional<CollisionInfo>{};

				CollisionInfo result = *collisionCheck;

				return result;
			}
		}
		if (auto area1 = std::get_if<Collision_Circle>(&collision1.GetAreaRef()))
		{
			if (auto area2 = std::get_if<Collision_Box>(&collision2.GetAreaRef()))
			{
				//Rectangle rect2 = { area2->postition.x, area2->postition.y, area2->width, area2->height };
				//return CheckCollisionCircleRec(area1->postition, area1->radius, rect2);
				//return CheckCollisionBoxCircle(*area2, *area1);

				std::optional<CollisionInfo> collisionCheck = CheckCollisionBoxCircle(*area2, *area1);

				if (!collisionCheck.has_value()) return std::optional<CollisionInfo>{};

				CollisionInfo result = *collisionCheck;

				return result;
			}
			if (auto area2 = std::get_if<Collision_Circle>(&collision2.GetAreaRef()))
			{
				//return CheckCollisionCircles(area1->postition, area1->radius*area1->scale, area2->postition,
				//	area2->radius*area2->scale);
				std::optional<CollisionInfo> collisionCheck = CheckCollisionCircles(*area2, *area1);

				if (!collisionCheck.has_value()) return std::optional<CollisionInfo>{};

				CollisionInfo result = *collisionCheck;

				return result;

			}
		}

		return std::optional<CollisionInfo>{};

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

			collision.UpdateMembers(transform.m_position, transform.m_rotation, transform.m_scale);

		}
	
		SparseArray<SparseArray<std::pair<CollisionState, CollisionInfo>>> newMatrix;

		for (int i : m_scene->GetIds())
		{
			newMatrix.Emplace(i);

			for (int j : m_scene->GetIds())
			{
				if (i == j)
				{
					newMatrix[i].Emplace(j, NOT_COLLIDING, CollisionInfo{});
					continue;
				}
				CollisionState currentState = NOT_COLLIDING;
				if (m_collisionMatrix.HasId(i) && m_collisionMatrix[i].HasId(j))
					currentState = m_collisionMatrix[i][j].first;

				CollisionState previousState = currentState;
				//bool areColliding = AreColliding(i, j);
				bool areColliding = false;

				std::optional<CollisionInfo> collisionCheck = AreColliding(i, j);

				if (collisionCheck.has_value()) areColliding = true;

				CollisionInfo info = (areColliding) ? *collisionCheck : CollisionInfo{};

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
				newMatrix[i].Emplace(j, currentState, info);
				
				Entity& e = m_scene->GetEntity(i);
				if (!e.HasComponent<BehaviourComponent>()) continue;

				//BehaviourScript* behaviourScript = &*e.GetComponent<BehaviourComponent>().GetScript();
				
				CollisionEvents& collisionEvents = 
					*std::static_pointer_cast<CollisionEvents, BehaviourScript>
					(e.GetComponent<BehaviourComponent>().GetScript());
				if (currentState == ENTERING)
					collisionEvents.On_Enter(e, m_scene->GetEntity(j));
				if (currentState == STAYING)
					collisionEvents.On_Stay(e, m_scene->GetEntity(j));
				if (currentState == NOT_COLLIDING && previousState != NOT_COLLIDING)
					collisionEvents.On_Exit(e, m_scene->GetEntity(j));
				
			}
		}
		//m_collisionMatrix = std::move(newMatrix);
		m_collisionMatrix = newMatrix;
		
	}
	
	//helper method
	void PrintCurrent()
	{
		std::cout << std::endl << "Collision test" << std::endl;
		for (int i: m_scene->GetIds())
		{
			for (int j : m_scene->GetIds())
			{
				std::string outp;
				switch (m_collisionMatrix[i][j].first)
				{
				case NOT_COLLIDING:
					outp = "NOT_COLLIDING";
					break;
				case ENTERING:
					outp = "ENTERING";
					break;
				case STAYING:
					outp = "STAYING";
					break;
				}
				//std::cout << m_collisionMatrix[i][j] << " ";
				std::cout << outp << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl << "-----------" << std::endl;
	}

};


#endif // !ANIMATION_SYSTEM_HPP
