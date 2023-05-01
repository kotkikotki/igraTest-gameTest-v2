#ifndef SYSTEM_HPP

#define SYSTEM_HPP

#include "Definitions.hpp"

template<typename Derived>
class System 
{
protected:
	Scene* m_scene;

public:
	explicit System(Scene& scene)
	{
		m_scene = &scene;
	}
	void On_Update()
	{
		static_cast<Derived*>(this)->On_Update();
	}


};


#endif // !SYSTEM_HPP

