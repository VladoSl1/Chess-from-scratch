 #pragma once

#include <cmath>
#include <iostream>
#include <stdexcept>


namespace math
{
	static const double PI = 3.14159265359L;

	template<typename Type = float>
	struct vec2;

	template<typename Type = float>
	struct vec3;

	template<typename Type = float>
	struct vec4;

	template<typename Type = float>
	struct mat4;

	static inline float Radians(float degrees) {
		return (float)(degrees * (PI / 180));
	}

	static inline float Angle(float radians) {
		return (float)(radians * (180 / PI));
	}

	template<typename T1, typename T2>
	static inline T1 mod(T1 a, T2 b) {
		T1 res = a % b;
		return (res >= 0) ? res : (res + b);
	}

	template<typename T>
	static inline T abs(T x) {
		return x < 0 ? -x : x;
	}

	template<typename T1, typename T2>
	static inline T1 max(T1 x, T2 y) {
		return x > y ? x : y;
	}

	template<typename T1, typename T2>
	static inline T1 min(T1 x, T2 y) {
		return x < y ? x : y;
	}
}
