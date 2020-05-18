#include "EntryPoint.h"
#include "Logger.h"
#include "Core.h"

#ifdef RUSH_OPENGL
	#include <GLFW/glfw3.h>
#endif

#include <iostream>

#include "../events/EventManager.h"
#include "../events/WindowEvent.h"

namespace Rush {
	#ifdef RUSH_OPENGL
	void glfwErrorCallback(int errorCode, const char * message){
		Logger::Error("(" + std::to_string(errorCode) + ") " + message);
	}
	#endif

	void Init(){
		Logger::Init();
		Logger::SetAlias("Initialization");
	#ifdef RUSH_OPENGL
		Logger::Info("Initializing OpenGL...");
		Logger::Info("Initializing GLFW...");
		if(!glfwInit()){
			Logger::Error("Failed to initialize GLFW!");
		}
		else{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_SAMPLES,4);

			glfwSetErrorCallback(glfwErrorCallback);
			Logger::Info("GLFW initialized");
		}

		Logger::Info("OpenGL initialized");
	#endif

		Logger::Info("Initialization completed");
		Logger::SetAlias("Main");
	}

	void Exit(){
		Logger::SetAlias("Exit");
		Logger::Info("Exiting Rush");
	#ifdef RUSH_OPENGL
		Logger::Info("Uninitializing OpenGL...");
		Logger::Info("Terminating GLFW...");
		glfwTerminate();
		Logger::Info("GLFW terminated");
	#endif
		Logger::Info("Exit completed");
		Logger::Destroy();
	}


}
