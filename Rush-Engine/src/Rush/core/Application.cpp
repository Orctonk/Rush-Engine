#include "Application.h"

#include "Logger.h"
#include "Rush/graphics/Renderer.h"

namespace Rush {

Application::Application(){
	WindowProperties props;
	m_Window = AbstractWindow::CreateWindow(props);
}

Application::~Application(){

}

void Application::Start(){
    Renderer::Init();
}
void Application::Shutdown(){
    Renderer::Shutdown();
}
void Application::Run(){
    Start();
    Init();
    m_Running = true;
    while(m_Running){
        Renderer::GetAPI()->Clear();
        Update();
    }
    Exit();
    Shutdown();
}

}