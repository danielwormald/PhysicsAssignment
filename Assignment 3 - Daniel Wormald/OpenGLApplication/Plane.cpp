
#include "Plane.h"

Plane::Plane(glm::vec3 normal, float distance) : PhysicsObject()
{
	m_normal = normal;
	m_distance = distance;

	m_shapeId = ShapeType::PLANE;
}

void Plane::Update(glm::vec3 gravity, float dt)
{

}

void Plane::Draw()
{
	glm::vec3 pos = m_normal * m_distance;
	Gizmos::addAABBFilled(pos, glm::vec3(100, 0.01f, 100), glm::vec4(0, 1, 0, 1));
}

void Plane::MakeGizmo()
{
	//float lineSegmentLength = 300;
	//glm::vec2 centrePoint = m_normal * m_distance;
	//glm::vec2 parallel = glm::vec2(m_normal.y, -m_normal.x); //easy to rotate normal through 90degrees around z
	//	glm::vec4 colour(1, 1, 1, 1);
	//glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	//glm::vec2 end = centrePoint - (parallel * lineSegmentLength);
	//Gizmos::add2DLine(start.xy(), end.xy(), colour);
}