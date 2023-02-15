#pragma once

#include "Core/BaseRenderer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/BasicModels.h"

#include "Mathematics/vec2.h"
#include "Mathematics/vec3.h"

namespace graphics
{
	class LineRenderer : core::BaseRenderer
	{
	public:
		LineRenderer(size_t width, size_t height)
		{
			InitShader(width, height);
			InitData();
		}

		void DrawLine(math::vec2<> start, math::vec2<> end, math::vec3<> color = math::vec3(0.0f))
		{
			math::mat4 model = core::models::CreateModel(start, end - start, 0.0f, false);

			shader.Use();
			shader.SetUniformMatrix4f("model", model);
			shader.SetUniform3f("lineColor", color);

			vao.Use();

			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			GLCall(glDrawArrays(GL_LINES, 0, 2));
		}

	private:
		void InitData()
		{
			float verticies[4]{
				0.0f, 0.0f,
				1.0f, 1.0f
			};

			vbo = core::VertexBuffer(verticies, 4 * sizeof(float));

			core::VertexBufferLayout layout;
			layout.Add<float>(2);

			vao.AddBuffer(vbo, layout);
		}

		void InitShader(size_t width, size_t height)
		{
			std::string vertex_shader(
				"#version 330 core\n"
				"layout(location = 0) in vec2 aPos;\n"

				"uniform mat4 model;\n"
				"uniform mat4 projection;\n"

				"void main()\n"
				"{\n"
				"	gl_Position = projection * model * vec4(aPos, 0.0f, 1.0);"
				"}\n"
			);

			std::string fragment_shader(
				"#version 330 core\n"

				"out vec4 o_Color;\n"

				"uniform vec3 lineColor;\n"

				"void main()\n"
				"{\n"
				"	o_Color = vec4(lineColor, 1.0f);\n"
				"}\n"
			);

			shader = core::Shader(vertex_shader, fragment_shader);
			shader.SetOrthoProjection("projection", (float)width, (float)height);
		}
	};
}

