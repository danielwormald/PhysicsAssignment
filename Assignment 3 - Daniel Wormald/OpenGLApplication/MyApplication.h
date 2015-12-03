

#ifndef MY_APPLICATION
#define MY_APPLICATION

#include "Application.h"
#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "FlyCamera.h"
#include "Object.h"	
#include "PhysicsObject.h"
#include "DIYRigidBody.h"
#include "Physics.h"
#include "DIYPhysicScene.h"
#include "Cube.h"
#include "Ball.h"
#include "SphereClass.h"
#include "Ragdoll.h"
#include "ParticleEmitter.h"
#include "ParticleFluidEmitter.h"
#include "Particle.h"
#include "PlayerController.h"
#include "MyControllerHitReport.h"
#include "MyCollisionCallback.h"
#include "TriggerBox.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera;
class Physics1;
class DIYPhysicScene;

enum PhysicsType
{
	PhysicsScene = 0,
	DIYPhysicsScene = 1
};

class MyApplication : public Application
{
public:
	virtual void StartUp();
	virtual void Update();
	virtual void Draw();
	virtual void ShutDown();

	void GenerateGrid(unsigned int rows, unsigned int cols);
		
	unsigned int m_programID;

	//Camera 
	Camera* m_pCamera;

	//Physics
	Physics1* m_physics;

	PhysicsType type;

	//DIYPhysics
	DIYPhysicScene* m_DIYPhysics;

private:
	float m_previousTime;
	
	int m_ogl_Load;
};


#endif



