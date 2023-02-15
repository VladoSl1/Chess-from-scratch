#pragma once

#include "Core/Shader.h"

#include "Texture2D.h"

#include <map>
#include <string>

namespace graphics
{
	class ResourceManager
	{
	public:
		static core::Shader& LoadShader(const char* vertexShaderPath, const char* fragmentShaderPath, std::string name);
		static core::Shader& GetShader(std::string name);

		static Texture2D& LoadTexture(const char* texturePath, std::string name);
		static Texture2D& GetTexture(std::string name);

		static void Clean();

	private:
		ResourceManager() = default;

		static std::map<std::string, core::Shader>			shaders;
		static std::map<std::string, Texture2D>		textures;
	};
}

