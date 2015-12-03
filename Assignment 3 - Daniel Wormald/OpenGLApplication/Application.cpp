#include "Application.h"
#include "ShaderManager.h"

int Application::Run()
{
	if (glfwInit() == false)
	return -1;

	//Create Window
	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);
	
	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	StartUp();

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Update();
		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ShutDown();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}