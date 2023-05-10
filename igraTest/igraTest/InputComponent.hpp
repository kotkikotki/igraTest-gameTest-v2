#ifndef INPUT_COMPONENT_HPP

#define INPUT_COMPONENT_HPP

//#include "Components.h"
#include "Definitions.hpp"

class InputScript;

class InputMappings
{
	
	
public:

	Input_Action_Tuple_Map m_Map;

	InputMappings(const std::initializer_list<std::pair<std::string, std::pair<int, KeyCondition>>>& actions)
	{
		for (auto pair : actions)
		{
			m_Map.AddAction(pair.first, pair.second.first, pair.second.second);
		}
	}
};

class InputComponent : public Component
{

	std::shared_ptr<InputScript> m_InputScript = nullptr;
	std::shared_ptr<InputMappings> m_InputMappings = std::make_shared<InputMappings>(InputMappings({}));
	

public:

	using Component::Component;

	InputComponent(const std::shared_ptr<InputScript>& script, const std::shared_ptr<InputMappings>& mappings): 
		m_InputScript(script),
		m_InputMappings(mappings) {}
	

	std::shared_ptr<InputScript>& GetScript()
	{
		return m_InputScript;
	}
	std::shared_ptr<InputMappings>& GetMappings()
	{
		return m_InputMappings;
	}
};




#endif // !INPUT_COMPONENT_HPP

