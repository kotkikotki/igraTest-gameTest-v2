#ifndef DEFINITIONS_HPP

#define DEFINITIONS_HPP

#include<iostream>
#include<string>
#include<unordered_map>
#include "raylib.h"
#include<memory>
#include<any>
#include<variant>
#include<typeindex>
#include<utility>
#include<optional>
#include<limits>
#include<random>

#define SQRT2 1.41421356f
//#define G_CONST 9.78033f / 1000.f
#define G_CONST 0.00978033f
#define GRAVITY_CONST 0.0420f

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

int random(int min_in, int max_in)
{
	std::random_device generator;
	std::uniform_int_distribution distribution(min_in, max_in);
	return distribution(generator);
}

enum Collision_Area_Type
{
	COLLISION_BOX,
	COLLISION_CIRCLE
};

struct Collision_Base
{
	Vector2 position = {0.f, 0.f};
	float rotation = 0.f;
	float scale = 1.f;
	//
	Vector2 position_r = { 0.f, 0.f };
	float rotation_r = 0.f;
	float scale_r = 1.f;

	Collision_Base() = default;
	void UpdateArea(const Vector2& position = {0.f, 0.f}, float rotation = 0.f, float scale = 1.f)
	{
		this->position = position;
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
		this->position_r = position;
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
		this->position_r = position;
		this->rotation_r = rotation;
		this->scale_r = scale;
	}
	
};
float DotProduct(const Vector2& p1, const Vector2& p2)
{
	return (p1.x * p2.x) + (p1.y * p2.y);
}
float Determinant(const Vector2& p1, const Vector2& p2)
{
	return (p1.x * p2.y) - (p1.y * p2.x);
}
void Normalize(Vector2& vector, float magnitude = 1.f)
{
	float len = sqrtf(vector.x * vector.x + vector.y * vector.y);
	if (len == 0.f)
		return;

	float ratio = magnitude / len;
	vector.x *= ratio;
	vector.y *= ratio;
}
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

float GetAngleOfVectors(const Vector2& p1, const Vector2& p2)
{
	float dot = DotProduct(p1, p2);
	float determinant = Determinant(p1, p2);
	float angle =  atan2f(determinant, dot);
	angle = angle * 180.f / PI;
	return angle;
}

float GetAngleOfPoint(const Vector2& point)
{
	float angle = atan2f(point.y, point.x);
	angle = angle * 180.f / PI;
	return angle;
}
float GetAngleOfPoint_Vertical(const Vector2& point)
{
	float angle = atan2f(point.y, point.x);
	angle = angle * 180.f / PI;
	angle += 90.f;
	if (angle > 180.f)
		angle -= 360.f;

	return angle;
}


//

//
Vector2 GetPerpendicularAxis(const Vector2* points, int index, float magnitude = 1.f, int length = 4)
{
	Vector2 p1 = points[index];
	Vector2 p2 = (index >= length - 1) ? points[0] : points[index + 1];

	

	Vector2 axis = {
		-(p2.y - p1.y),
		p2.x - p1.x
	};

	Normalize(axis, magnitude);
	return axis;
}
std::pair<float, float> ProjectPointsMinMax(const Vector2& axis, const Vector2* points, int length = 4)
{
	
	float min = DotProduct(axis, points[0]);
	float max = min;

	for (int i = 1; i < length; i++)
	{
		float dot = DotProduct(axis, points[i]);
		if (dot < min) min = dot;
		if (dot > max) max = dot;
	}

	return { min, max };
}
std::pair<float, float> ProjectPointsMinMaxCircle(const Vector2& axis, const Vector2& center, float radius) {
	float projection = DotProduct(axis, center);
	return {
		projection - radius,
		projection + radius
	};
}
class Entity;
struct CollisionInfo
{
	//const Entity* owner = nullptr;
	//const Entity* hit = nullptr;
	float distance = 0.f;
	Vector2 unit_vector = { 0.f, 0.f };
	bool ownerContained = false;
	bool hitContained = false;
	Vector2 separation = { 0.f, 0.f };

