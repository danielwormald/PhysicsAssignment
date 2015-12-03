
#include "AntTweakBar.h"

AntTweakBar::AntTweakBar( GLFWwindow *window )
{
	m_window = window;

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	m_bar = TwNewBar("Bar");

	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);

	m_clearColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
	m_lightPos = glm::vec3(30000);
	m_lightColour = glm::vec3(1);
	m_specPower = 1.f;

	m_seed = 0;
	m_octaves = 6;

	TwAddVarRW(m_bar, "Clear colour", TW_TYPE_COLOR4F, &m_clearColour[0], "");
	TwAddVarRW(m_bar, "Test", TW_TYPE_UINT32, &m_Test, "group=Procedural");
	
}

void AntTweakBar::Draw()
{
	TwDraw();
}

AntTweakBar::~AntTweakBar()
{
	TwDeleteAllBars();
	TwTerminate();
}

void AntTweakBar::SetSeed(unsigned int s)
{
	m_seed = s;
	TwAddVarRW(m_bar, "Seed", TW_TYPE_UINT32, &m_seed, "group=Procedural");
}
