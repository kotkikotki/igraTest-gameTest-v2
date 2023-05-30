#ifndef SCENE_HPP

#define SCENE_HPP


#include<vector>
#include<queue>
#include<set>
#include<stdexcept>
#include<cassert>

#include "Definitions.hpp"

#include "Entity.hpp"

class Scene
{

private:

	//
	static inline std::unordered_set<int> unloadedTextures;
	//

	SparseArray<std::shared_ptr<Entity>> m_entities;
	std::vector<SparseArray<std::shared_ptr<component_var_t>>> m_components;
	std::priority_queue<int, std::vector<int>, std::greater<int>> m_free_Ids;
	std::set<int> m_used_Ids;

	//
	std::queue<int> m_removeQueue;
	//
	



protected:

	template<class T, typename = enable_if_t<is_base_of_v<Component, T>>, typename ...Args>
	T& AddComponent(int id, Args&&... args)
	{
		ComponentType componentType = componentIndexes[typeid(T)];
		const int& component_id = componentType;

		m_components[component_id].Emplace(id, std::move(std::make_shared<component_var_t>(Create<T>(args...))));
		//m_components[component_id].Emplace(id, args...));

		std::cout << "added comp" << typeid(T).name();

		return std::get<T>(*m_components[component_id][id]);
	}


	template<class T, typename = enable_if_t<is_base_of_v<Component, T>>>
	T& GetComponent(int id)
	{
		const int& component_id = componentIndexes[typeid(T)];
		return std::get<T>(*m_components[component_id][id]);
	}

	template<class T, typename = enable_if_t<is_base_of_v<Component, T>>>
	std::shared_ptr<component_var_t> GetComponentPtr(int id)
	{
		const int& component_id = componentIndexes[typeid(T)];
		if (m_components[component_id].size() == 0 || !m_components[component_id].HasId(id)) return nullptr;
		return m_components[component_id][id];
	}

	template<class T, typename = enable_if_t<is_base_of_v<Component, T>>>
	void RemoveComponent(int id)
	{
		const int& component_id = componentIndexes[typeid(T)];

		m_components[component_id].Remove(id);
	}

	std::shared_ptr<Entity>& GetEntityPtr(int id)
	{
		if (!HasEntityById(id))throw std::invalid_argument("Entity does not exist.");
		return m_entities[id];

	}

	friend class Entity;

public:

	Scene()
	{
		m_components = std::vector <SparseArray<std::shared_ptr<component_var_t>>>(END);
	}

	~Scene()
	{
		//std::unordered_set<int> unloadedTextures;
		//texture_unload
		for (auto& comp : m_components[componentIndexes[typeid(SpriteComponent)]].GetVector())
		{
			Texture& texture = std::get<SpriteComponent>(*comp).m_texture;

			if(unloadedTextures.find(texture.id) == unloadedTextures.end())
			{
				unloadedTextures.emplace(texture.id);
				UnloadTexture(texture);
			}
		}
		for (auto& comp : m_components[componentIndexes[typeid(SpriteComponentAdvanced)]].GetVector())
		{
			SpriteComponentAdvanced& sprites = std::get<SpriteComponentAdvanced>(*comp);
			for(auto &a: sprites.m_layeredSprites)
			{
				Texture& texture = a.second.second.m_texture;

				if (unloadedTextures.find(texture.id) == unloadedTextures.end())
				{
					unloadedTextures.emplace(texture.id);
					UnloadTexture(texture);
				}
			}
		}
		for (int i : unloadedTextures)
		{
			std::cout << i << std::endl;
		}
	}

	void On_Update()
	{
		while (!m_removeQueue.empty())
		{
			int id = m_removeQueue.front();
			m_removeQueue.pop();

			RemoveEntity(id);
		}
	}

	Entity& AddEntity()
	{
		int id;
		if (m_free_Ids.empty())
		{
			id = m_entities.size();
		}
		else
		{
			id = m_free_Ids.top();
			m_free_Ids.pop();
		}

		m_entities.Emplace(id, std::move(std::make_shared<Entity>(Entity(id, *this))));

		m_used_Ids.emplace(id);

		std::cout << std::endl << "Added" << id << std::endl;
		return *m_entities[id];

	}


	bool HasEntityById(int id)
	{
		return m_entities.HasId(id);
	}

	int GetEntityCount()
	{
		return m_used_Ids.size();
	}

	const std::set<int>& GetIds()
	{
		return m_used_Ids;
	}

	Entity& GetEntity(int id)
	{
		if (!HasEntityById(id))throw std::invalid_argument("Entity does not exist.");
		return *m_entities[id];

	}
	//before or after update loop
	void RemoveEntity(int id)
	{

		if (!HasEntityById(id))throw std::invalid_argument("Entity does not exist.");
		//m_entities.erase(m_entities.begin() + id);
		
		for (auto& pair : componentIndexes)
		{
			if(m_components[pair.second].HasId(id))
				m_components[pair.second].Remove(id);

		}
		m_entities.Remove(id);
		m_free_Ids.push(id);
		m_used_Ids.erase(id);

		std::cout <<std::endl<< "Removed" << id << std::endl;
	}
	//
	void QueueForRemove(int id)
	{
		m_removeQueue.push(id);
	}

	const std::vector<std::shared_ptr<Entity>>& GetVector()
	{
		return m_entities.GetVector();
	}

	//

	const std::vector<std::shared_ptr<component_var_t>>& GetComponentsOfType(ComponentType componentType)
	{
		return m_components[componentType].GetVector();
	}
	template<typename T>
	T& GetComponentById(int id)
	{
		return GetComponent<T>(id);
	}
	template<typename T>
	bool HasComponentById(int id)
	{
		return !(GetComponentPtr<T>(id)==nullptr);
	}

	//test
	/*
	void PrintAll()
	{
		for (auto& a : m_components)
		{
			for (auto& b : a.GetVector())
			{
				std::cout <<(*b).index() << std::endl;
			}
		}
	}
	*/
};
void Entity::Destroy()
{
	this->GetOwner().QueueForRemove(this->GetId());
}
template<typename T, typename ...Args>
T& Entity::AddComponent(Args&& ...args)
{
	return this->GetOwner().AddComponent<T>(this->GetId(), args...);
}
template<typename T>
T& Entity::GetComponent()
{
	return this->GetOwner().GetComponent<T>(this->GetId());
}
template<typename T>
void Entity::RemoveComponent()
{
	this->GetOwner().RemoveComponent(this->GetId());
}
template<typename T>
bool Entity::HasComponent()
{
	return this->GetOwner().HasComponentById<T>(this->GetId());
}

#endif // !SCENE_HPP

