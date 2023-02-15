#include "GameObject.h"

#include "SpriteRenderer.h"

namespace graphics
{
	GameObject::GameObject() //{ }
		: position(0.0f, 0.0f), size(1.0f, 1.0f), color(1.0f), rotation(0.0f), midpoint(false) { }

	GameObject::GameObject(math::vec2<> pos, math::vec2<> size, Texture2D& sprite, math::vec3<> color, bool midpoint, float rotation)
		: position(pos), size(size), texture(sprite), color(color), rotation(rotation), midpoint(midpoint) { }

	GameObject::GameObject(const GameObject& src)
		: position(src.position), size(src.size), texture(src.texture), color(src.color),
		  rotation(src.rotation), midpoint(src.midpoint) { }

	GameObject& GameObject::operator=(const GameObject& src)
	{
		position	= src.position;
		size		= src.size;
		texture		= src.texture;
		color		= src.color;
		rotation	= src.rotation;
		midpoint	= src.midpoint;
		return *this;
	}

	void GameObject::Draw(SpriteRenderer* renderer)
	{
		if (midpoint)
			renderer->DrawSprite(texture, position - size / 2.0f, size, rotation, color);
		else
			renderer->DrawSprite(texture, position, size, rotation, color);
	}

	bool GameObject::IsIn(math::vec2<> point) const
	{
		math::vec2<> startPoint = midpoint ? position - size / 2.0f : position;
		math::vec2<> endPoint = startPoint + size;

		return point.x >= startPoint.x && point.x <= endPoint.x &&
			   point.y >= startPoint.y && point.y <= endPoint.y;
	}
}
