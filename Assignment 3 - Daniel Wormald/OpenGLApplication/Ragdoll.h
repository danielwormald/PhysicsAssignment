

#ifndef RAGDOLL_H
#define RAGDOLL_H

#include "MyApplication.h"

class Physics1;

const PxVec3 X_AXIS = PxVec3(1, 0, 0);
const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
const PxVec3 Z_AXIS = PxVec3(0, 0, 1);
enum RagDollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,
};

struct RagdollNode
{
	PxQuat globalRotation;
	PxVec3 scaledGobalPos;
	int parentNodeIdx;
	float halfLength;
	float radius;
	float parentLinkPos;
	float childLinkPos;
	char* name;
	PxArticulationLink* linkPtr;

	//constructor
	RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius, float _parentLinkPos, float _childLinkPos, char* _name)
	{
		globalRotation = _globalRotation, parentNodeIdx = _parentNodeIdx; halfLength = _halfLength; radius = _radius;
		parentLinkPos = _parentLinkPos; childLinkPos = _childLinkPos; name = _name;
	};
};

class Ragdoll
{
public:

	Ragdoll(PxPhysics* g_Physics, PxMaterial* ragdollMaterial, PxScene* physicsScene, Physics1* physics, Camera* camera);

	PxArticulation* MakeRagdoll(PxPhysics* g_Physics, RagdollNode** nodeArray, PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial);

	void Update(float dt, Camera* camera);

	void Draw();

private:
	PxArticulation* m_ragdoll;
	Physics1* m_physics;
	Camera* m_camera;

};

#endif