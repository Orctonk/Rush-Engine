#include "Window.h"

#ifdef RUSH_OPENGL
	#include <GLFW/glfw3.h>
#endif

namespace Rush
{

	Window::Window(std::string title, int width, int height)
	//: m_Width(width), m_Height(height)
	{
	#ifdef RUSH_OPENGL
		m_Window = glfwCreateWindow(width,height, title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
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

	// void Window::SetWindowMode(WindowMode mode)
	// {
	// #ifdef RUSH_OPENGL
	// 	switch(mode)
	// 	{
	// 	case WindowMode::Fullscreen:
	// 		glfwSetWindowMonitor(m_Window,glfwGetPrimaryMonitor(),NULL,NULL,NULL,NULL,60);
	// 		break;
	//
	// 	case WindowMode::Windowed:
	// 		glfwSetWindowMonitor(m_Window,NULL,NULL,NULL,m_Width,m_Height,NULL);
	// 	}
	// #endif
	// }

}
