

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "MyApplication.h"

class MyControllerHitReport;

class PlayerController
{
public:

	PlayerController(PxMaterial* material, PxPhysics* physics, PxScene* physicsScene, PxExtendedVec3 startingPosition, Physics1* physics1);
	~PlayerController();
	void Update(float dt);
	void Draw(Camera* camera);

	PxControllerManager* gCharacterManager;
	PxController* gPlayerController;
	MyControllerHitReport* myHitReport;

private:

	float _characterYVelocity; //initialize character velocity
	float _characterRotation; //and rotation
	float _playerGravity; //set up the player gravity
	Physics1* m_physics;

};

#endif