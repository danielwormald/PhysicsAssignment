#include "MyApplication.h"

void MyApplication::StartUp()
{
	//m_shaderManager = new ShaderManager();

	m_previousTime = 0.0f;

	Gizmos::create();

	type = PhysicsType::PhysicsScene;

	m_pCamera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);
	//m_pCamera->setLookAtFrom(vec3(0, 10, 10), vec3(0));
	m_pCamera->setLookAtFrom(vec3(0, 10, 20), vec3(0, 5, 0));

	//m_shader = new Shader(m_shaderManager, m_pCamera, window);

	m_physics = new Physics1();
	m_physics->Init(m_pCamera);



}

void MyApplication::Update()
{
	float currentTime = (float)glfwGetTime();
	float deltaTime = currentTime - m_previousTime;
	m_previousTime = currentTime;

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Gizmos::clear();

	if (glfwGetKey(window, '1') == GLFW_PRESS)
	{
		m_DIYPhysics = nullptr;

		m_pCamera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);
		m_pCamera->setLookAtFrom(vec3(10, 100, 100), vec3(0, 0, 0));

		m_physics = new Physics1();
		m_physics->Init(m_pCamera);

		type = PhysicsType::PhysicsScene;
	}
	if (glfwGetKey(window, '2') == GLFW_PRESS)
	{
		m_physics = nullptr;

		m_pCamera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 1000.0f);
		m_pCamera->setLookAtFrom(vec3(0, 30, 60), vec3(0));

		m_DIYPhysics = new DIYPhysicScene();

		type = PhysicsType::DIYPhysicsScene;
	}

	//Camera
	m_pCamera->update(deltaTime);

	switch (type)
	{
	case PhysicsScene:
		//Physics
		m_physics->Update(deltaTime, m_pCamera);
		break;
	case DIYPhysicsScene:
		m_DIYPhysics->Update(deltaTime);
		break;
	default:
		break;
	}
}

void MyApplication::Draw()
{
	switch (type)
	{
	case PhysicsScene:
		m_physics->Draw(m_pCamera);
		break;
	case DIYPhysicsScene:
		m_DIYPhysics->Draw(m_pCamera);
		break;
	default:
		break;
	}
}

void MyApplication::ShutDown()
{
	Gizmos::destroy();

	if (m_physics != nullptr)
		m_physics->CleanUpdate();

	delete m_pCamera;
	delete m_DIYPhysics;
}
