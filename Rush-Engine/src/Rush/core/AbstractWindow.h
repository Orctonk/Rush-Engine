#ifndef __ABSTRACTWINDOW_H__
#define __ABSTRACTWINDOW_H__

#include "Context.h"
#include "Core.h"

namespace Rush {

enum class RUSH_API WindowMode {
    WINDOWED,
    WINDOWED_FULLSCREEN,
    FULLSCREEN
};

struct RUSH_API WindowProperties {
    char *m_Title = nullptr;
    WindowMode windowMode = WindowMode::WINDOWED;
    int xPos = -1, yPos = -1;
    uint32_t width = 1024, height = 768;
};

class RUSH_API AbstractWindow {
protected:
    WindowProperties m_Properties;
    AbstractWindow(const WindowProperties &props);

public:
    virtual ~AbstractWindow() = 0;

    virtual void Move(int xPos, int yPos) = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void SetWindowMode(WindowMode mode) = 0;
    virtual void Update() = 0;
    virtual void SetVSync(bool enable) = 0;
    virtual void SetMaximized(bool maximized) = 0;

    uint32_t GetWidth() { return m_Properties.width; }
    uint32_t GetHeight() { return m_Properties.height; }

    static Unique<AbstractWindow> CreateWindow(const WindowProperties &properties);
};

} // namespace Rush

#endif // __ABSTRACTWINDOW_H__
