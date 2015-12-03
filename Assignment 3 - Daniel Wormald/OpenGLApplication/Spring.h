#ifndef SPRING_H
#define	SPRING_H

#include "PhysicsObject.h"
#include "DIYRigidBody.h"
#include "glm\glm.hpp"

class Spring : public PhysicsObject {
public:
	Spring(DIYRigidBody* object1, DIYRigidBody* object2, float springCoefficient = 1, float distance = 0);

	virtual void Update(glm::vec3 _gravity, float _dt);
	virtual void Draw();

	DIYRigidBody* m_object1;
	DIYRigidBody* m_object2;

	float m_springCoefficient;
	float m_distance;
};

#endif