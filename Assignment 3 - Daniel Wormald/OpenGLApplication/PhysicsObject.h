


#ifndef PHYSICS_OBJECT
#define PHYSICS_OBJECT

//#include "MyApplication.h"
#include "glm\glm.hpp"

enum ShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
};

class PhysicsObject
{
public:

	ShapeType m_shapeId;

	void virtual Update(glm::vec3 gravity, float timeStep) = 0;	//void virtual Debug() = 0;
	void virtual Draw() = 0;
	//void virtual MakeGizmo() = 0;
	void virtual ResetPosition(){};

private:
};

#endif