	CollisionInfo() = default;
	CollisionInfo(const Entity& owner, const Entity& hit, float distance, const Vector2& unit_vector, bool ownerContained,
		bool hitContained, const Vector2& separation):
		//owner(&owner),
		//hit(&hit),
		distance(distance),
		unit_vector(unit_vector),
		ownerContained(ownerContained),
		hitContained(hitContained),
		separation(separation)
	{}

};
std::optional<CollisionInfo> CheckCollisionBoxes(const Collision_Box& box1, const Collision_Box& box2, float separationScale = 2.f)
{

	Vector2 box1points[4] = {
		GetRotatedPoint({box1.position.x - box1.width * box1.scale / 2.f, box1.position.y - box1.height * box1.scale / 2.f}, box1.position, box1.rotation),
		GetRotatedPoint({box1.position.x + box1.width * box1.scale / 2.f, box1.position.y - box1.height * box1.scale / 2.f}, box1.position, box1.rotation),
		GetRotatedPoint({box1.position.x + box1.width * box1.scale / 2.f, box1.position.y + box1.height * box1.scale / 2.f}, box1.position, box1.rotation),
		GetRotatedPoint({box1.position.x - box1.width * box1.scale / 2.f, box1.position.y + box1.height * box1.scale / 2.f}, box1.position, box1.rotation)
	};
	
	Vector2 box2points[4] = {
		GetRotatedPoint({box2.position.x - box2.width * box2.scale / 2.f, box2.position.y - box2.height * box2.scale / 2.f}, box2.position, box2.rotation),
		GetRotatedPoint({box2.position.x + box2.width * box2.scale / 2.f, box2.position.y - box2.height * box2.scale / 2.f}, box2.position, box2.rotation),
		GetRotatedPoint({box2.position.x + box2.width * box2.scale / 2.f, box2.position.y + box2.height * box2.scale / 2.f}, box2.position, box2.rotation),
		GetRotatedPoint({box2.position.x - box2.width * box2.scale / 2.f, box2.position.y + box2.height * box2.scale / 2.f}, box2.position, box2.rotation)
	};
	
	
	float shortestDistance = std::numeric_limits<float>::max();

	CollisionInfo collisionInfo;
	collisionInfo.hitContained = true;
	collisionInfo.ownerContained = true;


	for (int i = 0; i < 4; i++)
	{
		Vector2 axis = GetPerpendicularAxis(box1points, i, 1.f);
		
		std::pair<float, float> box1Range = ProjectPointsMinMax(axis, box1points);
		std::pair<float, float> box2Range = ProjectPointsMinMax(axis, box2points);



		
		if ((box1Range.first - box2Range.second > 0.f) || (box2Range.first - box1Range.second > 0.f))
		{
			//std::cout << "no" << std::endl;
			return std::optional<CollisionInfo>{};
		}

		

		if (box1Range.second > box2Range.second || box1Range.first < box2Range.first) collisionInfo.ownerContained = false;
		if (box2Range.second > box1Range.second || box2Range.first < box1Range.first) collisionInfo.hitContained = false;
		//
		float distanceMin = (box2Range.second - box1Range.first) * -1.f;

		float distanceMinAbs = abs(distanceMin);
		if (distanceMinAbs < shortestDistance)
		{
			shortestDistance = distanceMinAbs;

			collisionInfo.distance = distanceMin;
			collisionInfo.unit_vector = axis;
		}
	}
	
	collisionInfo.separation = { collisionInfo.unit_vector.x * collisionInfo.distance * separationScale,
		collisionInfo.unit_vector.y * collisionInfo.distance * separationScale};

	return collisionInfo;
}
std::optional<CollisionInfo> CheckCollisionBoxCircle(const Collision_Box& box, const Collision_Circle& circle, float separationScale = 2.f)
{
	Vector2 boxpoints[4] = {
		GetRotatedPoint({box.position.x - box.width * box.scale / 2.f, box.position.y - box.height * box.scale / 2.f}, box.position, box.rotation),
		GetRotatedPoint({box.position.x + box.width * box.scale / 2.f, box.position.y - box.height * box.scale / 2.f}, box.position, box.rotation),
		GetRotatedPoint({box.position.x + box.width * box.scale / 2.f, box.position.y + box.height * box.scale / 2.f}, box.position, box.rotation),
		GetRotatedPoint({box.position.x - box.width * box.scale / 2.f, box.position.y + box.height * box.scale / 2.f}, box.position, box.rotation)
	};



	float shortestDistance = std::numeric_limits<float>::max();

	CollisionInfo collisionInfo;
	collisionInfo.hitContained = true;
	collisionInfo.ownerContained = true;


	Vector2 closestPoint = {};
	for (Vector2 &point: boxpoints)
	{
		float distance = powf(circle.position.x - (box.position.x + point.x), 2.f) + powf(circle.position.y - (box.position.y + point.y), 2.f);
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			closestPoint.x = box.position.x + point.x;
			closestPoint.y = box.position.y + point.y;
		}
	}
	
	Vector2 axis = { closestPoint.x - circle.position.x, closestPoint.y - circle.position.y };
	Normalize(axis);

	std::pair<float, float> boxRange = ProjectPointsMinMax(axis, boxpoints);
	
	std::pair<float, float> circleRange = ProjectPointsMinMaxCircle(axis, circle.position, circle.radius*circle.scale);


	if ((boxRange.first - circleRange.second > 0.f) || (circleRange.first - boxRange.second > 0.f))
	{
		return std::optional<CollisionInfo>{};
	}




	float distanceMin = (circleRange.second - boxRange.first);

	shortestDistance = abs(distanceMin);

	collisionInfo.distance = distanceMin;
	collisionInfo.unit_vector = axis;

	if (boxRange.second > circleRange.second || boxRange.first < circleRange.first) collisionInfo.ownerContained = false;
	if (circleRange.second > boxRange.second || circleRange.first < boxRange.first) collisionInfo.hitContained = false;


	for (int i = 0; i < 4; i++)
	{
		
		Vector2 axis_l = GetPerpendicularAxis(boxpoints, i, 1.f);
		
		std::pair<float, float> boxRange_l = ProjectPointsMinMax(axis_l, boxpoints);
		std::pair<float, float> circleRange_l = ProjectPointsMinMaxCircle(axis_l, circle.position, circle.radius*circle.scale);

		if ((boxRange_l.first - circleRange_l.second > 0.f) || (circleRange_l.first - boxRange_l.second > 0.f))
		{
			return std::optional<CollisionInfo>{};
		}

		if (boxRange_l.second > circleRange_l.second || boxRange_l.first < circleRange_l.first) collisionInfo.ownerContained = false;
		if (circleRange_l.second > boxRange_l.second || circleRange_l.first < boxRange_l.first) collisionInfo.hitContained = false;

		distanceMin = (circleRange_l.second - boxRange_l.first)*-1.f;

		float distanceMinAbs_l = abs(distanceMin);
		if (distanceMinAbs_l < shortestDistance)
		{
			shortestDistance = distanceMinAbs_l;

			collisionInfo.distance = distanceMin;
			collisionInfo.unit_vector = axis_l;
		}
	}

	collisionInfo.separation = { collisionInfo.unit_vector.x * collisionInfo.distance * separationScale,
		collisionInfo.unit_vector.y * collisionInfo.distance * separationScale };


	return collisionInfo;


}
std::optional<CollisionInfo> CheckCollisionCircles(const Collision_Circle& circle1, const Collision_Circle& circle2, float separationScale = 2.f)
{

	float radiusTotal = circle1.radius * circle1.scale + circle2.radius * circle2.scale;
	float distanceBetween = sqrtf(powf(circle2.position.x - circle1.position.x, 2.f) + powf(circle2.position.y - circle1.position.y, 2.f));

	if (distanceBetween > radiusTotal)
		return std::optional<CollisionInfo>{};

	CollisionInfo collisionInfo;

	collisionInfo.unit_vector = { circle2.position.x - circle1.position.x, circle2.position.y - circle1.position.y };
	Normalize(collisionInfo.unit_vector); 

	collisionInfo.distance = distanceBetween;

	float difference = radiusTotal - distanceBetween;
	collisionInfo.separation = { collisionInfo.unit_vector.x * difference * separationScale, 
		collisionInfo.unit_vector.y * difference  * separationScale};

	float radiusA = circle1.radius * circle1.scale;
	float radiusB = circle2.radius * circle2.scale;
	collisionInfo.ownerContained = radiusA <= radiusB && distanceBetween <= radiusB - radiusA;
	collisionInfo.hitContained = radiusB <= radiusA && distanceBetween <= radiusA - radiusB;

	return collisionInfo;
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

	std::unordered_map<std::string, std::shared_ptr<std::any>> pairs;

	bool HasVariable(const std::string& name)
	{
		return pairs.find(name) != pairs.end();
	}

	template<typename T>
	void ChangeVariablePtrByName(const std::string& name, const std::shared_ptr<T>& s_ptr)
	{
		pairs[name] = s_ptr;
	}
	
	template<typename T>
	void ChangeVariableByName(const std::string& name, const T& value)
	{
		(*pairs[name]) = value;
	}
	
	void AddVariable(const std::string& name, const std::shared_ptr<std::any> &s_ptr)
	{
		//pairs.insert(std::make_pair(name, s_ptr));
		pairs.emplace(name, s_ptr);
	}
	void RemoveVariable(const std::string& name)
	{
		pairs.erase(name);
	}
	const std::shared_ptr<std::any>& GetVariablePtr(const std::string& name)
	{
		return pairs[name];
	}
	
	template<typename T>
	const T& GetVariable(const std::string& name)
	{
		return std::any_cast<T>(*pairs[name]);
	}
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
		if (HasId(id)) return;
		//
		vector.emplace_back(element);
		keys.emplace(std::make_pair(id, vector.size() - 1));
	}
	
	template<typename ...Args>
	void Emplace(int id, Args&& ...args)
	{
		if (HasId(id)) return;
		//
		vector.emplace_back(args...);
		keys.emplace(std::make_pair(id, vector.size() - 1));
	}
	void Remove(int id)
	{
		if (!HasId(id)) return;

		vector.erase(vector.begin()+keys[id]);

		for (auto& pair : keys)
		{
			if (pair.second > keys[id])
				pair.second--;
		}

		keys.erase(id);
		/*
		std::cout << "Aft" << std::endl;

		for (auto& pair : keys)
		{
			std::cout<<pair.first<<" "<<pair.second<< std::endl;
		}
		*/
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
				return (IsKeyDown(k)) || (IsMouseButtonDown(k));
				break;
			case Pressed:
				return (IsKeyPressed(k)) || (IsMouseButtonPressed(k));
				break;
			case Released:
				return (IsKeyReleased(k)) || (IsMouseButtonReleased(k));
				break;
			case Up:
				return (IsKeyUp(k)) || (IsMouseButtonUp(k));
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
<AnimationComponent, BehaviourComponent, CollisionComponent, InputComponent, PhysicsComponent,
SpriteComponent, TransformComponent>;

enum ComponentType
{
	ANIMATION = 0,
	BEHAVIOUR,
	COLLISION,
	INPUT,
	PHYSICS,
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
	{typeid(PhysicsComponent), PHYSICS},
	{typeid(SpriteComponent), SPRITE},
	{typeid(TransformComponent), TRANSFORM}
};


//

#endif // !DEFINITIONS_HPP

