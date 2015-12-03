
#include "MyCollisionCallback.h"
#include <iostream>

using namespace std;

void MyCollisionCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		//only interested in touches found and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			cout << "Collision Detected between: ";
			cout << pairHeader.actors[0]->getName();
			cout << pairHeader.actors[1]->getName() << endl;
		}
	}
}

void MyCollisionCallBack::onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		PxTriggerPair* pair = pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;
		cout << otherActor->getName();
		cout << " Entered Trigger ";
		cout << triggerActor->getName() << endl;
	}
};