#include <glad/glad.h>
#include "GLFWContext.h"

#include "Rush/core/Core.h"
#include "Rush/core/Logger.h"

namespace Rush {

GLFWContext::GLFWContext(GLFWwindow *windowContext)
    : m_Context(windowContext) {
    
}


GLFWContext::~GLFWContext() {
    
}


void GLFWContext::Init() {
    RUSH_LOG_INFO("Initializing glad");
    MakeCurrent();
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    RUSH_ASSERT(status);

    RUSH_LOG_INFO("OpenGL info:");
    RUSH_LOG_INFO(std::string("\tVendor: ") + std::string((char*)glGetString(GL_VENDOR)));
    RUSH_LOG_INFO(std::string("\tRenderer: ") + std::string((char*)glGetString(GL_RENDERER)));
    RUSH_LOG_INFO(std::string("\tVersion: ") + std::string((char*)glGetString(GL_VERSION)));
    RUSH_LOG_INFO("Finished intitializing glad");
}

void GLFWContext::MakeCurrent() {
    glfwMakeContextCurrent(m_Context);
}


void GLFWContext::SwapBuffer() {
    glfwSwapBuffers(m_Context);
}    

} // namespace Rush
