#ifndef TRIGGER_BOX_H
#define	TRIGGER_BOX_H
#include "PxPhysicsAPI.h"
#include "MyApplication.h"

class TriggerBox : public physx::PxSimulationEventCallback {
public:
	TriggerBox(physx::PxScene* physicsScene, PxPhysics* g_Physics, PxMaterial* g_PhysicsMaterial, Physics1* physics1);
	~TriggerBox();

	void Update(double _dt);
	void Draw(Camera* camera);

	bool m_triggered;

private:
	physx::PxShape* m_shape;
	physx::PxRigidStatic* m_actor;
	Physics1* m_physics1;

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
	virtual void onWake(physx::PxActor**, physx::PxU32) {}
	virtual void onSleep(physx::PxActor**, physx::PxU32){}
};

#endif