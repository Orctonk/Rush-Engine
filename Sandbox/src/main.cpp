#include <Rush.h>
#include <iostream>

int main()
{
	Rush::Init();

	{
	Rush::Window g_window("Test", 1024, 768);

	while (!g_window.ShouldClose())
	{
		g_window.SwapBuffers();
	}
	}
	Rush::Exit();
	return 0;
}
