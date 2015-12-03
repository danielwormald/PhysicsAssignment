

#include "DIYRigidBody.h"

DIYRigidBody::DIYRigidBody(glm::vec3 position, glm::vec3 velocity, glm::quat rotation, float mass)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_rotation = rotation;
	m_static = false;
}

void DIYRigidBody::Update(glm::vec3 gravity, float timeStep)
{
	if (!m_static) {
		m_velocity += gravity;
		/*m_velocity.x *= 0.99f;
		m_velocity.y *= 0.99f;
		m_velocity.z *= 0.99f;*/

		m_position += m_velocity * timeStep;
	}
}

void DIYRigidBody::Debug()
{

}

void DIYRigidBody::ApplyForce(glm::vec3 force)
{
	m_velocity += force / m_mass;
}

void DIYRigidBody::ApplyForceToActor(DIYRigidBody* actor2, glm::vec2 force)
{
	
}