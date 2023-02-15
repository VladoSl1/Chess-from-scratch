#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Core/BaseRenderer.h"

#include "Mathematics/vec2.h"

#include <map>
#include <string>


namespace graphics
{
	struct Character
	{
		unsigned int ID;
		math::vec2<int> Size;
		math::vec2<int> Bearing;
		unsigned int Advance;
	};

	enum class CharacterSets
	{
		ASCII
	};

	class TextRenderer : core::BaseRenderer
	{
		const unsigned int defaultFontSize = 48U;

	public:
		TextRenderer(size_t w_width, size_t w_height, const char* fontPath,
					 CharacterSets characterSetName = CharacterSets::ASCII);

		void DrawText(std::string text, float x, float y, unsigned int fontSize = 48U, math::vec3<> color = math::vec3(1.0f));
		bool LoadCharacterSet(CharacterSets characterSetName);

		inline float TextLength(std::string text, unsigned int fontSize) const
		{
			float scale = (float)fontSize / defaultFontSize;
			float length = 0;

			for (char letter : text)
			{
				Character ch = characters.at(letter);
				length += ch.Size.x * scale;
			}

			return length;
		}
		
	private:
		std::map<char, Character> characters;
		FT_Library ft;
		FT_Face face;

		void InitShader();
	};
}


