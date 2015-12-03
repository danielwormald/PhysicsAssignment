


#ifndef DIYRIGID_BODY
#define DIYRIGID_BODY

#include "PhysicsObject.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

//#include "MyApplication.h"

//class PhysicsObject;

class DIYRigidBody : public PhysicsObject
{
public:
	
	DIYRigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass);

	virtual void Update(glm::vec3 gravity, float timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec3 force);
	void ApplyForceToActor(DIYRigidBody* actor2, glm::vec2 force);

	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_mass;
	glm::quat m_rotation;
	bool m_static;
};

#endif