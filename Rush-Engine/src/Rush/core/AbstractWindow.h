#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Context.h"
#include "Core.h"

namespace Rush {

enum class WindowMode{
    WINDOWED,
    WINDOWED_FULLSCREEN,
    FULLSCREEN
};

struct WindowProperties{
    char *m_Title = nullptr;
    WindowMode windowMode = WindowMode::WINDOWED;
    int xPos = 0, yPos = 0;
    int width = 1024, height = 768;
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
    virtual void SetVSync(bool enable) = 0;

    int GetWidth() { return m_Properties.width; }
    int GetHeight() { return m_Properties.height; }

    static Unique<AbstractWindow> CreateWindow(const WindowProperties &properties);
};
    
} // namespace Rush


#endif // __WINDOW_H__
