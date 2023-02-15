#pragma once

#include "Texture2D.h"

#include "Mathematics/vec2.h"
#include "Mathematics/vec3.h"


namespace graphics
{
	class SpriteRenderer;

	class GameObject
	{
	public:
		math::vec2<> position, size;
		math::vec3<> color;
		float rotation;
		bool midpoint;

		Texture2D texture;

		GameObject();
		GameObject(math::vec2<> pos, math::vec2<> size, Texture2D& texture,
				   math::vec3<> color = math::vec3<>(1.0f), bool midpoint = false, float rotation = 0.0f);
		GameObject(const GameObject& src);

		GameObject& operator=(const GameObject& src);

		virtual void Draw(SpriteRenderer* renderer);

		bool IsIn(math::vec2<> point) const;
	};
}

