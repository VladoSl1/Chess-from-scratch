#pragma once

#include "Mathematics.h"


namespace math
{
	template<typename Type>
	struct vec3
	{
		Type x;
		Type y;
		Type z;

		vec3() : x(0), y(0), z(0) { }
		vec3(Type n) : x(n), y(n), z(n) { }
		vec3(Type x, Type y, Type z) : x(x), y(y), z(z) { }
		vec3(const vec3<Type>& src) : x(src.x), y(src.y), z(src.z) { }

		vec3<Type>& operator=(const vec3<Type>& src) {
			x = src.x;
			y = src.y;
			z = src.z;
			return *this;
		}

		inline const Type& operator[](size_t i) const {
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw std::invalid_argument("Index Error");
			}
		}

		inline Type& operator[](size_t i) {
			return const_cast<Type&>(static_cast<const vec3&>(*this)[i]);
		}

		template<typename NewType>
		inline operator vec3<NewType>() const {
			return vec3<NewType>(static_cast<NewType>(x), static_cast<NewType>(y), static_cast<NewType>(z));
		}

		template<typename NewType>
		explicit inline operator vec4<NewType>() const {
			return vec4<NewType>(static_cast<NewType>(x), static_cast<NewType>(y), static_cast<NewType>(z), (NewType)0);
		}

		inline vec3 abs() const {
			return { math::abs(x), math::abs(y), math::abs(z) };
		}

		inline vec3& operator+=(const vec3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		inline vec3 operator+(const vec3& v) const {
			return { x + v.x, y + v.y, z + v.z };
		}

		inline vec3 operator+(Type s) const {
			return { x + s, y + s, z + s };
		}

		inline vec3 operator-(const vec3& v) const {
			return { x - v.x, y - v.y, z - v.z };
		}

		inline vec3 operator-(Type s) const {
			return { x - s, y - s, z - s};
		}

		inline Type operator*(const vec3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		inline vec3 operator*(Type s) const {
			return { x * s, y * s, z * s };
		}

		inline vec3 operator/(Type s) const {
			return { x / s, y / s, z / s };
		}

		inline friend std::ostream& operator<<(std::ostream& os, const vec3& v) {
			os << "[" << v.x << " " << v.y << " " << v.z << "]";
			return os;
		}
	};
}
