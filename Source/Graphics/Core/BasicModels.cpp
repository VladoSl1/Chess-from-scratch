#include "BasicModels.h"


namespace graphics::core::models
{
	math::mat4<> CreateModel(math::vec2<> position, math::vec2<> size, float rotation, bool center)
	{
		math::mat4 model = math::IdentityMatrix<float>();

		// Move middle point of model to position 
		model = math::Translate(model, position);

		// Rotate
		if (rotation)
			model = math::Rotate(model, math::Radians(rotation), math::vec3(0.0f, 0.0f, 1.0f));

		// Move middle to its right position
		if (center)
			model = math::Translate(model, { size.x / 2, size.y / 2 });

		// Scale to given size
		model = math::Scale(model, math::vec3(size.x, size.y, 1.0f));

		return model;
	}
}

