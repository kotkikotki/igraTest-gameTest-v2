#ifndef SCRIPTS_HPP

#define SCRIPTS_HPP

#include<functional>
#include<queue>
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


	using Script::Script;

	BehaviourScript() : Script() , CollisionEvents()
	{
	}
	BehaviourScript(Entity& e) : Script(), CollisionEvents()
	{
	}

	virtual void On_Update(Entity& owner)
	{}

	virtual void On_Draw(Entity& owner)
	{

	}

	virtual void On_Action(Entity& entity, const std::string& action) final
	{
		if (m_actions.find(action) == m_actions.end()) return;
		m_actions[action](entity);
	}

};

struct Sequence
{
private:
	float durationToNextFrame = 0.f;
	float duration = 0.f;
	float durationLeft = 0.f;
	

public:

	Sprite* sprite;
	int currentFrame = 0;

	Sequence(Sprite& sprite, float duration)
	{
		this->sprite = &sprite;
		this->duration = duration;

		durationLeft = duration;
		durationToNextFrame = duration / (sprite.m_frameCount.size() * sprite.m_framesOnRow);
		
	}
	float GetDurationLeft()
	{
		return durationLeft;
	}
	void Update(float frameTime)
	{
		durationLeft -= frameTime;
		durationToNextFrame -= frameTime;
		if (durationToNextFrame <= 0.f)
		{
			currentFrame++;
			durationToNextFrame = duration / (sprite->m_frameCount.size() * sprite->m_framesOnRow);

		}
		int frameCountY = sprite->m_frameCount.size();
		int frameCountX = sprite->m_framesOnRow;

		int currentFrameY = currentFrame / frameCountX,
			currentFrameX = currentFrame % frameCountX;
		sprite->m_currentFrameRectangle.x = (float)(currentFrameX) * (float)sprite->m_texture.width / (float)(frameCountX);
		sprite->m_currentFrameRectangle.y = (float)(currentFrameY) * (float)sprite->m_texture.height / (float)(frameCountY);

	}
};

// Animation
class AnimationScript : public Script
{
private:

	std::vector<Sequence> m_sequences;

public:

	using Script::Script;

	virtual void Animate(SpriteComponent& sprites) {}
	virtual void UpdateSequences()
	{
		float frameTime = GetFrameTime();
		for (int i = 0; i<m_sequences.size(); i++)
		{
			auto& sequence = m_sequences[i];
			if (sequence.GetDurationLeft() <= 0.f)
			{
				m_sequences.erase(m_sequences.begin() + i);
				i--;
				continue;
			}
			sequence.Update(frameTime);
		}
	}
	

	virtual void AddSequence(Sprite& sprite, float duration)
	{
		m_sequences.emplace_back(sprite, duration);
	}
	
	virtual const std::vector<Sequence>& GetSequences()
	{
		return m_sequences;
	}
	
	
};

//Input

class InputScript : public Script
{

public:

	using Script::Script;

	virtual void ProcessInput(const std::shared_ptr<InputMappings>& mappings, Entity& entity) {}

};






#endif // !SCRIPTS_HPP

