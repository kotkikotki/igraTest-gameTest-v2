#ifndef ENTITY_HPP

#define ENTITY_HPP

#include<unordered_set>
#include "Components.h"
#include "Definitions.hpp"

class Scene;

class Entity
{
private:

    int m_id;
    Scene* m_owner_ptr = nullptr;

protected:



    Entity(int id, Scene& owner)
    {
        m_id = id;
        m_owner_ptr = &owner;
    }

    friend class Scene;

public:

    std::unordered_set<std::string> m_tags;

    Entity() = delete;
    //Entity(const Entity& r) = delete;

    Entity(const Entity& r)
    {
        m_id = r.m_id;
        m_owner_ptr = r.m_owner_ptr;
        m_tags = r.m_tags;
    }

    ~Entity() = default;

    int GetId()
    {
        return this->m_id;
    }

    Scene& GetOwner()
    {
        return *this->m_owner_ptr;
    }

    bool HasTag(const std::string& tag)
    {
        return m_tags.find(tag) != m_tags.end();
    }

    template<typename T, typename ...Args>
    T& AddComponent(Args&& ...args);
    
    template<typename T>
    T& GetComponent();
    
    template<typename T>
    void RemoveComponent();
    
    template<typename T>
    bool HasComponent();

};



#endif // !ENTITY_HPP

