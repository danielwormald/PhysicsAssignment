
#include "Ball.h"

Ball::Ball(PxVec3 transform, PxVec3 extents, float density, PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, vec4 colour, float radius, bool isProjectile) {
	m_colour = colour;
	m_radius = radius;

	Physics1::ConvertVec3(extents, m_extents);

	m_rigidBody = PxCreateDynamic(*physics, PxTransform(transform), PxSphereGeometry((PxReal)radius), *material, density);

	physicsScene->addActor(*m_rigidBody);
}

Ball::~Ball() {

}

void Ball::SetLinearVelocity(PxVec3 vel)
{
	m_rigidBody->setLinearVelocity(vel, true);
}

void Ball::Update(float dt) {

}

void Ball::Draw(){
	/*vec3 center(m_rigidBody->getWorldBounds().getCenter().x, m_rigidBody->getWorldBounds().getCenter().y, m_rigidBody->getWorldBounds().getCenter().z);
	glm::mat4 newRot(mat4_cast(quat(m_rigidBody->getGlobalPose().q.w, m_rigidBody->getGlobalPose().q.x, m_rigidBody->getGlobalPose().q.y, m_rigidBody->getGlobalPose().q.z)));

	Gizmos::addAABBFilled(center, m_extents, m_colour, &newRot);*/

	vec3 center(m_rigidBody->getWorldBounds().getCenter().x, m_rigidBody->getWorldBounds().getCenter().y, m_rigidBody->getWorldBounds().getCenter().z);
	glm::mat4 newRot(mat4_cast(quat(m_rigidBody->getGlobalPose().q.w, m_rigidBody->getGlobalPose().q.x, m_rigidBody->getGlobalPose().q.y, m_rigidBody->getGlobalPose().q.z)));


	Gizmos::addSphere(center, m_radius, 25, 25, m_colour, &newRot);
}