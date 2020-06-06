#ifndef __RUSH_APPLICATION_H__
#define __RUSH_APPLICATION_H__

#include "../window/AbstractWindow.h"

namespace Rush {

class Application {
protected:
    bool m_Running;
    AbstractWindow *m_Window;
public:
    Application();
    virtual ~Application();
    virtual void Init() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
    void Run();
};

Application* CreateApplication();
    
} // namespace Rush


#endif // __RUSH_ENTRY_H__