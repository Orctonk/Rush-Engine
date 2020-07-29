#ifndef __CORE_H__
#define __CORE_H__
#include <memory>
#include "Rush/profiling/Profiler.h"

#ifdef _WIN32
	#define RUSH_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
	#define RUSH_PLATFORM_MACOS
#elif defined(__linux__)
	#define RUSH_PLATFORM_LINUX
#else
	#error "Unknown platform"
#endif

#ifdef RUSH_PLATFORM_WINDOWS
	#ifdef RUSH_BUILD_SHARED
		#define RUSH_API __declspec(dllexport)
	#else
		#define RUSH_API __declspec(dllimport)
	#endif
#elif defined(RUSH_PLATFORM_LINUX)
	#ifdef RUSH_BUILD_SHARED
		#define RUSH_API __attribute__((visibility("default")))
	#else
		#define RUSH_API
	#endif
#else
	#error	Platform not supported!
#endif

#ifdef RUSH_DEBUG
	#ifdef RUSH_PLATFORM_WINDOWS
		#define RUSH_BREAK() __debugbreak()
	#elif defined(RUSH_PLATFORM_LINUX)
		#include <signal.h>
		#define RUSH_BREAK() raise(SIGTRAP)
	#else
		#error "Platform does not support debug mode!"
	#endif
	#define RUSH_ENABLE_ASSERT
#else
	#define RUSH_BREAK()
#endif

#ifdef RUSH_ENABLE_ASSERT
	#define RUSH_ASSERT(x) { if(!x) { RUSH_BREAK(); } }
#else
	#define RUSH_ASSERT(x) 
#endif

namespace Rush {

	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Unique<T> CreateUnique(Args&& ... args){
		return std::make_unique<T>(std::forward<Args>(args) ...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Shared<T> CreateShared(Args&& ... args){
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}
	
} // namespace Rush
#endif // __CORE_H__
