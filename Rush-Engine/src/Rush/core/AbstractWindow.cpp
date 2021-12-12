#include "AbstractWindow.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/GLFWWindow.h"
#else
    #error "No window manager provided!";
#endif

namespace Rush {

AbstractWindow::AbstractWindow(const WindowProperties &properties) {
    m_Properties = properties;
}

AbstractWindow::~AbstractWindow() {
}

Unique<AbstractWindow> AbstractWindow::CreateWindow(const WindowProperties &properties) {
#ifdef RUSH_OPENGL
    return CreateUnique<GLFWWindow>(properties);
#endif
}

} // namespace Rush