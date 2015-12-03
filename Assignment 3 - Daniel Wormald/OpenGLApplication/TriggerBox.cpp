#include "TriggerBox.h"
//#include "Utils.h"
#include "Gizmos.h"
using namespace physx;
TriggerBox::TriggerBox(PxScene* physicsScene, PxPhysics* g_Physics, PxMaterial* g_PhysicsMaterial, Physics1* physics1) 
{
	m_actor = PxCreateStatic(*g_Physics, PxTransform(PxVec3(-20, 5, -10)), PxBoxGeometry(PxVec3(0.1f, 0.1f, 0.1f)), *g_PhysicsMaterial);
	physicsScene->addActor(*m_actor);

	PxShape* shape = g_Physics->createShape(PxBoxGeometry(5, 5, 5), *g_PhysicsMaterial);

	m_shape = shape;
	m_shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	m_shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	m_actor->attachShape(*shape);
	physicsScene->setSimulationEventCallback(this);
	m_triggered = false;
	m_physics1 = physics1;
}

TriggerBox::~TriggerBox()
{

}

void TriggerBox::Update(double _dt) 
{
	m_triggered = false;
}

void TriggerBox::Draw(Camera* camera) 
{
	m_physics1->AddWidget(m_shape, m_actor, camera);
}

void TriggerBox::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) 
{
	m_triggered = true;
}

void TriggerBox::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	m_triggered = true;
}