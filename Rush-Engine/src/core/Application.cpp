#include "Application.h"

#include "Logger.h"

#ifdef RUSH_OPENGL
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
#endif

namespace Rush {

Application::Application(){
	WindowProperties props;
	m_Window = AbstractWindow::CreateWindow(props);
	// Logger::Info("Initializing Glad...");
	// if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
	// 	Logger::Error("Failed to initialize Glad!");
	// 	return false;
	// }
	// Logger::Info("Glad initialized");
	// Logger::Info("OpenGL initialized, version: " + std::to_string(GLVersion.major) + "." + std::to_string(GLVersion.minor));
}

Application::~Application(){
	delete m_Window;
}

void Application::Init(){}
void Application::Exit(){}
void Application::Run(){
    Init();
    m_Running = true;
    while(m_Running){
        Update();
    }
    Exit();
}
void Application::Update(){}

}