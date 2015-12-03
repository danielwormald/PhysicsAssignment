#include "Physics.h"
Physics1::Physics1()
{
	m_allowProjectile = true;
}

void Physics1::Init(Camera* camera)
{
	SetUpPhysX();
	SetUpVisualDebugger();
	SetupTutorial(camera);
}

void Physics1::SetUpPhysX()
{
	PxAllocatorCallback *myCallback = new myAllocator();
	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation, PxTolerancesScale());
	PxInitExtensions(*g_Physics);
	g_PhysicsMaterial = g_Physics->createMaterial(0.5f, 0.5f, .5f);
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	//sceneDesc.filterShader = MyFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);

	/*PxSimulationEventCallback* mycollisionCallBack = new MyCollisionCallBack();
	g_PhysicsScene->setSimulationEventCallback(mycollisionCallBack);*/

	g_PhysicsScene = g_Physics->createScene(sceneDesc);}

//PxFilterFlags MyFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	// let triggers through
//	if (PxFilterObjectIsTrigger(attributes0) ||
//		PxFilterObjectIsTrigger(attributes1))
//	{
//		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
//		return PxFilterFlag::eDEFAULT;
//	}
//	// generate contacts for all that were not filtered above
//	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
//	// trigger the contact callback for pairs (A,B) where
//	// the filtermask of A contains the ID of B and vice versa.
//	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
//		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND |
//		PxPairFlag::eNOTIFY_TOUCH_LOST;
//
//	return PxFilterFlag::eDEFAULT;
//}
//helper function to set up filtering
void Physics1::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask; // word1 = ID mask to filter pairs that trigger a contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	_aligned_free(shapes);
}
void Physics1::SetUpVisualDebugger()
{
	if (g_Physics->getPvdConnectionManager() == NULL)
		return;
	const char* pvd_host_ip = "127.0.0.1";
	int port = 5425;
	unsigned int timeout = 100;
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	auto theConnection = PxVisualDebuggerExt::createConnection(g_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
}

void Physics1::SetupTutorial(Camera* camera)
{
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*plane);
		
	PxExtended a = PxExtended();
	PxExtendedVec3 pos = PxExtendedVec3(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	m_playerController = new PlayerController(g_PhysicsMaterial, g_Physics, g_PhysicsScene, pos, this);

	AddActors(camera);

	m_triggerBox = new TriggerBox(g_PhysicsScene, g_Physics, g_PhysicsMaterial, this);
}

void Physics1::AddActors(Camera* camera)
{
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			m_objects.push_back(new Cube(PxVec3(5 + (10 * i), 5 + (10 * j), -50), PxVec3(5, 5, 5), 2.0f, g_PhysicsMaterial, g_Physics, g_PhysicsScene, vec4(0.5f, 0.5f, 1, 1)));
		}
	}

	//m_objects.push_back(new SphereClass(g_PhysicsMaterial, g_Physics, g_PhysicsScene, PxVec3(-10, 10, 5), 2.0f, vec4(0.5f, 0.5f, 1, 1)));
	//m_objects.push_back(new SphereClass(g_PhysicsMaterial, g_Physics, g_PhysicsScene, PxVec3(10, 10, 5), 2.0f, vec4(0.5f, 0.5f, 1, 1)));

	m_ragdoll = new Ragdoll(g_Physics, g_PhysicsMaterial, g_PhysicsScene, this, camera);

	m_particle = new ParticleClass(g_PhysicsMaterial, g_Physics, g_PhysicsScene, this);
}

void Physics1::Update(float a_deltaTime, Camera* camera)
{
	//Draw Shader
	glClearColor(0, 0, 0, 1);
	Gizmos::clear();

	GLFWwindow* window = glfwGetCurrentContext();
	if (glfwGetKey(window, 'F') == GLFW_PRESS && m_allowProjectile)
	{
		glm::vec3 startingPosition(camera->getTransform()[3]);

		Ball *sphere = new Ball(PxVec3(startingPosition.x, startingPosition.y, startingPosition.z), PxVec3(1, 1, 1), 1200.0f, g_PhysicsMaterial, g_Physics, g_PhysicsScene, vec4(0.5f, 0.5f, 1, 1), 2.0f);

		glm::vec3 direction(camera->getTransform()[2]);
		physx::PxVec3 velocity = physx::PxVec3(-direction.x, -direction.y, -direction.z) * 50;

		sphere->SetLinearVelocity(velocity);

		m_objects.push_back(sphere);

		m_allowProjectile = false;

		/*glm::vec3 startingPosition(camera->getTransform()[3]);

		Cube* cube = new Cube(PxVec3(startingPosition.x, startingPosition.y, startingPosition.z), PxVec3(1, 1, 1), 1200.0f, g_PhysicsMaterial, g_Physics, g_PhysicsScene, vec4(0.5f, 0.5f, 1, 1));

		glm::vec3 direction(camera->getTransform()[2]);
		physx::PxVec3 velocity = physx::PxVec3(-direction.x, -direction.y, -direction.z) * 50;

		cube->SetLinearVelocity(velocity);

		m_objects.push_back(cube);

		m_allowProjectile = false;*/
	}

	if (glfwGetKey(window, 'F') == GLFW_RELEASE)
		m_allowProjectile = true;

	for (auto actor : m_objects)
	{
		actor->Update(a_deltaTime);
	}

	m_ragdoll->Update(a_deltaTime, camera);

	m_particle->Update(a_deltaTime);

	m_playerController->Update(a_deltaTime);

	if (m_triggerBox->m_triggered) 
	{
		m_particle->AllowParticles();
	}
	m_triggerBox->Update(a_deltaTime);

	//Update Physx LAST
	UpdatePhysx(a_deltaTime);
}

