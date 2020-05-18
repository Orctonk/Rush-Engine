#include "AbstractWindow.h"

#ifdef RUSH_OPENGL
#include "GLFWWindow.h"
#else
    #error "No window manager provided!";
#endif

namespace Rush{

AbstractWindow::AbstractWindow(const WindowProperties &properties){
    m_Properties = properties;
}

AbstractWindow::~AbstractWindow(){ 
    
}

#ifdef RUSH_OPENGL
AbstractWindow *AbstractWindow::CreateWindow(const WindowProperties &properties){
    return new GLFWWindow(properties);
}
#endif

}