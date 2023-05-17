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

template<typename T, typename ...Args>
T Create(Args&& ...args)
{
	return T(std::forward<Args>(args)...);
}

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
/*
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
*/
//

enum Collision_Area_Type
{
	COLLISION_BOX,
	COLLISION_CIRCLE
};

struct Collision_Base
{
	Vector2 postition = {0.f, 0.f};
	float rotation = 0.f;
	float scale = 1.f;
	//
	Vector2 postition_r = { 0.f, 0.f };
	float rotation_r = 0.f;
	float scale_r = 1.f;

	Collision_Base() = default;
	void UpdateArea(const Vector2& position = {0.f, 0.f}, float rotation = 0.f, float scale = 1.f)
	{
		this->postition = position;
		this->rotation = rotation;
		this->scale = scale;
	}
};

struct Collision_Box : Collision_Base
{
	using Collision_Base::Collision_Base;

	float width = 0.f;
	float height = 0.f;

	Collision_Box(float width, float height, const Vector2& position = { 0.f, 0.f }, float rotation = 0.f, float scale = 1.f)
	{
		this->width = width;
		this->height = height;
		this->postition_r = position;
		this->rotation_r = rotation;
		this->scale_r = scale;
	}
	
	
};

struct  Collision_Circle : Collision_Base
{
	using Collision_Base::Collision_Base;

	float radius = 0.f;

