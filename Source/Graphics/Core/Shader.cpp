#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace graphics
{
	namespace core
	{
		Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
			: _ID(0)
		{
			std::string vertexShader = LoadShaderFromFile(vertexShaderPath);
			std::string fragmentShader = LoadShaderFromFile(fragmentShaderPath);

			_ID = CreateShader(vertexShader, fragmentShader);
		}

		Shader::Shader(std::string& vertexShader, std::string& fragmentShader)
			: _ID(0)
		{
			_ID = CreateShader(vertexShader, fragmentShader);
		}

		void Shader::Use() {
			GLCall(glUseProgram(_ID));
		}

		void Shader::Unbind() {
			GLCall(glUseProgram(0));
		}

		void Shader::Delete() {
			if (_ID)
				GLCall(glDeleteProgram(_ID));
			_ID = 0;
		}

		void Shader::SetOrthoProjection(const std::string& name, float width, float height) {
			Use();
			math::mat4 projection = math::Ortho<float>(0.0f, width, height, 0.0f, -1.0f, 1.0f);
			SetUniformMatrix4f(name, projection);
		}

		void Shader::SetUniform2f(const std::string& name, const math::vec2<>& value) {
			GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
		}

		void Shader::SetUniform3f(const std::string& name, const math::vec3<>& value) {
			GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
		}

		void Shader::SetUniform4f(const std::string& name, const math::vec4<>& value) {
			GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
		}

		void Shader::SetUniformMatrix4f(const std::string& name, const math::mat4<>& data) {
			GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &data[0][0]));
		}

		int Shader::GetUniformLocation(const std::string& name)
		{
			GLCall(int location = glGetUniformLocation(_ID, name.c_str()));
			if (location == -1)
				std::cout << "'" << name << "'" << " does not correspond to an active uniform variable\n";

			return location;
		}

		unsigned int Shader::CompileShader(unsigned int type, std::string& source) const
		{
			GLCall(unsigned int id = glCreateShader(type));
			const char* src = source.c_str();
			GLCall(glShaderSource(id, 1, &src, nullptr));
			GLCall(glCompileShader(id));

			int succes;
			GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &succes));
			if (!succes)
			{
				int length;
				GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
				char* message = new char[length];
				GLCall(glGetShaderInfoLog(id, length, &length, message));

				std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader\n\n";
				std::cout << message << std::endl;
				GLCall(glDeleteShader(id));

				return 0;
			}

			return id;
		}

		unsigned int Shader::CreateShader(std::string& vertexShader, std::string& fragmentShader) const
		{
			GLCall(unsigned int program = glCreateProgram());
			unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
			unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

			GLCall(glAttachShader(program, vs));
			GLCall(glAttachShader(program, fs));
			GLCall(glLinkProgram(program));
			GLCall(glValidateProgram(program));

			GLCall(glDeleteShader(vs));
			GLCall(glDeleteShader(fs));

			return program;
		}

		std::string Shader::LoadShaderFromFile(const std::string& path) const
		{
			std::ifstream file(path);
			std::stringstream buffer;

			if (!file.is_open())
				std::cout << "Failed to load shader from: " << path << std::endl;

			buffer << file.rdbuf();
			file.close();

			return buffer.str();
		}
	}
}