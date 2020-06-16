#ifndef __RUSH_APPLICATION_H__
#define __RUSH_APPLICATION_H__

#include "Core.h"
#include "LayerStack.h"
#include "AbstractWindow.h"
#include "Rush/events/WindowEvent.h"

namespace Rush {

class Application {
private:
    LayerStack m_LayerStack;

    void PollEvents();
    bool WindowCloseHandle(WindowCloseEvent &e);
protected:
    bool m_Running;
    Unique<AbstractWindow> m_Window;
public:
    Application();
    virtual ~Application();
    
    virtual void Init() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *layer);
    void Run();
};

Application* CreateApplication();
    
} // namespace Rush


#endif // __RUSH_ENTRY_H__