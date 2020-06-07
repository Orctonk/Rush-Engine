#include "Application.h"

#include "Logger.h"

namespace Rush {

Application::Application(){
	WindowProperties props;
	m_Window = AbstractWindow::CreateWindow(props);
}

Application::~Application(){

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