#ifdef RUSH_BUILD_SHARED
	#ifdef RUSH_WINDOWS
		#define RUSH_API __declspec(dllexport)
	#elif RUSH_LINUX
		#define RUSH_API __attribute__((visibility("default")))
	#elif RUSH_MAC
		#define RUSH_API
	#endif
#else
	#ifdef RUSH_WINDOWS
		#define RUSH_API __declspec(dllimport)
	#elif RUSH_LINUX
		#define RUSH_API
	#elif RUSH_MAC
		#define RUSH_API
	#endif
	#define RUSH_OPENGL
#endif

namespace Rush {

	RUSH_API void Init();
	RUSH_API void Exit();

}
