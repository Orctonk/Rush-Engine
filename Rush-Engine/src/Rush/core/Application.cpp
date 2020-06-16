#include "Application.h"

#include "Logger.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/events/EventQueue.h"

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
        PollEvents();

        Renderer::GetAPI()->Clear();
        for(auto l : m_LayerStack)
            l->OnUpdate();
        Update();
    }
    Exit();
}

void Application::PushLayer(Layer *layer){
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer *layer){
    m_LayerStack.PushOverlay(layer);
}

void Application::PollEvents(){
    Event *e;
    while((e = EventQueue::GetInstance().ConsumeEvent()) != nullptr){
        e->Dispatch<WindowCloseEvent>(RUSH_BIND_FN(Application::WindowCloseHandle));
        RUSH_LOG_INFO(e->GetString());
    }
}

bool Application::WindowCloseHandle(WindowCloseEvent &e){
    m_Running = false;
    return true;
}

}