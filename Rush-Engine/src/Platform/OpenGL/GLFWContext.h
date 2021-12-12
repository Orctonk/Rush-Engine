#ifndef __GLFWCONTEXT_H__
#define __GLFWCONTEXT_H__

#include "Rush/core/Context.h"

#include <GLFW/glfw3.h>

namespace Rush {

class GLFWContext : public Context {
private:
    GLFWwindow *m_Context;

public:
    GLFWContext(GLFWwindow *windowContext);
    ~GLFWContext();

    void Init() override;
    void MakeCurrent() override;
    void SwapBuffer() override;
};

} // namespace Rush

#endif // __GLFWCONTEXT_H__
