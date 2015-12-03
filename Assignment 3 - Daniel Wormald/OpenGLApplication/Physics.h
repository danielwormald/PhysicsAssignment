#ifndef PHYSICS_H
#define PHYSICS_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include "MyApplication.h"
#include "DIYPhysicScene.h"
#include "Plane.h"
#include <vector>

using namespace physx;
using namespace glm;

class DIYPhysicScene;
class ObjectClass;
class Ragdoll;
class ParticleClass;
class PlayerController;
class TriggerBox;

class Physics1
{
public:

	Physics1();

	~Physics1();

	void Init(Camera* camera);

	void SetUpPhysX();

	void SetUpVisualDebugger();

	void SetupTutorial(Camera* camera);

	void Update(float a_deltaTime, Camera* camera);

	void UpdatePhysx(float a_deltaTime);

	void AddWidget(PxShape* shape, PxRigidActor* actor, Camera* camera);

	void AddBox(PxShape* pShape, PxRigidActor* actor);

	void AddSphere(PxShape* pShape, PxRigidActor* actor, Camera* camera);

	void AddCapsule(PxShape* pShape, PxRigidActor* actor, Camera* camera);

	void AddPlane(PxShape* pShape, PxRigidActor* actor, Camera* camera);

	void AddActors(Camera* camera);

	static glm::mat4 ConvertToMat4(PxTransform _transform) {
		return(mat4_cast(quat(_transform.q.w, _transform.q.x, _transform.q.y, _transform.q.z)));
	}

	static void ConvertVec3(PxVec3 _in, vec3& _out) {
		_out = vec3(_in.x, _in.y, _in.z);
	}

	static void ConvertVec3(vec3 _in, PxVec3& _out) {
		_out = PxVec3(_in.x, _in.y, _in.z);
	}

	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);

	/*PxFilterFlags MyFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);*/
	
	void Draw(Camera* camera);

	void CleanUpdate();

	DIYPhysicScene* m_physicsScene;
		
	std::vector<ObjectClass*> m_objects;

	Ragdoll* m_ragdoll;

	ParticleClass* m_particle;

	PlayerController* m_playerController;

	TriggerBox* m_triggerBox;

private:

	PxFoundation* g_PhysicsFoundation;
	PxPhysics* g_Physics;
	PxScene* g_PhysicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;	PxMaterial* g_PhysicsMaterial;
	PxMaterial* g_boxMaterial;
	PxCooking* g_PhysicsCooker;

	std::vector<PxRigidActor*> g_PhysXActors;	bool m_allowProjectile;};



class myAllocator : public PxAllocatorCallback
{
public:
	virtual ~myAllocator() {}
	virtual void* allocate(size_t size,const char* typeName,const char* filename,int line)
	{
		void* pointer = _aligned_malloc(size, 16);
		return pointer;
	}
	virtual void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}
};

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};

#endif