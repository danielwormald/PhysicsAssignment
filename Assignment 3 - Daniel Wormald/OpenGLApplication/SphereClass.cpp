
#include "SphereClass.h"

SphereClass::SphereClass(float radius, float mass, vec4 colour, vec3 position, vec3 velocity) : DIYRigidBody(position, velocity, glm::quat(), mass)
{
	m_radius = radius;
	m_colour = colour;
	m_shapeId = ShapeType::SPHERE;

	m_type = PhysicsSceneType::DIY;
}

SphereClass::SphereClass(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, PxVec3 transform, float radius, vec4 colour, vec3 position, vec3 velocity) : DIYRigidBody(vec3(0, 0, 0), vec3(0, 0, 0), glm::quat(), 1.0f)
{
	m_radius = radius;
	m_colour = colour;
	m_dynamicActor = PxCreateDynamic(*physics, PxTransform(transform), PxSphereGeometry((PxReal)radius), *material, 2.0f);

	m_type = PhysicsSceneType::Phyx;

	physicsScene->addActor(*m_dynamicActor);
}

SphereClass::~SphereClass()
{
}

void SphereClass::Draw()
{
	glm::mat4 newRot;
	vec3 center;

	switch (m_type)
	{
	case Phyx:
		center = vec3(m_dynamicActor->getWorldBounds().getCenter().x, m_dynamicActor->getWorldBounds().getCenter().y, m_dynamicActor->getWorldBounds().getCenter().z);

		newRot = glm::mat4(mat4_cast(quat(m_dynamicActor->getGlobalPose().q.w, m_dynamicActor->getGlobalPose().q.x, m_dynamicActor->getGlobalPose().q.y, m_dynamicActor->getGlobalPose().q.z)));

		Gizmos::addSphere(center, m_radius, 25, 25, m_colour, &newRot);
		break;
	case DIY:
		Gizmos::addSphere(m_position, m_radius, 25, 25, m_colour);
		break;
	default:
		break;
	}

	/*vec3 center(m_dynamicActor->getWorldBounds().getCenter().x, m_dynamicActor->getWorldBounds().getCenter().y, m_dynamicActor->getWorldBounds().getCenter().z);
	glm::mat4 newRot(mat4_cast(quat(m_dynamicActor->getGlobalPose().q.w, m_dynamicActor->getGlobalPose().q.x, m_dynamicActor->getGlobalPose().q.y, m_dynamicActor->getGlobalPose().q.z)));*/
	//Gizmos::addSphere(center, m_radius, 25, 25, m_colour, &newRot);

	//Gizmos::addSphere(m_position, m_radius, 25, 25, m_colour);
}

void SphereClass::DrawPhysics()
{
	vec3 center(m_dynamicActor->getWorldBounds().getCenter().x, m_dynamicActor->getWorldBounds().getCenter().y, m_dynamicActor->getWorldBounds().getCenter().z);
	glm::mat4 newRot(mat4_cast(quat(m_dynamicActor->getGlobalPose().q.w, m_dynamicActor->getGlobalPose().q.x, m_dynamicActor->getGlobalPose().q.y, m_dynamicActor->getGlobalPose().q.z)));


	Gizmos::addSphere(center, m_radius, 25, 25, m_colour, &newRot);
}