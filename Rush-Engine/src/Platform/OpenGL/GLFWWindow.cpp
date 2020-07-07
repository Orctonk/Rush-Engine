#include <string>
#include <glad/glad.h>

#include "GLFWWindow.h"

#include "GLFWContext.h"
#include "Rush/events/EventQueue.h"
#include "Rush/events/WindowEvent.h"
#include "Rush/events/KeyboardEvent.h"
#include "Rush/events/MouseEvent.h"
#include "Rush/core/Logger.h"

namespace Rush{

static void glfwErrorCallback(int errorCode, const char * message){
    RUSH_LOG_ERROR("(" + std::to_string(errorCode) + ") " + message);
}

static void APIENTRY glDebugOutput(  GLenum source, 
                                        GLenum type, 
                                        unsigned int id, 
                                        GLenum severity, 
                                        GLsizei length, 
                                        const char *message, 
                                        const void *userParam){
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::string messagestr = std::string("OpenGL Message (") + std::to_string(id) + "): " +  message;
    std::string sourcestr;
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             sourcestr = std::string("API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourcestr = std::string("Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourcestr = std::string("Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourcestr = std::string("Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourcestr = std::string("Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           sourcestr = std::string("Other"); break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               messagestr += std::string(" Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: messagestr += std::string(" Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  messagestr += std::string(" Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         messagestr += std::string(" Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         messagestr += std::string(" Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              messagestr += std::string(" Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          messagestr += std::string(" Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           messagestr += std::string(" Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               messagestr += std::string(" Type: Other"); break;
    }
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         Logger::Error(messagestr,sourcestr);; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       Logger::Warning(messagestr,sourcestr);; break;
        case GL_DEBUG_SEVERITY_LOW:          Logger::Info(messagestr,sourcestr);; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: Logger::Trace(messagestr,sourcestr);; break;
    }
}

int GLFWWindow::s_WindowCount = 0;

GLFWWindow::GLFWWindow(const WindowProperties &properties) :
    AbstractWindow(properties)
{
    if(s_WindowCount == 0){
        RUSH_LOG_INFO("Initializing GLFW...");
        if(!glfwInit()){
			RUSH_LOG_ERROR("Failed to initialize GLFW!");
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES,4);
#ifdef RUSH_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,true);
#endif

		glfwSetErrorCallback(glfwErrorCallback);
		RUSH_LOG_INFO("GLFW initialized");
    }
    if(m_Properties.m_Title == nullptr)
        m_Window = glfwCreateWindow(
            m_Properties.width,
            m_Properties.height,
            "Title",
            nullptr,
            nullptr
        );
    else 
        m_Window = glfwCreateWindow(
            m_Properties.width,
            m_Properties.height,
            m_Properties.m_Title,
            nullptr,
            nullptr
        );

    glfwSetWindowUserPointer(m_Window,&m_Properties);
    
    m_Context = CreateUnique<GLFWContext>(m_Window);
    m_Context->Init();

#ifdef RUSH_DEBUG
    GLint flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);        
    }
#endif

    Move(m_Properties.xPos,m_Properties.yPos);
    SetWindowMode(m_Properties.windowMode);
    s_WindowCount++;

    EventQueue::GetInstance().PostEvent(new WindowOpenEvent());

    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* win, int x, int y){
        WindowProperties *props = (WindowProperties*)glfwGetWindowUserPointer(win);
        props->xPos = x;
        props->yPos = y;
        EventQueue::GetInstance().PostEvent(new WindowMoveEvent(x,y));
    });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win, int width, int height){
        WindowProperties *props = (WindowProperties*)glfwGetWindowUserPointer(win);
        props->width = width;
        props->height = height;
        EventQueue::GetInstance().PostEvent(new WindowResizeEvent(width,height));
    });
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *win){
        EventQueue::GetInstance().PostEvent(new WindowCloseEvent());
    });
    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow * win, int focus_gained){
        EventQueue::GetInstance().PostEvent(new WindowFocusEvent(focus_gained == GLFW_TRUE));
    });
    glfwSetKeyCallback(m_Window,[](GLFWwindow *win, int key, int scan, int action, int mods){
        if(action == GLFW_PRESS){
            EventQueue::GetInstance().PostEvent(new KeyboardPressEvent(key));
        } else if (action == GLFW_REPEAT){
            EventQueue::GetInstance().PostEvent(new KeyboardRepeatEvent(key));
        } else if (action == GLFW_RELEASE){
            EventQueue::GetInstance().PostEvent(new KeyboardReleaseEvent(key));
        }
    });
    glfwSetCharCallback(m_Window, [](GLFWwindow *win, unsigned int character){
        EventQueue::GetInstance().PostEvent(new KeyboardTypeEvent(character));
    });

    glfwSetMouseButtonCallback(m_Window,[](GLFWwindow *win, int button, int action, int mods){
        if(action == GLFW_PRESS){
            EventQueue::GetInstance().PostEvent(new MousePressedEvent(button));
        } else if(action == GLFW_RELEASE){
            EventQueue::GetInstance().PostEvent(new MouseReleasedEvent(button));
        }
    });
    glfwSetCursorPosCallback(m_Window,[](GLFWwindow *win, double x, double y){
        EventQueue::GetInstance().PostEvent(new MouseMoveEvent(x,y));
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow *win, double x, double y){
        EventQueue::GetInstance().PostEvent(new MouseScrollEvent(y));
    });
}

GLFWWindow::~GLFWWindow() {
    glfwDestroyWindow(m_Window);
    s_WindowCount--;
    if(s_WindowCount == 0){
        glfwTerminate();
    }
}


void GLFWWindow::Move(int xPos,int yPos) {
    m_Properties.xPos = xPos;
    m_Properties.yPos = yPos;
    glfwSetWindowPos(m_Window,xPos,yPos);
}


void GLFWWindow::Resize(int width, int height) {
    m_Properties.width = width;
    m_Properties.height = height;
    glfwSetWindowSize(m_Window,width,height);
}


void GLFWWindow::SetWindowMode(WindowMode mode) {
    m_Properties.windowMode = mode;   
    const GLFWvidmode *vmode = glfwGetVideoMode(glfwGetPrimaryMonitor()); 
    switch (mode){
    case WindowMode::FULLSCREEN:
        glfwSetWindowMonitor(m_Window,glfwGetPrimaryMonitor(),0,0,m_Properties.width,m_Properties.height,0);
        break;
    case WindowMode::WINDOWED_FULLSCREEN:
        glfwSetWindowMonitor(m_Window,nullptr,m_Properties.xPos,m_Properties.yPos,m_Properties.width,m_Properties.height,60);
        glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(),0,0,vmode->width, vmode->height, vmode->refreshRate);
        break;
    case WindowMode::WINDOWED:
        glfwSetWindowMonitor(m_Window,nullptr,m_Properties.xPos,m_Properties.yPos,m_Properties.width,m_Properties.height,60);
        break;
    }
}

void GLFWWindow::Update() {
    glfwPollEvents();
    m_Context->SwapBuffer();
}

void GLFWWindow::SetVSync(bool enable) {
    glfwSwapInterval(enable ? 1 : 0);
}
}