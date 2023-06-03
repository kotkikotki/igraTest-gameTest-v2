#ifndef SCRIPTS_HPP

#define SCRIPTS_HPP

#include<functional>
#include "Definitions.hpp"
#include "Entity.hpp"


class Script
{
public:

	Name_Variable_Tuple_Map m_Properties;
	Name_LinkedVariable_Tuple_Map m_LinkedProperties;

	explicit Script() : Script({}, {})
	{
	}
	explicit Script(const std::initializer_list < std::pair<std::string, std::any>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto &pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto &pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}


	virtual void UpdateProperties() {}

	virtual void AddProperties(const std::initializer_list < std::pair<std::string, std::string>>& properties, const std::initializer_list < std::pair<std::string, std::shared_ptr<std::any>>>& linkedProperties)
	{
		for (auto &pair : properties)
		{
			m_Properties.AddVariable(pair.first, pair.second);
		}
		for (auto &pair : linkedProperties)
		{
			m_LinkedProperties.AddVariable(pair.first, pair.second);
		}

	}

protected:
	Script(const Script& r) = default;
	Script(Script&& r) = default;
	Script& operator=(const Script& r) = default;
	Script& operator=(Script&& r) = default;
};

//Behaviour
class CollisionEvents
{

public:

	virtual void On_Enter(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
	{
	}
	virtual void On_Stay(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
	{
	}
	virtual void On_Exit(Entity& owner, Entity& hit, const CollisionInfo& collisionInfo)
	{
	}
};

class BehaviourScript : public Script, public CollisionEvents
{
	

protected:
	
	std::unordered_map<std::string, std::function<void(Entity& entity)>> m_actions;

public:

	static inline std::unordered_set<std::string> m_tags;

	using Script::Script;

	BehaviourScript() : Script() , CollisionEvents()
	{
	}

	virtual void On_Update(Entity& owner)
	{}

	virtual void On_Action(Entity& entity, const std::string& action)
	{
		if (m_actions.find(action) == m_actions.end()) return;
		m_actions[action](entity);
	}
};

// Animation
class AnimationScript : public Script
{

public:

	using Script::Script;

	//virtual void Animate(SpriteComponent& sprite) {}
	virtual void Animate(SpriteComponentAdvanced& sprites) {}
};

//Input

class InputScript : public Script
{

public:

	using Script::Script;

	virtual void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity) {}

};






#endif // !SCRIPTS_HPP

