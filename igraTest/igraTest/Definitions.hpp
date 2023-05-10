#ifndef DEFINITIONS_HPP

#define DEFINITIONS_HPP

#include<string>

#include<unordered_map>
#include "raylib.h"
#include<memory>
#include<any>
#include<variant>
#include<typeindex>
#include<utility>

#define SQRT2 1.41421356f

template<typename T> 
T max(T a, T b)
{
	return (a >= b) ? a : b;
}
template<typename T>
T min(T a, T b)
{
	return (a <= b) ? a : b;
}

struct Circle
{
	Vector2 point = {};
	float radius = 0.f;
	Circle(const Vector2& point, float radius)
	{
		this->point = point;
		this->radius = radius;
	}
};

//
struct Collision_Rectangle
{
	float width = 0.f;
	float height = 0.f;
	Collision_Rectangle(float width, float height)
	{
		this->width = width;
		this->height = height;
	}
};

struct  Collision_Circle
{
	float radius = 0.f;
	Collision_Circle(float radius)
	{
		this->radius = radius;
	}
};
//

struct TextureFilePath_ScrollingSpeed_Tuple
{
	std::string filePath = "";
	float scrollingSpeed = 0.f;
};

struct Texture_ScrollingSpeed_Tuple
{
	Texture2D texture = { 0 };
	float scrollingSpeed = 0.f;
};

struct Name_Variable_Tuple_Map
{
public:

	std::unordered_map<std::string, std::any> pairs;

	bool HasVariable(const std::string& name)
	{
		return pairs.find(name) != pairs.end();
	}
	void ChangeVariableByName(const std::string &name, const std::any &value)
	{
		pairs[name] = value;
	}
	void AddVariable(const std::string &name, const std::any  &value)
	{
		pairs.insert(std::make_pair(name, value));
	}
	void RemoveVariable(const std::string &name)
	{
		pairs.erase(name);
	}
	const std::any& GetVariable(const std::string& name)
	{
		return pairs[name];
	}
};

struct Name_LinkedVariable_Tuple_Map
{
public:

	std::unordered_map<std::string, std::shared_ptr<void>> pairs;

	bool HasVariable(const std::string& name)
	{
		return pairs.find(name) != pairs.end();
	}

	template<typename T>
	void ChangeVariableByName(const std::string& name, const std::shared_ptr<void>& s_ptr)
	{
		pairs[name] = s_ptr;
	}

	void AddVariable(const std::string& name, const std::shared_ptr<void> &s_ptr)
	{
		pairs.insert(std::make_pair(name, s_ptr));
	}
	void RemoveVariable(const std::string& name)
	{
		pairs.erase(name);
	}
	const std::shared_ptr<void>& GetVariablePtr(const std::string& name)
	{
		return pairs[name];
	}
	
	/*
	template<typename T>
	const T& GetVariable(const std::string& name)
	{
		return std::any_cast<T>(*std::static_pointer_cast<std::any>(pairs[name]));
	}*/
};

template<typename T>
struct SparseArray
{
private:
	std::vector<T> vector;
	std::unordered_map<int, int> keys;
public:
	void Insert(int id, const T& element)
	{
		vector.emplace_back(element);
		keys.emplace(std::make_pair(id, vector.size() - 1));
	}
	void Remove(int id)
	{
		vector.erase(keys[id]);
	}
	T& operator[](int id)
	{
		return vector[keys[id]];
	}
	const std::vector<T> &GetVector()
	{
		return vector;
	}
	
};

enum KeyCondition
{
	Down,
	Pressed,
	Released,
	Up
};

struct Input_Action_Tuple_Map
{
public:

	

	bool GetKeyCondition(int k, KeyCondition c)
	{
		switch (c)
		{
			case Down:
				return (IsKeyDown(k));
				break;
			case Pressed:
				return (IsKeyPressed(k));
				break;
			case Released:
				return (IsKeyReleased(k));
				break;
			case Up:
				return (IsKeyUp(k));
				break;
		}
	}

	std::unordered_map<std::string, std::pair<int, KeyCondition>> pairs;

	bool HasAction(const std::string& action)
	{
		return pairs.find(action) != pairs.end();
	}
	void ChangeActuibByName(const std::string& action, int key, KeyCondition condition)
	{
		pairs[action] = std::make_pair(key, condition);
	}
	void AddAction(const std::string& action, int key, KeyCondition condition)
	{
		pairs.insert(std::make_pair(action, std::make_pair(key, condition)));
	}
	void RemoveAction(const std::string& action)
	{
		pairs.erase(action);
	}
	const std::pair<int, KeyCondition>& GetAction(const std::string& action)
	{
		return pairs[action];
	}

	bool GetActionState(const std::string& action)
	{
		return GetKeyCondition(pairs[action].first, pairs[action].second);
	}

};

//c++ 17 standard
template< class Base, class Derived >
inline constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B, T>::type;

//
#include "Components.h"
using component_var_t = std::variant<AnimationComponent, InputComponent, SpriteComponent, TransformComponent>;

enum ComponentType
{
	ANIMATION,
	COLLISION,
	INPUT,
	SPRITE,
	TRANSFORM,

	END
};

std::unordered_map<std::type_index, ComponentType> componentIndexes
{
	{typeid(AnimationComponent), ANIMATION},
	{typeid(AnimationComponent), COLLISION},
	{typeid(InputComponent), INPUT},
	{typeid(SpriteComponent), SPRITE},
	{typeid(TransformComponent), TRANSFORM}
};

template<typename T, typename ...Args>
T Create(Args&& ...args)
{
	return T(std::forward<Args>(args)...);
}
//

#endif // !DEFINITIONS_HPP

