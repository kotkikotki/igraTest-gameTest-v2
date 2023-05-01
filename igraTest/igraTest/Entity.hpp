#ifndef ENTITY_HPP

#define ENTITY_HPP

#include "Components.h"
#include "Definitions.hpp"

class Scene;

class Entity
{
private:

    int m_id;
    Scene *m_owner_ptr = nullptr;

protected:

    

    Entity(int id, Scene& owner)
    {
        m_id = id;
        m_owner_ptr = &owner;
    }

    friend class Scene;

public:

    Entity() = delete;
    //Entity(const Entity& r) = delete;
    
    Entity(const Entity& r)
    {
        m_id = r.m_id;
        m_owner_ptr = r.m_owner_ptr;
    }
    
    ~Entity() = default;

    int GetId()
    {
        return m_id;
    }
    
    Scene& GetOwner()
    {
        return *m_owner_ptr;
    }

    template<typename T>
    T& AddComponent();
    
    template<typename T>
    T& GetComponent();
    
    template<typename T>
    void RemoveComponent();
    
    template<typename T>
    bool HasComponent();

};



#endif // !ENTITY_HPP

