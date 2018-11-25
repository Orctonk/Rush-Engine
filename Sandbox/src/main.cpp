#include <Rush.h>
#include <iostream>

int main()
{
	Rush::Init();

	Rush::Window g_window("Test", 640, 480);

 	std::cout << sizeof(g_window) << std::endl;

	//g_window.SetWindowMode(Rush::WindowMode::Fullscreen);

	while (!g_window.ShouldClose())
	{
		g_window.SwapBuffers();
	}

	Rush::Exit();
	return 0;
}
