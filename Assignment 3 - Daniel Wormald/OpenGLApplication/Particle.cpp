
#include "Particle.h"

ParticleClass::ParticleClass(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, Physics1* physics1)
{
	m_physics = physics1;
	
	//PxParticleSystem* pf;
	PxParticleFluid* pf;
	// create particle system in PhysX SDK
	// set immutable properties.
	PxU32 maxParticles = 500;
	bool perParticleRestOffset = false;
	//pf = physics->createParticleSystem(maxParticles, perParticleRestOffset);
	pf = physics->createParticleFluid(maxParticles, perParticleRestOffset);
	pf->setRestParticleDistance(.3f);
	pf->setDynamicFriction(0.1);
	pf->setStaticFriction(0.1);
	pf->setDamping(0.1);
	pf->setParticleMass(1);
	pf->setRestitution(0);
	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, false);

	if (pf)
	{
		physicsScene->addActor(*pf);
		//m_particleEmitter = new ParticleEmitter(maxParticles, PxVec3(0, 10, 0), pf, .01);
		m_particleEmitter = new ParticleFluidEmitter(maxParticles, PxVec3(0, 10, 0), pf, 0.01f);
		//m_particleEmitter->setStartVelocityRange(-2.0f, 0, -2.0f, 2.0f, 0.0f, 2.0f);
		m_particleEmitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f, 0.001f, -250.0f, 0.001f);
	}

	/*PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*physics, pose, PxPlaneGeometry(), *material);
	const PxU32 numShapes = plane->getNbShapes();
	physicsScene->addActor(*plane);*/
	PxBoxGeometry side1(4.5, 1, .5);
	PxBoxGeometry side2(.5, 1, 4.5);
	PxTransform pose;
	pose = PxTransform(PxVec3(0.0f, 0.5, 4.0f));
	PxRigidStatic* box = PxCreateStatic(*physics, pose, side1, *material);
	physicsScene->addActor(*box);
	m_actors.push_back(box);

	pose = PxTransform(PxVec3(0.0f, 0.5, -4.0f));
	box = PxCreateStatic(*physics, pose, side1, *material);
	physicsScene->addActor(*box);
	m_actors.push_back(box);

	pose = PxTransform(PxVec3(4.0f, 0.5, 0));
	box = PxCreateStatic(*physics, pose, side2, *material);
	
	physicsScene->addActor(*box);
	m_actors.push_back(box);

	pose = PxTransform(PxVec3(-4.0f, 0.5, 0));
	box = PxCreateStatic(*physics, pose, side2, *material);
	physicsScene->addActor(*box);
	m_actors.push_back(box);

	m_allowParticles = false;
}

ParticleClass::~ParticleClass()
{

}

void ParticleClass::Update(float dt)
{
	if (m_particleEmitter && m_allowParticles)
	{
		m_particleEmitter->update(dt);
		//render all our particles
		m_particleEmitter->renderParticles();
	}
}

void ParticleClass::Draw(Camera* camera)
{
	if (m_allowParticles)
	{
		for (auto actor : m_actors)
		{
			{
				PxU32 nShapes = actor->getNbShapes();
				PxShape** shapes = new PxShape*[nShapes];
				actor->getShapes(shapes, nShapes);
				// Render all the shapes in the physx actor (for early tutorials there is just one)
				while (nShapes--)
				{
					m_physics->AddWidget(shapes[nShapes], actor, camera);
				}
				delete[] shapes;
			}
		}
	}
}

