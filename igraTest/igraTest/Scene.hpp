#ifndef SCENE_HPP

#define SCENE_HPP


#include <vector>
#include<stdexcept>
#include<cassert>

#include "Definitions.hpp"

#include "Entity.hpp"

class Scene
{
	
private:

	std::vector<std::shared_ptr<Entity>> m_entities;
	std::vector<SparseArray<std::shared_ptr<component_var_t>>> m_components;

protected:

	template<class T, typename = enable_if_t<is_base_of_v<Component, T>>, typename ...Args>
	T& AddComponent(int id, Args&&... args)
	{
		ComponentType componentType = componentIndexes[typeid(T)];
		const int& component_id = componentType;

		component_var_t component = Create<T>(args... );;

		m_components[component_id].Insert(id, std::make_shared<component_var_t>(component));

		std::cout << "added comp"<<typeid(T).name();

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
		return m_entities[id];

	}

	friend class Entity;

public:

	Scene()
	{
		for (int i = 0; i < ComponentType::END; i++)
		{
			m_components.push_back(SparseArray<std::shared_ptr<component_var_t>>());
		}
	}

	Entity& AddEntity()
	{
		//std::shared_ptr<Entity> entityPtr = std::make_shared<Entity>(Entity(m_entities.size(), *this));
		m_entities.push_back(std::make_shared<Entity>(Entity(m_entities.size(), *this)));
		return **(m_entities.end()-1);
		
	}

	bool HasEntityById(int id)
	{
		//static_assert(is_base_of_v<Component, T>);
		return !(GetEntityPtr(id) == nullptr);
	}

	int GetIdCount()
	{
		return m_entities.size();
	}

	Entity& GetEntity(int id)
	{
		if (!HasEntityById(id))throw std::invalid_argument("Entity does not exist.");
		return *m_entities[id];

	}

	void RemoveEntity(int id)
	{
		m_entities.erase(m_entities.begin() + id);

	}

	const std::vector<std::shared_ptr<Entity>>& GetVector()
	{
		return m_entities;
	}

	//

	const std::vector<std::shared_ptr<component_var_t>>& GetComponentsOfType(ComponentType componentType)
	{
		return m_components[componentType].GetVector();
	}
	template<typename T>
	T& GetComponentById(int id)
	{
		//static_assert(is_base_of_v<Component, T>);
		return GetComponent<T>(id);
	}
	template<typename T>
	bool HasComponentById(int id)
	{
		//static_assert(is_base_of_v<Component, T>);
		return !(GetComponentPtr<T>(id)==nullptr);
	}
};

template<typename T, typename ...Args>
T& Entity::AddComponent(Args&& ...args)
{
	//static_assert(is_base_of_v<Component, T>);
	return this->GetOwner().AddComponent<T>(this->GetId(), args...);
}
template<typename T>
T& Entity::GetComponent()
{
	//static_assert(is_base_of_v<Component, T>);
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

