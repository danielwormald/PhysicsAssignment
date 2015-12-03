#ifndef PLANE_H
#define	PLANE_H

#include "PhysicsObject.h"
#include "Gizmos.h"
//#include "MyApplication.h"

//class PhysicsObject;

class Plane : public PhysicsObject
{
public:
	Plane(glm::vec3 normal, float distance);

	glm::vec3 m_normal;
	float m_distance;

	virtual void Update(glm::vec3 gravity, float dt);
	virtual void Draw();
	virtual void MakeGizmo();
	
};

#endif