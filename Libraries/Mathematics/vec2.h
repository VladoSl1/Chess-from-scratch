#pragma once

#include "Mathematics.h"


namespace math
{
	template<typename Type>
	struct vec2
	{
		Type x;
		Type y;

		vec2() : x(0), y(0) { }
		vec2(Type n) : x(n), y(n) { }
		vec2(Type x, Type y) : x(x), y(y) { }
		vec2(const vec2<Type>& src) : x(src.x), y(src.y) { }

		vec2<Type>& operator=(const vec2<Type>& src) {
			x = src.x;
			y = src.y;
			return *this;
		}

		inline const Type& operator[](size_t i) const {
			return (i ? x : y);
		}

		inline Type& operator[](size_t i) {
			return const_cast<Type&>(static_cast<const vec2&>(*this)[i]);
		}

		template<typename NewType>
		inline operator vec2<NewType>() const {
			return vec2<NewType>(static_cast<NewType>(x), static_cast<NewType>(y));
		}

		template<typename NewType>
		explicit inline operator vec3<NewType>() const {
			return vec3<NewType>(static_cast<NewType>(x), static_cast<NewType>(y), (NewType)0);
		}

		template<typename NewType>
		explicit inline operator vec4<NewType>() const {
			return vec4<NewType>(static_cast<NewType>(x), static_cast<NewType>(y), (NewType)0, (NewType)0);
		}

		inline vec2 abs() const {
			return { math::abs(x), math::abs(y) };
		}

		inline vec2& operator+=(const vec2& v) {
			x += v.x;
			y += v.y;
			return *this;
		}

		inline vec2& operator-=(const vec2& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		}

		inline vec2 operator+(const vec2& v) const {
			return { x + v.x, y + v.y };
		}

		inline vec2 operator+(Type s) const {
			return { x + s, y + s };
		}

		inline vec2 operator-(const vec2& v) const {
			return { x - v.x, y - v.y };
		}

		inline vec2 operator-(Type s) const {
			return { x - s, y - s };
		}

		inline Type operator*(const vec2& v) const {
			return x * v.x + y * v.y;
		}

		inline vec2 operator*(Type s) const {
			return { x * s, y * s };
		}

		inline vec2 operator/(Type s) const {
			return { x / s, y / s };
		}

		inline friend std::ostream& operator<<(std::ostream& os, const vec2& v) {
			os << "[" << v.x << " " << v.y << "]";
			return os;
		}
	};
}
