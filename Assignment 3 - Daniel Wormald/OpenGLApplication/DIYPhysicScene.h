

#ifndef PHYSICSCENE
#define PHYSICSCENE

//#include "PhysicsObject.h"
#include "glm\glm.hpp"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Plane.h"
#include "SphereClass.h"
#include "Box.h"
#include "FlyCamera.h"
#include "Spring.h"

class PhysicsObject;

class DIYPhysicScene
{
public:
	DIYPhysicScene();
	
	void Draw(Camera* camera);
	void AddActor(PhysicsObject*);
	void RemoveActor(PhysicsObject*);
	void Update(float _dt);
	void DebugScene();
	void AddGizmos();

	bool SphereToPlane(PhysicsObject*, PhysicsObject*);
	bool SphereToSphere(PhysicsObject*, PhysicsObject*);
	bool SphereToBox(PhysicsObject*, PhysicsObject*);
	bool BoxToBox(PhysicsObject*, PhysicsObject*);
	bool BoxToPlane(PhysicsObject*, PhysicsObject*);

	glm::vec3 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

	std::vector<SphereClass*> m_spheres;
	Box* m_box[5];
	Plane* m_plane;
	//Spring* m_spring;
};

#endif