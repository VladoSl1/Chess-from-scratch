#include "TextRenderer.h"

#include "Core/VertexBufferLayout.h"
using namespace graphics::core;

#include "Mathematics/mat4.h"

namespace graphics
{
	TextRenderer::TextRenderer(size_t w_width, size_t w_height, const char* fontPath, CharacterSets characterSetName)
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		InitShader();
		shader.Use();
		shader.SetOrthoProjection("transform", (float)w_width, (float)w_height);

		if (FT_Init_FreeType(&ft))
			throw std::logic_error("Failed to initialize FreeType Library");

		if (FT_New_Face(ft, fontPath, 0, &face))
			throw std::logic_error("ERROR: Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, defaultFontSize);
		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

		vao = VertexArray();
		vao.Use();

		vbo = VertexBuffer(sizeof(float) * 6 * 4);

		VertexBufferLayout layout;
		layout.Add<float>(4);

		vao.AddBuffer(vbo, layout);

		LoadCharacterSet(characterSetName);
	}

	void TextRenderer::DrawText(std::string text, float x, float y, unsigned int fontSize, math::vec3<> color)
	{
		float scale = (float)fontSize / defaultFontSize;

		shader.Use();
		shader.SetUniform3f("textColor", color);
		GLCall(glActiveTexture(GL_TEXTURE0));

		vao.Use();
		vbo.Use();

		for (char letter : text)
		{
			Character ch = characters[letter];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = -ch.Size.y * scale;

			float vertices[6][4] =
			{
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos, ypos, 0.0f, 1.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },

				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos + w, ypos + h, 1.0f, 0.0f}
			};

			vbo.BufferSubData(vertices, sizeof(vertices));

			GLCall(glBindTexture(GL_TEXTURE_2D, ch.ID));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

			x += (ch.Advance >> 6) * scale;
		}
	}
	
	bool TextRenderer::LoadCharacterSet(CharacterSets characterSetName)
	{
		characters = std::map<char, Character>();

		switch (characterSetName)
		{
		case CharacterSets::ASCII:
			for (unsigned char ch = 0; ch < 128; ch++)
			{
				if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					return false;
				}

				unsigned int texture;
				GLCall(glGenTextures(1, &texture));
				GLCall(glBindTexture(GL_TEXTURE_2D, texture));
				GLCall(glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				));

				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

				Character character = {
					texture,
					math::vec2<int>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					math::vec2<int>(face->glyph->bitmap_left, face->glyph->bitmap_top),
					(unsigned int)(face->glyph->advance.x)
				};

				characters.insert(std::pair<char, Character>(ch, character));
			}
			break;
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}

    void TextRenderer::InitShader()
    {
		std::string vertex_shader(
			"#version 330 core\n"
			"layout(location = 0) in vec4 vertex;\n"

			"out vec2 TexCoords;\n"

			"uniform mat4 transform;\n"

			"void main()\n"
			"{\n"
			"	gl_Position = transform * vec4(vertex.xy, 0.0, 1.0);\n"
			"	TexCoords = vertex.zw;\n"
			"}\n"
		);

		std::string fragment_shader(
			"#version 330 core\n"

			"in vec2 TexCoords;\n"
			"out vec4 o_Color;\n"

			"uniform sampler2D text;\n"
			"uniform vec3 textColor;\n"

			"void main()\n"
			"{\n"
			"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
			"	o_Color = vec4(textColor, 1.0) * sampled;\n"
			"}\n"
		);

		shader = Shader(vertex_shader, fragment_shader);
    }
}