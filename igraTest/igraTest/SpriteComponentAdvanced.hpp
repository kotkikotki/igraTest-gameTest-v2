#ifndef SPRITE_COMPONENT_ADVANCED_HPP

#define SPRITE_COMPONENT_ADVANCED_HPP

#include "Component.hpp"
#include<map>

struct Sprite
{
	Texture2D m_texture = { 0 };

	int m_frameCountX = 1; //static
	int m_frameCountY = 1;

	float m_textureScale = 1.f;

	Rectangle m_currentFrameRectangle = {};

	unsigned int m_layer = 0; //0 -> min

	float m_relative_rotation = 0.f;
	Vector2 m_relative_position = { 0.f, 0.f };

	Sprite(const Texture2D& texture, int frameCountX, int frameCountY, float textureScale, unsigned int layer = 0,
		float relative_rotation = 0.f, const Vector2& relative_position = { 0.f, 0.f }) :
		m_texture(texture),
		m_frameCountX(frameCountX),
		m_frameCountY(frameCountY),
		m_textureScale(textureScale),
		m_layer(layer),
		m_relative_rotation(relative_rotation),
		m_relative_position(relative_position)
	{
		m_currentFrameRectangle = { 0.f, 0.f, (float)m_texture.width / (float)m_frameCountX, (float)m_texture.height / (float)m_frameCountY };
	}
};

class SpriteComponent : public Component
{

public:
	
	std::multimap<int, std::pair<std::string, Sprite>> m_layeredSprites;

	unsigned int m_layer = 0; //0 -> min


	using Component::Component;

	SpriteComponent(const Sprite& base, const std::string& name = "base", unsigned int layer = 0) :
		m_layer(layer)
	{
		m_layeredSprites.emplace(std::make_pair(base.m_layer, std::make_pair(name,base)));

	}
	
	bool HasSprite(const std::string& name)
	{

		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first)
				return true;
		}

		return false;
	}
	bool HasSprite(const std::string& name, int layer)
	{
		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first && a.first == layer)
				return true;
		}
		return false;
	}
	Sprite& GetSprite(const std::string &name)
	{
		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first)
				return spritePair.second;
		}
		//
		if (name == "base") return m_layeredSprites.begin()->second.second;
		//
		throw std::invalid_argument("Invalid sprite name.");
	}
	Sprite& GetSprite(const std::string& name, int layer)
	{
		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first && a.first == layer)
				return spritePair.second;
		}
		//
		if (name == "base") return m_layeredSprites.begin()->second.second;
		//
		throw std::invalid_argument("Invalid sprite name or layer.");

	}

	void AddSprite(const Sprite& sprite, const std::string& name = "base", unsigned int layer = 0)
	{
		m_layeredSprites.emplace(std::make_pair(sprite.m_layer, std::make_pair(name, sprite)));
	}

	void RemoveSprite(const std::string& name)
	{
		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first)
				m_layeredSprites.erase(a.first);
		}
	}
	void RemoveSprite(const std::string& name, int layer)
	{
		for (auto& a : m_layeredSprites)
		{
			auto& spritePair = a.second;
			if (name == spritePair.first && a.first == layer)
				m_layeredSprites.erase(a.first);
		}
	}

};



#endif // !SPRITE_COMPONENT_HPP

