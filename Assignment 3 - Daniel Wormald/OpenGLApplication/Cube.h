#ifndef CUBE_H
#define	CUBE_H
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include "MyApplication.h"

using namespace physx;
using namespace glm;
class Cube : public ObjectClass
{
public:
	Cube(PxVec3 transform, PxVec3 extents, float density, PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, vec4 colour, bool isStatic = false);
	~Cube();

	virtual void Update(float dt);

	virtual void Draw();

	void SetLinearVelocity(PxVec3 vel);
private:

	PxRigidDynamic* m_rigidBody;
	vec3 m_extents;
	vec4 m_colour;
	bool m_static;
};

#endif