#include "GLFWContext.h"
#include "Rushpch.h"
#include <glad/glad.h>

namespace Rush {

GLFWContext::GLFWContext(GLFWwindow *windowContext)
    : m_Context(windowContext) {
    RUSH_PROFILE_FUNCTION();
}

GLFWContext::~GLFWContext() {
    RUSH_PROFILE_FUNCTION();
}

void GLFWContext::Init() {
    RUSH_PROFILE_FUNCTION();
    RUSH_LOG_INFO("Initializing glad");
    MakeCurrent();
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    RUSH_ASSERT(status);

    RUSH_LOG_INFO("OpenGL info:");
    RUSH_LOG_INFO("\tVendor: {}", glGetString(GL_VENDOR));
    RUSH_LOG_INFO("\tRenderer: {}", glGetString(GL_RENDERER));
    RUSH_LOG_INFO("\tVersion: {}", glGetString(GL_VERSION));
    RUSH_LOG_INFO("Finished intitializing glad");
}

void GLFWContext::MakeCurrent() {
    RUSH_PROFILE_FUNCTION();
    glfwMakeContextCurrent(m_Context);
}

void GLFWContext::SwapBuffer() {
    RUSH_PROFILE_FUNCTION();
    glfwSwapBuffers(m_Context);
}

} // namespace Rush
