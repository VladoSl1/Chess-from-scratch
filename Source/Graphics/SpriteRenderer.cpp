#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Core/VertexBufferLayout.h"

#include "Core/BasicModels.h"

using namespace graphics::core;

namespace graphics
{
	SpriteRenderer::SpriteRenderer(size_t width, size_t height, bool instance_option)
	{
		InitShader(instance_option);
		shader.SetOrthoProjection("projection", (float)width, (float)height);
		InitData(instance_option);
	}

	SpriteRenderer::~SpriteRenderer()
	{
		ibo.Delete();
		instance_vbo.Delete();
	}
	
	void SpriteRenderer::DrawSprite(Texture2D& texture, math::vec2<> position,
		math::vec2<> size, float rotation, math::vec3<> color)
	{
		math::mat4 model = models::CreateModel(position, size, rotation);

		shader.Use();
		shader.SetUniformMatrix4f("model", model);
		shader.SetUniform3f("modelColor", color);

		if (texture.ID())
			texture.Activate();

		vao.Use();
		vbo.Use();
		ibo.Use();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.Count(), GL_UNSIGNED_INT, 0));
	}

	void SpriteRenderer::CreateQueue(math::mat4<> model, Texture2D& texture, uint* queueID, math::vec3<> color) {
		queues.push_back(GraphicQueue({ texture, color, model }));
		*queueID = queues.size();
	}

	void SpriteRenderer::CreateQueue(GameObject& baseObject, uint* queueID) {
		math::mat4<> model = models::CreateModel(baseObject.position, baseObject.size, baseObject.rotation);
		queues.push_back(GraphicQueue({ baseObject.texture, baseObject.color, model }));

		*queueID = queues.size();
	}

	void SpriteRenderer::DrawQueue(uint queueID)
	{
		GraphicQueue queue = queues[queueID - 1];

		const uint numberOfFullQueues = (uint)floor(queue.size() / SubDataPackSize);

		shader.Use();
		shader.SetUniformMatrix4f("model", queue.model);
		shader.SetUniform3f("modelColor", queue.color);
		
		vao.Use();
		ibo.Use();
		instance_vbo.Use();

		if (queue.texture.ID())
			queue.texture.Activate();

		for (uint i = 0; i < numberOfFullQueues; ++i) {
			instance_vbo.BufferSubData(&queue.offsets[0] + i * SubDataPackSize, SubDataPackSize * sizeof(float) * 2);
			GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.Count(), GL_UNSIGNED_INT, 0, SubDataPackSize));
		}

		instance_vbo.BufferSubData(&queue.offsets[0] + numberOfFullQueues * SubDataPackSize,
			(queue.size() % SubDataPackSize) * sizeof(float) * 2);
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.Count(), GL_UNSIGNED_INT, 0, queue.size() % SubDataPackSize));
	}

	void SpriteRenderer::InitData(bool instance_option)
	{
		float vertices[] = {
			// coordinates			// texture coords
			-0.5f, 0.5f,			0.0f, 1.0f,				// left  upper  
			-0.5f, -0.5f,			0.0f, 0.0f,				// left  bottom 
			0.5f, -0.5f,			1.0f, 0.0f,				// right bottom 
			0.5f, 0.5f,				1.0f, 1.0f,				// right upper  
		};

		uint indexes[] = {
			0, 1, 2,
			0, 3, 2
		};

		vbo = VertexBuffer(vertices, sizeof(vertices));
		ibo = IndexBuffer(indexes, 6);

		VertexBufferLayout layout;
		layout.Add<float>(2);
		layout.Add<float>(2);

		vao.AddBuffer(vbo, layout);
		
		if (instance_option)
		{
			instance_vbo = VertexBuffer(SubDataPackSize * sizeof(float) * 2);

			VertexBufferLayout instance_layout;
			instance_layout.Add<float>(2, 1);

			vao.AddBuffer(instance_vbo, instance_layout);
		}
	}

	void SpriteRenderer::InitShader(bool instance_option)
	{
		std::string vertex_shader;
		std::string fragment_shader;

		if (!instance_option)
		{
			vertex_shader =
				"#version 330 core\n"
				"layout(location = 0) in vec2 aPos;\n"
				"layout(location = 1) in vec2 aTexCoord;\n"

				"out vec2 TexCoords;\n"

				"uniform mat4 model;\n"
				"uniform mat4 projection;\n"

				"void main()\n"
				"{\n"
					"TexCoords = aTexCoord;\n"
					"gl_Position = projection * model * vec4(aPos, 0.0f, 1.0);\n"
				"}\n";

			fragment_shader =
				"#version 330 core\n"

				"out vec4 o_Color;\n"

				"in vec2 TexCoords;\n"

				"uniform sampler2D texture1;\n"
				"uniform vec3 modelColor;\n"

				"void main()\n"
				"{\n"
					"vec4 texColor = vec4(modelColor, 1.0) * texture(texture1, TexCoords);\n"
					"if (texColor.a < 0.1)\n"
						"discard;\n"
					"o_Color = texColor;\n"
				"}\n";
		}

		else
		{
			vertex_shader =
				"#version 330 core\n"
				"layout(location = 0) in vec2 aPos;\n"
				"layout(location = 1) in vec2 aTexCoord;\n"
				"layout(location = 2) in vec2 aOffset;\n"

				"uniform mat4 model;\n"
				"uniform mat4 projection;\n"

				"void main()\n"
				"{\n"
					"gl_Position = projection * model * vec4(aPos + aOffset, 0.0f, 1.0);\n"
				"}\n";

			fragment_shader =
				"#version 330 core\n"

				"out vec4 o_Color;\n"

				"uniform vec3 modelColor;\n"

				"void main()\n"
				"{\n"
					"o_Color = vec4(modelColor, 1.0);\n"
				"}\n";
		}

		shader = Shader(vertex_shader, fragment_shader);
	}
}