#ifndef ANIMATION_COMPONENT_HPP

#define ANIMATION_COMPONENT_HPP

#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "Definitions.hpp"

class AnimationScript;

class AnimationComponent : public Component
{

	std::shared_ptr<AnimationScript> m_AnimationScript = nullptr;
	//req

public:

	using Component::Component;

	AnimationComponent(const std::shared_ptr<AnimationScript>& script) :
		m_AnimationScript(script) {}
	
	std::shared_ptr<AnimationScript>& GetScript()
	{
		return m_AnimationScript;
	}

	

};

//Scripts





#endif // !ANIMATION_COMPONENT_HPP

