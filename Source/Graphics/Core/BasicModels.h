#pragma once

#include "Mathematics/mat4.h"
#include "Mathematics/vec2.h"
#include "Mathematics/vec3.h"


namespace graphics::core::models
{
	math::mat4<> CreateModel(math::vec2<> position, math::vec2<> size, float rotation, bool center = true);
}