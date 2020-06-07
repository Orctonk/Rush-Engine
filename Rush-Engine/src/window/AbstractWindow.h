#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "../graphics/Context.h"
#include "../core/Core.h"

namespace Rush {

enum class WindowMode{
    WINDOWED,
    WINDOWED_FULLSCREEN,
    FULLSCREEN
};

struct WindowProperties{
    char *m_Title = nullptr;
    WindowMode windowMode = WindowMode::WINDOWED;
    int xPos = 0,yPos = 0;
    int width = 640,height = 480;
};

class AbstractWindow{
protected:
    WindowProperties m_Properties;
    AbstractWindow(const WindowProperties &props);

public:
    virtual ~AbstractWindow() = 0;

    virtual void Move(int xPos,int yPos) = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void SetWindowMode(WindowMode mode) = 0;
    virtual void Update() = 0;

    static Unique<AbstractWindow> CreateWindow(const WindowProperties &properties);
};
    
} // namespace Rush


#endif // __WINDOW_H__
