#include <string>

#include "GLFWWindow.h"
#include "../events/EventManager.h"
#include "../events/WindowEvent.h"
#include "../events/KeyboardEvent.h"
#include "../events/MouseEvent.h"
#include "../core/Logger.h"

namespace Rush{

static void glfwErrorCallback(int errorCode, const char * message){
    Logger::Error("(" + std::to_string(errorCode) + ") " + message);
}

int GLFWWindow::s_WindowCount = 0;

GLFWWindow::GLFWWindow(const WindowProperties &properties) :
    AbstractWindow(properties)
{
    if(s_WindowCount == 0){
        Logger::Info("Initializing GLFW...");
        if(!glfwInit()){
			Logger::Error("Failed to initialize GLFW!");
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES,4);

		glfwSetErrorCallback(glfwErrorCallback);
		Logger::Info("GLFW initialized");
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
    Move(m_Properties.xPos,m_Properties.yPos);
    SetWindowMode(m_Properties.windowMode);
    s_WindowCount++;

    Events::WindowOpenEvent e;
    Events::EventManager::GetInstance().PostEvent(e);

    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* win, int x, int y){
        Events::WindowMoveEvent e(x,y);
        Events::EventManager::GetInstance().PostEvent(e);
    });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win, int width, int height){
        Events::WindowResizeEvent e(width,height);
        Events::EventManager::GetInstance().PostEvent(e);
    });
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *win){
        Events::WindowCloseEvent e;
        Events::EventManager::GetInstance().PostEvent(e);
    });
    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow * win, int focus_gained){
        Events::WindowFocusEvent e(focus_gained == GLFW_TRUE);
        Events::EventManager::GetInstance().PostEvent(e);
    });
    glfwSetKeyCallback(m_Window,[](GLFWwindow *win, int key, int scan, int action, int mods){
        if(action == GLFW_PRESS){
            Events::KeyboardPressEvent e(key);
            Events::EventManager::GetInstance().PostEvent(e);
        } else if (action == GLFW_REPEAT){
            Events::KeyboardRepeatEvent e(key);
            Events::EventManager::GetInstance().PostEvent(e);
        } else if (action == GLFW_RELEASE){
            Events::KeyboardReleaseEvent e(key);
            Events::EventManager::GetInstance().PostEvent(e);
        }
    });
    glfwSetMouseButtonCallback(m_Window,[](GLFWwindow *win, int button, int action, int mods){
        if(action == GLFW_PRESS){
            Events::MousePressedEvent e(button);
            Events::EventManager::GetInstance().PostEvent(e);
        } else if(action == GLFW_RELEASE){
            Events::MousePressedEvent e(button);
            Events::EventManager::GetInstance().PostEvent(e);
        }
    });
    glfwSetCursorPosCallback(m_Window,[](GLFWwindow *win, double x, double y){
        Events::MouseMoveEvent e(x,y);
        Events::EventManager::GetInstance().PostEvent(e);
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow *win, double x, double y){
        Events::MouseScrollEvent e(y);
        Events::EventManager::GetInstance().PostEvent(e);
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
    glfwSwapBuffers(m_Window);
}

}