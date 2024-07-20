#pragma once

#include <iostream>
#include <memory>

#define ENABLE_CUP_ASSERTS
#define ENABLE_CUP_LOGGING

#ifdef ENABLE_CUP_ASSERTS
#define CUP_ASSERT(x, ...) { if(!(x)) { std::cout << __VA_ARGS__ << std::endl; __debugbreak(); } }
#define CUP_ASSERT_FUNC(x, r, ...) { if(!(x)) { std::cout << __VA_ARGS__ << std::endl; __debugbreak(); r; } }
#else
#define CUP_ASSERT(x, ...)
#define CUP_ASSERT_FUNC(x, ...)
#endif

#ifdef ENABLE_CUP_LOGGING
#define CUP_LOG(...) { std::cout << __VA_ARGS___ << "\n"; }
#else
#define CUP_LOG()
#endif


#ifdef ENABLE_CUP_BENCHMARKING

#include <chrono>
#include <string>

namespace Cup {

	class ScopedTimer
	{
	public:
		ScopedTimer(const std::string& name);
		~ScopedTimer();

	private:
		const std::chrono::time_point<std::chrono::steady_clock> m_start;
		const std::string m_name;
	};

	inline ScopedTimer::ScopedTimer(const std::string& name)
		: m_name(name), m_start(std::chrono::high_resolution_clock::now())
	{
	}

	inline ScopedTimer::~ScopedTimer()
	{
		const auto endTimepoint = std::chrono::high_resolution_clock::now();

		const long long start = std::chrono::time_point_cast<std::chrono::microseconds> (m_start).time_since_epoch().count();
		const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		const auto time = end - start;

		// could create algorithim to add space
		std::cout << m_name << ":" << "\n";
		std::string blankspace(m_name.size(), ' ');
		std::cout << blankspace << time << " mcs" << "\n";
		std::cout << "-------------------------------------------" << "\n";
	}

}

#define SCOPE(name) Cup::ScopedTimer timer##__LINE__(name)
#define SCOPE_FUNC() SCOPE(__func__)

#else
#define SCOPE(...)
#define SCOPE_FUNC()
#endif