#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <numeric>
#include <random>
#include <type_traits>


#define ENABLE_IF_INT(in, out)		typename std::enable_if_t<std::is_integral_v<in>, out>
#define ENABLE_IF_FLOAT(in, out)	typename std::enable_if_t<std::is_floating_point_v<in>, out>

namespace random
{
	class RNG
	{
		using _rng = std::mt19937;

	public:

		RNG() : generator(createRandomGenerator()) { }
		
		/* Generates random number in interval <min, max) */
		template<typename Type>
		ENABLE_IF_FLOAT(Type, Type) randomNumber(Type min, Type max)
		{
			std::uniform_real_distribution<Type> distrib(min, max);
			return distrib(generator);
		}

		/* Generates random number in interval <min, max) */
		template<typename Type>
		ENABLE_IF_INT(Type, Type) randomNumber(Type min, Type max)
		{
			std::uniform_int_distribution<Type> distrib(min, max);
			return distrib(generator);
		}

		/* Shuffle contiguous list of numbers in interval <min, max) */
		template<typename Type>
		static std::vector<Type> shuffledSequence(Type _start, Type _end, Type step = static_cast<Type>(1))
		{
			size_t n = int((_end - _start) / step);
			std::vector<Type> result(n);

			// fill vector
			Type actual_val = _start;
			for (typename std::vector<Type>::iterator x = result.begin(); x != result.end(); ++x)
			{
				*x = actual_val;
				actual_val += step;
			}
			
			// shuffle vector
			std::shuffle(result.begin(), result.end(), generator);

			return result;
		}

		/* Shuffles given list */
		template<typename Type>
		inline void shuffle(std::vector<Type>& list) {
			std::shuffle(list.begin(), list.end(), generator);
		}

		template<typename Type>
		ENABLE_IF_FLOAT(Type, std::vector<Type>) randomList(double _min, double _max, size_t n)
		{
			std::vector<Type> result(n);
			std::uniform_real_distribution<Type> distrib(_min, _max);

			for (typename std::vector<Type>::iterator x = result.begin(); x != result.end(); ++x)
				*x = distrib(generator);

			return result;
		}

		template<typename Type>
		ENABLE_IF_INT(Type, std::vector<Type>) randomList(double _min, double _max, size_t n)
		{
			std::vector<Type> result(n);
			std::uniform_int_distribution<Type> distrib((int)_min, (int)_max);

			for (typename std::vector<Type>::iterator x = result.begin(); x != result.end(); ++x)
				*x = distrib(generator);

			return result;
		}
		
		template<typename Type>
		ENABLE_IF_FLOAT(Type, std::vector<Type>) randomListNormal(double mean, double deviation, size_t n)
		{
			std::vector<Type> result(n);
			std::normal_distribution<Type> distrib(mean, deviation);

			for (typename std::vector<Type>::iterator x = result.begin(); x != result.end(); ++x)
				*x = distrib(generator);

			return result;
		}

	private:
		_rng generator;

		_rng createRandomGenerator() const
		{
			using namespace std;

			// Create random number generator
			random_device rd;
			mt19937::result_type seed = rd() ^
				(
					(mt19937::result_type)
					chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count() +
					(mt19937::result_type)
					chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()
					);

			return mt19937(seed);
		}
	};
}