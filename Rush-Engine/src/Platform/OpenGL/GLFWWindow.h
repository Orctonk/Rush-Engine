#ifndef __GLFWWINDOW_H__
#define __GLFWWINDOW_H__

#include "Rush/core/Core.h"
#include "Rush/core/AbstractWindow.h"
#include "Rush/core/Context.h"

#include <GLFW/glfw3.h>

namespace Rush{

class GLFWWindow : public AbstractWindow {
private:
    static int s_WindowCount;
    GLFWwindow *m_Window;
    Unique<Context> m_Context;

public:
    GLFWWindow(const WindowProperties &properties);
    ~GLFWWindow();

    void Move(int xPos,int yPos) override;
    void Resize(int width, int height) override;
    void SetWindowMode(WindowMode mode) override;
    void Update() override;
    void SetVSync(bool enable) override;
};

}


#endif // __GLFWWINDOW_H__
