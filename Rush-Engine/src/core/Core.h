#ifdef RUSH_WINDOWS
	#ifdef RUSH_BUILD_SHARED
		#define RUSH_API __declspec(dllexport)
	#else
		#define RUSH_API __declspec(dllimport)
	#endif
#elif RUSH_LINUX
	#ifdef RUSH_BUILD_SHARED
		#define RUSH_API __attribute__((visibility("default")))
	#else
		#define RUSH_API
	#endif
#else
	#error	Platform not supported!
#endif
