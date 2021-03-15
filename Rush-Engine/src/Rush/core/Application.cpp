#include "Rushpch.h"
#include "Application.h"

#include "Time.h"
#include "Input.h"
#include "Random.h"
#include "Settings.h"
#include "Rush/resources/AssetManager.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/graphics/LineRenderer.h"
#include "Rush/graphics/Renderer2D.h"
#include "Rush/events/EventQueue.h"
#include "Rush/debug/DebugLayer.h"

namespace Rush {

Application *Application::s_Instance;

Application::Application(){ 
    s_Instance = this;
    Settings::Init();
    Random::Init();
    Time::Init();
    Profiler::Init();
    Logger::Init();
	Logger::SetAlias("Initialization");
    WindowProperties props;
    props.m_Title = "Rush Engine";
	m_Window = AbstractWindow::CreateWindow(props);
    m_Window->SetMaximized(true);
    Renderer::Init();
    LineRenderer::Init();
    Renderer2D::Init();
    AssetManager::Init();
	RUSH_LOG_INFO("Initialization completed");
	Logger::SetAlias("Main");
}

Application::~Application(){
    Logger::SetAlias("Exit");
	RUSH_LOG_INFO("Exiting Rush");
    RUSH_LOG_INFO("Removing layers");
    for(auto l : m_LayerStack){
        l->OnDetach();
    }
    AssetManager::Shutdown();
    Renderer2D::Shutdown();
    LineRenderer::Shutdown();
    Renderer::Shutdown();
	RUSH_LOG_INFO("Exit completed");
	Logger::Destroy();
    Profiler::Shutdown();
    Time::Shutdown();
}

void Application::Run(){
    RUSH_PROFILE_FUNCTION();
    Init();
    m_ImguiLayer = new ImguiLayer();
    PushOverlay(m_ImguiLayer);
#ifdef RUSH_DEBUG
    PushOverlay(new DebugLayer());
#endif
    Time::Update();
    m_Running = true;
    while(m_Running){
        RUSH_PROFILE_SCOPE("Main loop");
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
    RUSH_PROFILE_FUNCTION();
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer *layer){
    RUSH_PROFILE_FUNCTION();
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::PollEvents(){
    RUSH_PROFILE_FUNCTION();
    Event *e;
    while((e = EventQueue::GetInstance().ConsumeEvent()) != nullptr){
        RUSH_PROFILE_SCOPE("Event Polling");
        e->Dispatch<WindowCloseEvent>(RUSH_BIND_FN(Application::WindowCloseHandle));
        e->Dispatch<WindowResizeEvent>(RUSH_BIND_FN(Application::WindowResizeHandle));
        Input::Update(*e);
        RUSH_LOG_TRACE(e->GetString());
        for(auto it = m_LayerStack.end();it != m_LayerStack.begin(); it--){
            (*(it-1))->OnEvent(*e);
            if(e->IsHandled())
                break;
        }
        delete e;
    }
}

bool Application::WindowCloseHandle(WindowCloseEvent &e){
    m_Running = false;
    return false;
}

bool Application::WindowResizeHandle(WindowResizeEvent &e){
    RUSH_PROFILE_FUNCTION();
    Renderer::OnResize(e.width,e.height);
    return false;
}

Application& Application::GetInstance() { return *s_Instance; }

}