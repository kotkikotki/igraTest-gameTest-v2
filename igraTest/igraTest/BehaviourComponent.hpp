#ifndef BEHAVIOUR_COMPONENT_HPP

#define BEHAVIOUR_COMPONENT_HPP

//#include "Components.h"
#include "Definitions.hpp"

class BehaviourScript;


class BehaviourComponent : public Component
{

	std::shared_ptr<BehaviourScript> m_BehaviourScript = nullptr;


public:

	using Component::Component;

	BehaviourComponent(const std::shared_ptr<BehaviourScript>& script) :
		m_BehaviourScript(script) {}


	std::shared_ptr<BehaviourScript>& GetScript()
	{
		return m_BehaviourScript;
	}
};




#endif // !BEHAVIOUR_COMPONENT_HPP

