#include "ResourceManager.h"
using namespace graphics::core;

#include "ErrorHandler.h"

namespace graphics
{
	std::map<std::string, Shader>       ResourceManager::shaders;
	std::map<std::string, Texture2D>    ResourceManager::textures;

	Shader& ResourceManager::LoadShader(const char* vertexShaderPath, const char* fragmentShaderPath, std::string name) {
		ASSERT(&(shaders[name]));
		shaders[name] = Shader(vertexShaderPath, fragmentShaderPath);
		return shaders[name];
	}

	Shader& ResourceManager::GetShader(std::string name) {
		return shaders[name];
	}

	Texture2D& ResourceManager::LoadTexture(const char* texturePath, std::string name) {
		ASSERT(&(textures[name]));
		textures[name] = Texture2D(texturePath);
		return textures[name];
	}

	Texture2D& ResourceManager::GetTexture(std::string name) {
		return textures[name];
	}

	void ResourceManager::Clean()
	{
		for (auto &shader : shaders)
			shader.second.Delete();

		for (auto& texture : textures)
			texture.second.Delete();
	}
}

