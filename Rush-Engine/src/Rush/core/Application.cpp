#include "Application.h"

#include "Logger.h"
#include "Rush/graphics/Renderer.h"

namespace Rush {

Application::Application(){ 
    Logger::Init();
	Logger::SetAlias("Initialization");
    WindowProperties props;
	m_Window = AbstractWindow::CreateWindow(props);
    Renderer::Init();
	RUSH_LOG_INFO("Initialization completed");
	Logger::SetAlias("Main");
}

Application::~Application(){
    Logger::SetAlias("Exit");
	RUSH_LOG_INFO("Exiting Rush");
    Renderer::Shutdown();
	RUSH_LOG_INFO("Exit completed");
	Logger::Destroy();
}

void Application::Run(){
    Init();
    m_Running = true;
    while(m_Running){
        Renderer::GetAPI()->Clear();
        Update();
    }
    Exit();
}

}