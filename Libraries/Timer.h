#pragma once

#include <chrono>

namespace TimeUnits
{
	using nano		= std::ratio<1, 1000000000>;
	using micro		= std::ratio<1, 1000000>;
	using mili		= std::ratio<1, 1000>;
	using sec		= std::ratio<1>;
	using min		= std::ratio<60>;
	using hour		= std::ratio<3600>;
}


class Timer
{
	using _time = std::chrono::steady_clock::time_point;
	typedef void (*callback_function)(void);

public:
	Timer() { reset(); }

	/* Update base_time to current time */
	inline void reset() {
		t_begin = getTime();
	}

	/* Returns time passed after last reset in miliseconds*/
	template<typename Unit = TimeUnits::sec>
	inline double deltaTime() const {
		return std::chrono::duration<double, Unit>(getTime() - t_begin).count();
	}

	/* Measures runtime length of given funtion*/
	template<typename Unit = TimeUnits::sec>
	static inline double functionRuntime(callback_function func)
	{
		Timer timer;
		func();
		return timer.deltaTime<Unit>();
	}

private:
	_time t_begin;

	inline _time getTime() const {
		return std::chrono::high_resolution_clock::now();
	}
};

