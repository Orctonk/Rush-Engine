#include "Application.h"

namespace Rush {

Application::Application(){}
Application::~Application(){}
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