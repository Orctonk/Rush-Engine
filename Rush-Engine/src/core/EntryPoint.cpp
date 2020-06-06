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

		Logger::Info("Initialization completed");
		Logger::SetAlias("Main");
		return true;
	}

	void Exit(){
		Logger::SetAlias("Exit");
		Logger::Info("Exiting Rush");
		Logger::Info("Exit completed");
		Logger::Destroy();
	}


}
