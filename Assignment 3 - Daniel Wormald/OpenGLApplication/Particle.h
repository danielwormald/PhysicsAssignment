

#ifndef PARTICLECLASS_H
#define PARTICLECLASS_H

#include "MyApplication.h"
#include <vector>

class ParticleClass
{
public:
	ParticleClass(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, Physics1* physics1);
	~ParticleClass();

	virtual void Update(float dt);
	virtual void Draw(Camera* camera);
	void AllowParticles() { m_allowParticles = true; }
private:

	//ParticleEmitter* m_particleEmitter;
	ParticleFluidEmitter* m_particleEmitter;
	PxRigidActor* m_rigidBody;
	vec3 m_extents;
	vec4 m_colour;
	bool m_static;
	bool m_allowParticles;

	Physics1* m_physics;

	std::vector<PxRigidActor*> m_actors;
};

#endif