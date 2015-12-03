
#ifndef MY_COLLISION_CALLBACK_H
#define MY_COLLISION_CALLBACK_H

#include "MyApplication.h"

class MyCollisionCallBack : public PxSimulationEventCallback
{
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32){};
	virtual void onWake(PxActor**, PxU32){};
	virtual void onSleep(PxActor**, PxU32){};
};

#endif