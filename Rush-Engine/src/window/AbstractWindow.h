#ifndef __WINDOW_H__
#define __WINDOW_H__

namespace Rush {

enum class WindowMode{
    WINDOWED,
    WINDOWED_FULLSCREEN,
    FULLSCREEN
};

struct WindowProperties{
    char *m_Title;
    WindowMode windowMode;
    int xPos,yPos;
    int width,height;
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

    static AbstractWindow *CreateWindow(const WindowProperties &properties);
};
    
} // namespace Rush


#endif // __WINDOW_H__
