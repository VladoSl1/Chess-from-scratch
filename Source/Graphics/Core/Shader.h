#pragma once

#include "Base.h"

#include "Mathematics/vec2.h"
#include "Mathematics/vec3.h"
#include "Mathematics/vec4.h"
#include "Mathematics/mat4.h"

#include <string>

namespace graphics
{
	namespace core
	{
		class Shader
		{
		public:
			Shader() : _ID(0) { }
			Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
			Shader(std::string& vertexShader, std::string& fragmentShader);

			/* Activation of shader */
			void Use();
			void Unbind();
			void Delete();

			/* Send Orthographic projection matrix to shader */
			void SetOrthoProjection(const std::string& name, float width, float height);

			// Uniform utility functions
			void SetUniform2f(const std::string& name, const math::vec2<>& value);
			void SetUniform3f(const std::string& name, const math::vec3<>& value);
			void SetUniform4f(const std::string& name, const math::vec4<>& value);
			void SetUniformMatrix4f(const std::string& name, const math::mat4<>& data);

			inline unsigned int ID() const { return _ID; }

		private:
			/* Get location of uniform with given name in shader */
			int GetUniformLocation(const std::string& name);

			std::string LoadShaderFromFile(const std::string& path) const;
			unsigned int CompileShader(unsigned int type, std::string& source) const;
			unsigned int CreateShader(std::string& vertexShader, std::string& fragmenShader) const;

			unsigned int _ID;
		};
	}

}