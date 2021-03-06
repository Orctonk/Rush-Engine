#ifndef __RUSH_APPLICATION_H__
#define __RUSH_APPLICATION_H__

#include "Core.h"
#include "LayerStack.h"
#include "AbstractWindow.h"
#include "Rush/events/WindowEvent.h"
#include "Rush/imgui/ImguiLayer.h"

namespace Rush {

class RUSH_API Application {
private:
    static Application *s_Instance;
    LayerStack m_LayerStack;
    ImguiLayer *m_ImguiLayer;

    void PollEvents();
    bool WindowCloseHandle(WindowCloseEvent &e);
    bool WindowResizeHandle(WindowResizeEvent &e);
protected:
    bool m_Running;
    Unique<AbstractWindow> m_Window;
public:
    Application();
    virtual ~Application();
    
    virtual void Init() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;

    Unique<AbstractWindow> &GetWindow() { return m_Window; }
    void PushLayer(Layer *layer);
    void PushOverlay(Layer *layer);
    void Run();

    static Application& GetInstance();
};

Application* CreateApplication();
    
} // namespace Rush


#endif // __RUSH_ENTRY_H__