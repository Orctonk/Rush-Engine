#ifndef __GLFWCONTEXT_H__
#define __GLFWCONTEXT_H__

#include <GLFW/glfw3.h>

#include "Context.h"

namespace Rush {
    
class GLFWContext : public Context {
private:
    GLFWwindow* m_Context;
public:
    GLFWContext(GLFWwindow *windowContext);
    ~GLFWContext();

    virtual void Init() override;
    virtual void MakeCurrent() override;
    virtual void SwapBuffer() override;
};

} // namespace Rush

#endif // __GLFWCONTEXT_H__
