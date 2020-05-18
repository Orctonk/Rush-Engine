#ifndef __RUSH_APPLICATION_H__
#define __RUSH_APPLICATION_H__
namespace Rush {

class Application {
protected:
    bool m_Running;
public:
    Application();
    virtual ~Application() = 0;
    virtual void Init();
    virtual void Exit();
    virtual void Run();
    virtual void Update();
};

Application* CreateApplication();
    
} // namespace Rush


#endif // __RUSH_ENTRY_H__