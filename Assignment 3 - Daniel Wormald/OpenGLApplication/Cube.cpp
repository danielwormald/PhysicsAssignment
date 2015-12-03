
#include "Cube.h"

Cube::Cube(PxVec3 transform, PxVec3 extents, float density, PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, vec4 colour, bool isProjectile) {
	m_colour = colour;
	Physics1::ConvertVec3(extents, m_extents);
	
	m_rigidBody = PxCreateDynamic(*physics, PxTransform(transform), PxBoxGeometry(extents), *material, density);

	physicsScene->addActor(*m_rigidBody);
}

Cube::~Cube() {

}

void Cube::SetLinearVelocity(PxVec3 vel)
{
	m_rigidBody->setLinearVelocity(vel, true);
}

void Cube::Update(float dt) {

}

void Cube::Draw(){
	vec3 center(m_rigidBody->getWorldBounds().getCenter().x, m_rigidBody->getWorldBounds().getCenter().y, m_rigidBody->getWorldBounds().getCenter().z);
	glm::mat4 newRot(mat4_cast(quat(m_rigidBody->getGlobalPose().q.w, m_rigidBody->getGlobalPose().q.x, m_rigidBody->getGlobalPose().q.y, m_rigidBody->getGlobalPose().q.z)));

	Gizmos::addAABBFilled(center, m_extents, m_colour, &newRot);
}