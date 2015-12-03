

#ifndef SPHERECLASS
#define SPHERECLASS
#include <PxPhysicsAPI.h>
#include <PxScene.h>
//#include "MyApplication.h"
#include "DIYRigidBody.h"
#include "glm\glm.hpp"
#include "Gizmos.h"

using namespace physx;
using namespace glm;
//class DIYRigidBody;

enum PhysicsSceneType
{
	Phyx = 0,
	DIY = 2
};

class SphereClass : public DIYRigidBody
{
public:
	SphereClass(float radius, float mass, vec4 colour, vec3 position, vec3 velocity);
	SphereClass(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, PxVec3 transform, float radius, vec4 colour, vec3 position, vec3 velocity);
	~SphereClass();

	//virtual void Update(glm::vec3 gravity, float dt);
	virtual void Draw();
	void DrawPhysics();

	PxRigidDynamic* m_dynamicActor;
	vec4 m_colour;
	float m_radius;

	PhysicsSceneType m_type;
private:
	
};

#endif