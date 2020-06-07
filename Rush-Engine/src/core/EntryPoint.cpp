#include "EntryPoint.h"
#include "Logger.h"
#include "Core.h"

#ifdef RUSH_OPENGL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

#include <iostream>

namespace Rush {

	bool Init(){
		Logger::Init();
		Logger::SetAlias("Initialization");

		RUSH_LOG_INFO("Initialization completed");
		Logger::SetAlias("Main");
		return true;
	}

	void Exit(){
		Logger::SetAlias("Exit");
		RUSH_LOG_INFO("Exiting Rush");
		RUSH_LOG_INFO("Exit completed");
		Logger::Destroy();
	}


}
