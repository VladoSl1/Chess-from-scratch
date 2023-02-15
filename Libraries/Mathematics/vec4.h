#pragma once

#include "Mathematics.h"


namespace math
{
	template<typename Type>
	struct vec4
	{
		Type x;
		Type y;
		Type z;
		Type w;

		vec4() : x(0), y(0), z(0), w(0) { }
		vec4(Type n) : x(n), y(n), z(n), w(n) { }
		vec4(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) { }
		vec4(const Type* data) : x(data[0]), y(data[1]), z(data[2]), w(data[3]) { }
		vec4(const vec4<Type>& src) : x(src.x), y(src.y), z(src.z), w(src.w) { }

		vec4<Type>& operator=(const vec4<Type>& src) {
			x = src.x;
			y = src.y;
			z = src.z;
			w = src.w;
			return *this;
		}


		inline const Type& operator[](size_t i) const {
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: throw std::invalid_argument("Index Error");
			}
		}

		inline Type& operator[](size_t i) {
			return const_cast<Type&>(static_cast<const vec4&>(*this)[i]);
		}

		template<typename NewType>
		inline operator vec4<NewType>() const {
			return vec4<NewType>(static_cast<NewType>(x), static_cast<NewType>(y), static_cast<NewType>(z), static_cast<NewType>(w));
		}

		inline vec4 abs() const {
			return { math::abs(x), math::abs(y), math::abs(z), math::abs(w) };
		}

		inline vec4& operator+=(const vec4& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		inline vec4 operator+(const vec4& v) const {
			return { x + v.x, y + v.y, z + v.z, w + v.z };
		}

		inline vec4 operator+(Type s) const {
			return { x + s, y + s, z + s, w + s };
		}

		inline vec4 operator-(const vec4& v) const {
			return { x - v.x, y - v.y, z - v.z, w - v.z };
		}

		inline vec4 operator-(Type s) const {
			return { x - s, y - s, z - s, w - s };
		}

		inline Type operator*(const vec4& v) const {
			return x * v.x + y * v.y + z * v.z, w * v.z;
		}

		inline vec4 operator*(Type s) const {
			return { x * s, y * s, z * s, w * s };
		}

		inline friend std::ostream& operator<<(std::ostream& os, const vec4& v) {
			os << "[" << v.x << " " << v.y << " " << v.z << " " << v.w << "]";
			return os;
		}
	};
};