
#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec3 position, glm::vec3 extents, glm::vec3 velocity, float mass, glm::vec4 colour) : DIYRigidBody(position, velocity, glm::quat(), mass) {
	m_colour = colour;
	m_extents = extents;
	
	m_shapeId = ShapeType::BOX;
}

void Box::Draw() {
	Gizmos::addAABBFilled(m_position, m_extents, m_colour);
}