	Collision_Circle(float radius, const Vector2& position = { 0.f, 0.f }, float rotation = 0.f, float scale = 1.f)
	{
		this->radius = radius;
		this->postition_r = position;
		this->rotation_r = rotation;
		this->scale_r = scale;
	}
	
};
//
Vector2 GetRotatedPoint(const Vector2& point, const Vector2& centerOfRotation, float rotation)
{

	//
	int angle = rotation;
	angle %= 360;
	rotation = (float)(angle * 2) - rotation;
	//
	rotation = rotation * PI / 180.f;
	
	
	return { centerOfRotation.x + (point.x - centerOfRotation.x) * cosf(rotation) - (point.y - centerOfRotation.y) * sinf(rotation),
			centerOfRotation.y + (point.x - centerOfRotation.x) * sinf(rotation) + (point.y - centerOfRotation.y) * cosf(rotation) };
	
	

	
}
//
bool CheckCollisionBoxes(const Collision_Box& box1, const Collision_Box& box2)
{
	Vector2 box1points[4] = {
		GetRotatedPoint({box1.postition.x - box1.width * box1.scale / 2.f, box1.postition.y + box1.height * box1.scale / 2.f}, box1.postition, box1.rotation),
		GetRotatedPoint({box1.postition.x + box1.width * box1.scale / 2.f, box1.postition.y + box1.height * box1.scale / 2.f}, box1.postition, box1.rotation),
		GetRotatedPoint({box1.postition.x + box1.width * box1.scale / 2.f, box1.postition.y - box1.height * box1.scale / 2.f}, box1.postition, box1.rotation),
		GetRotatedPoint({box1.postition.x - box1.width * box1.scale / 2.f, box1.postition.y - box1.height * box1.scale / 2.f}, box1.postition, box1.rotation)
	};
	Vector2 box2points[4] = {
		GetRotatedPoint({box2.postition.x - box2.width * box2.scale / 2.f, box2.postition.y + box2.height * box2.scale / 2.f}, box2.postition, box2.rotation),
		GetRotatedPoint({box2.postition.x + box2.width * box2.scale / 2.f, box2.postition.y + box2.height * box2.scale / 2.f}, box2.postition, box2.rotation),
		GetRotatedPoint({box2.postition.x + box2.width * box2.scale / 2.f, box2.postition.y - box2.height * box2.scale / 2.f}, box2.postition, box2.rotation),
		GetRotatedPoint({box2.postition.x - box2.width * box2.scale / 2.f, box2.postition.y - box2.height * box2.scale / 2.f}, box2.postition, box2.rotation)
	};
	for (int i = 0; i<4; i++)
	{
		if(CheckCollisionPointPoly(box2points[i], box1points, 4))
			return true;
	}
	return false;
}
bool CheckCollisionBoxCircle(const Collision_Box& box, const Collision_Circle& circle)
{

	Vector2 boxpoints[4] = {
		GetRotatedPoint({box.postition.x - box.width * box.scale / 2.f, box.postition.y + box.height * box.scale / 2.f}, box.postition, box.rotation),
		GetRotatedPoint({box.postition.x + box.width * box.scale / 2.f, box.postition.y + box.height * box.scale / 2.f}, box.postition, box.rotation),
		GetRotatedPoint({box.postition.x + box.width * box.scale / 2.f, box.postition.y - box.height * box.scale / 2.f}, box.postition, box.rotation),
		GetRotatedPoint({box.postition.x - box.width * box.scale / 2.f, box.postition.y - box.height * box.scale / 2.f}, box.postition, box.rotation)
	};
	
	for (int i = 0; i < 4; i++)
	{
		BeginDrawing();
		DrawCircle(boxpoints[i].x, boxpoints[i].y, 5.f, GREEN);
		DrawCircle(box.postition.x, box.postition.y, 5.f, GREEN);
		EndDrawing();
	}
	
	for (int i = 0; i < 4; i++)
	{
		//!
		//if(CheckCollisionCir)
		if (CheckCollisionPointCircle(boxpoints[i], circle.postition, circle.radius * circle.scale))
			return true;
	}
	return false;
}

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
		//pairs.insert(std::make_pair(name, value));
		pairs.emplace(name, value);
	}
	void RemoveVariable(const std::string &name)
	{
		pairs.erase(name);
	}
	const std::any& GetVariable(const std::string& name)
	{
		return pairs[name];
	}
	template<typename T>
	const T& GetVariableT(const std::string& name)
	{
		return std::any_cast<T>(pairs[name]);
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
		//pairs.insert(std::make_pair(name, s_ptr));
		pairs.emplace(name, s_ptr);
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

	size_t size()
	{
		return vector.size();
	}

	bool HasId(int id)
	{
		return (keys.find(id) != keys.end());
	}
	void Insert(int id, const T& element)
	{
		vector.emplace_back(element);
		keys.emplace(std::make_pair(id, vector.size() - 1));
	}
	
	template<typename ...Args>
	void Emplace(int id, Args&& ...args)
	{
		vector.emplace_back(args...);
		keys.emplace(std::make_pair(id, vector.size() - 1));
	}
	void Remove(int id)
	{
		std::cout << "Bef" << std::endl;
		for (auto& pair : keys)
		{
			std::cout << pair.first << " " << pair.second << std::endl;
		}
		vector.erase(vector.begin()+keys[id]);

		for (auto& pair : keys)
		{
			if (pair.second > keys[id])
				pair.second--;
		}

		keys.erase(id);
		std::cout << "Aft" << std::endl;

		for (auto& pair : keys)
		{
			std::cout<<pair.first<<" "<<pair.second<< std::endl;
		}
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
		//pairs.insert(std::make_pair(action, std::make_pair(key, condition)));
		pairs.emplace(action, std::make_pair(key, condition));
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
using component_var_t = std::variant
<AnimationComponent, BehaviourComponent, CollisionComponent, InputComponent, SpriteComponent, TransformComponent>;

enum ComponentType
{
	ANIMATION = 0,
	BEHAVIOUR,
	COLLISION,
	INPUT,
	SPRITE,
	TRANSFORM,

	END
};

std::unordered_map<std::type_index, ComponentType> componentIndexes
{
	{typeid(AnimationComponent), ANIMATION},
	{typeid(BehaviourComponent), BEHAVIOUR},
	{typeid(CollisionComponent), COLLISION},
	{typeid(InputComponent), INPUT},
	{typeid(SpriteComponent), SPRITE},
	{typeid(TransformComponent), TRANSFORM}
};


//

#endif // !DEFINITIONS_HPP

