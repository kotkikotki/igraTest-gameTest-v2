#ifndef COMPONENT_HPP

#define COMPONENT_HPP


class Component
{

    
public:

    //virtual ~Component() = default;
    //Component(const Component& r) = delete;
    bool m_enabled = true;
    Component() = default;
    virtual ~Component() = default;


protected:
    
    Component(const Component& r) = default;
    Component(Component&& r) = default;
    Component& operator=(const Component& r) = default;
    Component& operator=(Component&& r) = default;

    //Component() = default;
    
};

class TestComponent : Component
{
public:
    
};

#endif // !COMPONENT_HPP

