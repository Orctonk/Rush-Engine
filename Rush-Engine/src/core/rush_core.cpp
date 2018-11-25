#include "rush_core.h"

#ifdef RUSH_OPENGL
	#include <GLFW/glfw3.h>
#endif

#include <iostream>


namespace Rush {

	#ifdef RUSH_OPENGL
	void glfwErrorCallback(int errorCode, const char * message)
	{
		std::cout << "[" << errorCode << "] " << message;
	}
	#endif

	void Init()
	{
	#ifdef RUSH_OPENGL
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_SAMPLES,4);

		glfwSetErrorCallback(glfwErrorCallback);
	#endif
	}

	void Exit()
	{
	#ifdef RUSH_OPENGL
		glfwTerminate();
	#endif
	}


}
