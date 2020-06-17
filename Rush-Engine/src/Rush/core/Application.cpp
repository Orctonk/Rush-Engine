#include "Application.h"

#include "Time.h"
#include "Logger.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/events/EventQueue.h"
#include "Rush/debug/DebugLayer.h"

namespace Rush {

Application *Application::s_Instance;

Application::Application(){ 
    s_Instance = this;
    Time::Init();
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
    Time::Shutdown();
}

void Application::Run(){
    m_ImguiLayer = new ImguiLayer();
    PushOverlay(m_ImguiLayer);
    PushOverlay(new DebugLayer());
    Init();
    m_Running = true;
    while(m_Running){
        Time::Update();

        PollEvents();
        
        Renderer::GetAPI()->Clear();
        for(auto l : m_LayerStack)
            l->OnUpdate();
        Update();

        m_ImguiLayer->Begin();
        for(auto l : m_LayerStack)
            l->OnImguiRender();
        m_ImguiLayer->End();
        m_Window->Update();
    }
    Exit();
}

void Application::PushLayer(Layer *layer){
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer *layer){
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::PollEvents(){
    Event *e;
    while((e = EventQueue::GetInstance().ConsumeEvent()) != nullptr){
        e->Dispatch<WindowCloseEvent>(RUSH_BIND_FN(Application::WindowCloseHandle));
        e->Dispatch<WindowResizeEvent>(RUSH_BIND_FN(Application::WindowResizeHandle));
        RUSH_LOG_TRACE(e->GetString());
        for(auto it = m_LayerStack.end()-1;it != m_LayerStack.begin() - 1; it--){
            (*it)->OnEvent(*e);
            if(e->IsHandled())
                break;
        }
    }
}

bool Application::WindowCloseHandle(WindowCloseEvent &e){
    m_Running = false;
    return false;
}

bool Application::WindowResizeHandle(WindowResizeEvent &e){
    Renderer::OnResize(e.width,e.height);
    return false;
}

}