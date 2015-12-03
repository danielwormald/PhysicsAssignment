
#include "Spring.h"
#include "Gizmos.h"

Spring::Spring(DIYRigidBody* object1, DIYRigidBody* object2, float springCoefficient, float distance) : PhysicsObject(){
	m_object1 = object1;
	m_object2 = object2;
	m_springCoefficient = springCoefficient;
	m_distance = distance;
}

void Spring::Update(glm::vec3 _gravity, float _dt) {

	glm::vec3 direction = m_object1->m_position - m_object2->m_position;
	direction -= glm::normalize(direction) * m_distance;
	direction *= -m_springCoefficient;
	m_object1->ApplyForce(direction);
	m_object2->ApplyForce(-direction);
}

void Spring::Draw() {
	Gizmos::addLine(m_object1->m_position, m_object2->m_position, glm::vec4(1, 1, 1, 1));
}