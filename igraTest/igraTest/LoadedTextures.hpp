#ifndef LOADED_TEXTURES_HPP

#define LOADED_TEXTURES_HPP

#include"Definitions.hpp"

namespace LoadedTextures
{

	static std::map<std::string, Texture2D> loadedTextures;

	static Texture2D GetTexture(const std::string& name)
	{
		return loadedTextures[name];
	}

};

#endif // !LOADED_TEXTURES_HPP