void Physics1::AddWidget(PxShape* shape, PxRigidActor* actor, Camera* camera)
{
	PxGeometryType::Enum type = shape->getGeometryType();
	switch (type)
	{
	case PxGeometryType::eBOX:
		AddBox(shape, actor);
	case PxGeometryType::eSPHERE:
		AddSphere(shape, actor, camera);
	case PxGeometryType::ePLANE:
		AddPlane(shape, actor, camera);
	case PxGeometryType::eCAPSULE:
		AddCapsule(shape, actor, camera);
		break;
	}
}

void Physics1::AddBox(PxShape* pShape, PxRigidActor* actor)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = pShape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(0.87, 0.72, 0.52, 1);
	//if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
	//{
	//	colour = glm::vec4(0, 1, 0, 1);a
	//}
	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void Physics1::AddSphere(PxShape* pShape, PxRigidActor* actor, Camera* camera)
{
	//get the geometry for this PhysX collision volume
	PxSphereGeometry geometry;
	float radius = 1;
	//get the geometry for this PhysX collision volume
	bool status = pShape->getSphereGeometry(geometry);
	if (status)
	{
		radius = geometry.radius;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	glm::vec4 colour = glm::vec4(0.87, 0.72, 0.52, 1);

	Gizmos::addSphere(position, radius, 5, 5, colour, &M);
}

void Physics1::AddCapsule(PxShape* pShape, PxRigidActor* actor, Camera* camera)
{
	PxCapsuleGeometry geometry;
	float radius = 1;
	float halfHeight = 1;

	bool status = pShape->getCapsuleGeometry(geometry);
	if (status)
	{
		radius = geometry.radius; //copy out capsule radius
		halfHeight = geometry.halfHeight; //copy out capsule half length
	}
	PxTransform transform = PxShapeExt::getGlobalPose(*pShape, *actor);

	PxMat44 m(transform); //Create a rotation matrix from transform
	glm::mat4* M = (glm::mat4*)(&m);
	//get the world position from the PhysX tranform
	glm::vec3 position = vec3(transform.p.x, transform.p.y, transform.p.z);  //Px2GlV3(transform.p);
	glm::vec4 axis(halfHeight, 0, 0, 0); //axis for the capsule
	axis = *M * axis; //rotate axis to correct orientation
	//add our 2 end cap spheres...

	glm::vec4 colour = glm::vec4(1, 0.2, 0.2, 1);

	Gizmos::addSphere(position + axis.xyz(), radius, 10, 10, colour);
	Gizmos::addSphere(position - axis.xyz(), radius, 10, 10, colour);
	//Fix the gizmo rotation
	glm::mat4 m2 = glm::rotate(*M, 11 / 7.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	Gizmos::addCylinderFilled(position, radius, halfHeight, 10, colour, &m2);}

void Physics1::AddPlane(PxShape* pShape, PxRigidActor* actor, Camera* camera)
{
	//get the geometry for this PhysX collision volume
	PxBoxGeometry geometry;
	float width = 1, height = 1, length = 1;
	bool status = pShape->getBoxGeometry(geometry);
	if (status)
	{
		width = geometry.halfExtents.x;
		height = geometry.halfExtents.y;
		length = geometry.halfExtents.z;
	}
	//get the transform for this PhysX collision volume
	PxMat44 m(PxShapeExt::getGlobalPose(*pShape, *actor));
	glm::mat4 M(m.column0.x, m.column0.y, m.column0.z, m.column0.w,
		m.column1.x, m.column1.y, m.column1.z, m.column1.w,
		m.column2.x, m.column2.y, m.column2.z, m.column2.w,
		m.column3.x, m.column3.y, m.column3.z, m.column3.w);
	glm::vec3 position;
	//get the position out of the transform
	position.x = m.getPosition().x;
	position.y = m.getPosition().y;
	position.z = m.getPosition().z;
	glm::vec3 extents = glm::vec3(width, height, length);
	glm::vec4 colour = glm::vec4(0.87, 0.72, 0.52, 1);
	//if (actor->getName() != NULL && strcmp(actor->getName(), "Pickup1")) //seriously horrid hack so I can show pickups a different colour
	//{
	//	colour = glm::vec4(0, 1, 0, 1);a
	//}
	//create our box gizmo
	Gizmos::addAABBFilled(position, extents, colour, &M);
}

void Physics1::UpdatePhysx(float a_deltaTime)
{
	if (a_deltaTime <= 0)
	{
		return;
	}
	g_PhysicsScene->simulate((physx::PxReal)a_deltaTime);

	while (g_PhysicsScene->fetchResults() == false)
	{
		// don’t need to do anything here yet but we have to fetch results
	}

	//g_PhysicsScene->
}

void Physics1::Draw(Camera* camera)
{
	for (auto actor : m_objects)
	{
		{
			actor->Draw();
		}
	}

	m_ragdoll->Draw();

	m_particle->Draw(camera);

	m_playerController->Draw(camera);

	Gizmos::addAABBFilled(vec3(0, -0.1, 0), vec3(500, 0.1, 500), vec4(1, 0.72, 0.52, 1));

	Gizmos::addAABBFilled(vec3(-20, 0, -10), vec3(5, 0.1, 5), vec4(1, 0.2, 0.2, 1));

	Gizmos::draw(camera->getProjectionView());
}

void Physics1::CleanUpdate()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
}