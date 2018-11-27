#ifndef _RUSH_WINDOW_H_
#define _RUSH_WINDOW_H_

#include <string>

#include "rush_core.h"

#ifdef RUSH_OPENGL
struct GLFWwindow;
#endif

namespace Rush{

	enum RUSH_API WindowMode
	{
		Fullscreen,
		Windowed,
		Windowed_Fullscreen
	};

	class RUSH_API Window
	{

	private:
		int m_Width, m_Height;

		#ifdef RUSH_OPENGL
		GLFWwindow* m_Window;
		int m_Pre_Fullscreen_X, m_Pre_Fullscreen_Y;
		#endif



	public:
		Window(const char *title, int width, int height);
		~Window();
		bool ShouldClose();
		void SwapBuffers();

		void SetWindowMode(WindowMode mode);
	};

}

#endif // _RUSH_WINDOW_H_
