#pragma once

#include "Mathematics.h"
#include "vec4.h"

namespace math
{
	template<typename Type>
	struct mat4
	{
		mat4() { _data = new vec4<Type>[4]; }
		mat4(const mat4& m) {
			_data = new vec4<Type>[4];
			memcpy(_data, m.data(), 4 * 4 * sizeof(Type));
		}
		mat4(const Type* data)
			: mat4()
		{
			_data[0] = vec4<Type>(data);
			_data[1] = vec4<Type>(data + 4);
			_data[2] = vec4<Type>(data + 8);
			_data[3] = vec4<Type>(data + 12);
		}

		~mat4() {
			delete[] _data;
		}

		inline const vec4<Type>& operator[](size_t i) const { return _data[i]; }
		inline vec4<Type>& operator[](size_t i) {
			return const_cast<vec4<Type>&>(static_cast<const mat4&>(*this)[i]);
		}

		mat4& operator=(const mat4& m) {
			delete[] _data;

			_data = new vec4<Type>[4];
			memcpy(_data, m.data(), 4 * 4 * sizeof(Type));
			return *this;
		}

		inline const vec4<Type>* data() const { return _data; }

		friend std::ostream& operator<<(std::ostream& os, const mat4& m) {
			os << "[[" << m[0][0] << ' ' << m[0][1] << ' ' << m[0][2] << ' ' << m[0][3] << ']'
				<< "\n [" << m[1][0] << ' ' << m[1][1] << ' ' << m[1][2] << ' ' << m[1][3] << ']'
				<< "\n [" << m[2][0] << ' ' << m[2][1] << ' ' << m[2][2] << ' ' << m[2][3] << ']'
				<< "\n [" << m[3][0] << ' ' << m[3][1] << ' ' << m[3][2] << ' ' << m[3][3] << "]]";

			return os;
		}

	private:
		vec4<Type>* _data;
	};

	template<typename Type = float>
	static mat4<Type> IdentityMatrix()
	{
		float _data[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		return mat4<Type>(&_data[0]);
	}

	template<typename Type>
	static mat4<Type> Translate(const mat4<Type>& matrix, const vec3<Type>& translationVector)
	{
		mat4<Type> result = matrix;

		result[0][3] += translationVector.x;
		result[1][3] += translationVector.y;
		result[2][3] += translationVector.z;

		return result;
	}

	template<typename Type>
	inline static mat4<Type> Translate(const mat4<Type>& matrix, const vec2<Type>& translationVector) {
		return Translate(matrix, math::vec3(translationVector.x, translationVector.y, 0.0f));
	}

	template<typename Type>
	static mat4<Type> Scale(const mat4<Type>& matrix, const vec3<Type>& translationVector)
	{
		mat4 result = matrix;

		for (unsigned char row = 0; row < 4; row++)
			for (unsigned char col = 0; col < 3; col++)
				result[row][col] *= translationVector[col];

		return result;
	}

	template<typename Type>
	static mat4<Type> Ortho(float left, float right, float bottom, float top)
	{
		mat4<Type> result = IdentityMatrix<Type>();

		result[0][0] = static_cast<Type>(2) / (right - left);
		result[1][1] = static_cast<Type>(2) / (top - bottom);

		result[0][3] = static_cast<Type>(-1) * (right + left) / (right - left);
		result[1][3] = static_cast<Type>(-1) * (top + bottom) / (top - bottom);

		return result;
	}

	template<typename Type>
	static mat4<Type> Ortho(float left, float right, float bottom, float top, float far, float near)
	{
		mat4<Type> result = IdentityMatrix<Type>();

		result[0][0] = static_cast<Type>(2) / (right - left);
		result[1][1] = static_cast<Type>(2) / (top - bottom);
		result[2][2] = static_cast<Type>(-2) / (far - near);

		result[0][3] = static_cast<Type>(-1) * (right + left) / (right - left);
		result[1][3] = static_cast<Type>(-1) * (top + bottom) / (top - bottom);
		result[2][3] = static_cast<Type>(-1) * (far + near) / (far - near);

		return result;
	}

	template<typename Type>
	static mat4<Type> Rotate(const mat4<Type>& matrix, float angle, const vec3<Type>& rotation_axis)
	{
		mat4 result = matrix;

		double _cos = cos(angle);
		double _sin = sin(angle);

		result[0][0] = static_cast<Type>(_cos + pow(rotation_axis.x, 2) * (1 - _cos));
		result[0][1] = static_cast<Type>(rotation_axis.x * rotation_axis.y * (1 - _cos) - rotation_axis.z * _sin);
		result[0][2] = static_cast<Type>(rotation_axis.x * rotation_axis.z * (1 - _cos) + rotation_axis.y * _sin);

		result[1][0] = static_cast<Type>(rotation_axis.x * rotation_axis.y * (1 - _cos) + rotation_axis.z * _sin);
		result[1][1] = static_cast<Type>(_cos + pow(rotation_axis.y, 2) * (1 - _cos));
		result[1][2] = static_cast<Type>(rotation_axis.y * rotation_axis.z * (1 - _cos) - rotation_axis.y * _sin);

		result[2][0] = static_cast<Type>(rotation_axis.x * rotation_axis.z * (1 - _cos) - rotation_axis.y * _sin);
		result[2][1] = static_cast<Type>(rotation_axis.y * rotation_axis.z * (1 - _cos) + rotation_axis.y * _sin);
		result[2][2] = static_cast<Type>(_cos + pow(rotation_axis.z, 2) * (1 - _cos));

		return result;
	}
}