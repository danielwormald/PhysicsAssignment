

#include "PlayerController.h"

PlayerController::PlayerController(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, PxExtendedVec3 startingPosition, Physics1* physics1)
{
	m_physics = physics1;
	myHitReport = new MyControllerHitReport();
	gCharacterManager = PxCreateControllerManager(*physicsScene);
	//describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 0, 0);
	desc.material = material;
	desc.reportCallback = myHitReport; //connect it to our collision detection routine
	desc.density = 10;
	//create the layer controller
	gPlayerController = gCharacterManager->createController(desc);
	gPlayerController->setPosition(startingPosition);
	//set up some variables to control our player with
	_characterYVelocity = 0; //initialize character velocity
	_characterRotation = 0; //and rotation
	_playerGravity = -0.5f; //set up the player gravity
	myHitReport->clearPlayerContactNormal(); //initialize the contact normal (what we are in contact with)

	physicsScene->addActor(*gPlayerController->getActor());
}

PlayerController::~PlayerController()
{

}

void PlayerController::Update(float dt)
{
	bool onGround; //set to true if we are on the ground
	float movementSpeed = 10.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
	//check if we have a contact normal. if y is greater than .3 we assume this is solid ground.This is a rather primitive way to do this.Can you do better ?
	if (myHitReport->getPlayerContactNormal().y > 0.3f)
	{
		_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		_characterYVelocity += _playerGravity * dt;
		onGround = false;
	}
	myHitReport->clearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	//scan the keys and set up our intended velocity based on a global transform
	PxVec3 velocity(0, _characterYVelocity, 0);

	GLFWwindow* window = glfwGetCurrentContext();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		velocity.x -= movementSpeed*dt;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		velocity.x += movementSpeed*dt;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		velocity.z += movementSpeed*dt;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		velocity.z -= movementSpeed*dt;
	}
	//To do.. add code to control z movement and jumping
	float minDistance = 0.001f;
	PxControllerFilters filter;
	//make controls relative to player facing
	PxQuat rotation(_characterRotation, PxVec3(0, 1, 0));
	//velocity = PxVec3(0, _characterYVelocity, 0);
	//move the controller
	gPlayerController->move(rotation.rotate(velocity), minDistance, dt,	filter);
}

void PlayerController::Draw(Camera* camera)
{
	PxU32 nShapes = gPlayerController->getActor()->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	gPlayerController->getActor()->getShapes(shapes, nShapes);
	while (nShapes--)
	{
		m_physics->AddWidget(shapes[nShapes], gPlayerController->getActor(), camera);
	}
}