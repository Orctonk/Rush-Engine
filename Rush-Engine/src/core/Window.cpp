#include "Window.h"

#ifdef RUSH_OPENGL
	#include <GLFW/glfw3.h>
#endif

#include <iostream>

namespace Rush
{

	Window::Window(const char *title, int width, int height)
	: m_Width(width), m_Height(height)
	{
	#ifdef RUSH_OPENGL
		m_Window = glfwCreateWindow(width,height, title, NULL, NULL);
		if(m_Window == nullptr)
		{
			std::cerr << "Failed to create a GLFW window!" << std::endl;
		}
		else
		{
			glfwMakeContextCurrent(m_Window);
			glfwSwapInterval(1);
		}
	#endif
	}

	Window::~Window()
	{
	#ifdef RUSH_OPENGL
		glfwDestroyWindow(m_Window);
	#endif
	}

	bool Window::ShouldClose()
	{
	#ifdef RUSH_OPENGL
		return glfwWindowShouldClose(m_Window);
	#endif
	}

	void Window::SwapBuffers()
	{
	#ifdef RUSH_OPENGL
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	#endif
	}

	void Window::SetWindowMode(WindowMode mode)
	{
	#ifdef RUSH_OPENGL
		switch(mode)
		{
		case WindowMode::Fullscreen:
			glfwGetWindowPos(m_Window,&m_Pre_Fullscreen_X,&m_Pre_Fullscreen_Y);
			glfwSetWindowMonitor(m_Window,glfwGetPrimaryMonitor(),0,0,m_Width,m_Height,60);
			break;

		case WindowMode::Windowed:
			glfwSetWindowMonitor(m_Window,nullptr,m_Pre_Fullscreen_X,m_Pre_Fullscreen_Y,m_Width,m_Height,GLFW_DONT_CARE);
		}
	#endif
	}

}
