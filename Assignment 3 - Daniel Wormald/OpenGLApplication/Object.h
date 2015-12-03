

#ifndef OBJECT_H
#define OBJECT_H

class ObjectClass
{
public:

	//virtual ObjectClass() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};

#endif