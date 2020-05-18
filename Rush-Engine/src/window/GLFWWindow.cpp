#include "GLFWWindow.h"
#include "../events/EventManager.h"
#include "../events/WindowEvent.h"
#include "../events/KeyboardEvent.h"
#include "../events/MouseEvent.h"
#include <string>
#include "../core/Logger.h"

namespace Rush{

void moveCallback(GLFWwindow* win, int x, int y){
    Events::WindowMoveEvent e(x,y);
    Events::EventManager::GetInstance().PostEvent(e);
}

void resizeCallback(GLFWwindow* win, int width, int height){
    Events::WindowResizeEvent e(width,height);
    Events::EventManager::GetInstance().PostEvent(e);
}

void closeCallback(GLFWwindow *win){
    Events::WindowCloseEvent e;
    Events::EventManager::GetInstance().PostEvent(e);
}

void focusCallback(GLFWwindow * win, int focus_gained){
    Events::WindowFocusEvent e(focus_gained == GLFW_TRUE);
    Events::EventManager::GetInstance().PostEvent(e);
}

void keyCallback(GLFWwindow *win, int key, int scan, int action, int mods){
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
}

void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods){
    if(action == GLFW_PRESS){
        Events::MousePressedEvent e(button);
        Events::EventManager::GetInstance().PostEvent(e);
    } else if(action == GLFW_RELEASE){
        Events::MousePressedEvent e(button);
        Events::EventManager::GetInstance().PostEvent(e);
    }
}

void mouseMoveCallback(GLFWwindow *win, double x, double y){
    Events::MouseMoveEvent e(x,y);
    Events::EventManager::GetInstance().PostEvent(e);
}

void mouseScrollCallback(GLFWwindow *win, double x, double y){
    Events::MouseScrollEvent e(y);
    Events::EventManager::GetInstance().PostEvent(e);
}

GLFWWindow::GLFWWindow(const WindowProperties &properties) :
    AbstractWindow(properties)
{
    m_Window = glfwCreateWindow(m_Properties.width,m_Properties.height,m_Properties.m_Title,nullptr,nullptr);
    Move(m_Properties.xPos,m_Properties.yPos);
    SetWindowMode(m_Properties.windowMode);

    Events::WindowOpenEvent e;
    Events::EventManager::GetInstance().PostEvent(e);

    glfwSetWindowPosCallback(m_Window, moveCallback);
    glfwSetWindowSizeCallback(m_Window, resizeCallback);
    glfwSetWindowCloseCallback(m_Window, closeCallback);
    glfwSetWindowFocusCallback(m_Window, focusCallback);
    glfwSetKeyCallback(m_Window,keyCallback);
    glfwSetMouseButtonCallback(m_Window,mouseButtonCallback);
    glfwSetCursorPosCallback(m_Window,mouseMoveCallback);
    glfwSetScrollCallback(m_Window, mouseScrollCallback);
}

GLFWWindow::~GLFWWindow() {
    glfwDestroyWindow(m_Window);
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