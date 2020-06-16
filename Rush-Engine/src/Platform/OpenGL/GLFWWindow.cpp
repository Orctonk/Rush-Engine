#include <string>

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

		glfwSetErrorCallback(glfwErrorCallback);
		RUSH_LOG_INFO("GLFW initialized");
    }
    if(m_Properties.m_Title == nullptr)
        m_Properties.m_Title = "Title";
    m_Window = glfwCreateWindow(
        m_Properties.width,
        m_Properties.height,
        m_Properties.m_Title,
        nullptr,
        nullptr
    );
    
    m_Context = CreateUnique<GLFWContext>(m_Window);
    m_Context->Init();

    Move(m_Properties.xPos,m_Properties.yPos);
    SetWindowMode(m_Properties.windowMode);
    s_WindowCount++;

    EventQueue::GetInstance().PostEvent(new WindowOpenEvent());

    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* win, int x, int y){
        EventQueue::GetInstance().PostEvent(new WindowMoveEvent(x,y));
    });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win, int width, int height){
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

}