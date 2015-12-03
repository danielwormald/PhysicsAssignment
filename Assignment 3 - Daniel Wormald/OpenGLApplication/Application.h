#pragma once

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class Application
{
public:
	virtual void StartUp() = 0;
	virtual void ShutDown() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	int Run();

	GLFWwindow* window;
private:

	
	

};
