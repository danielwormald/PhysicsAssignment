
#ifndef BOX_H
#define	BOX_H

#include "DIYRigidBody.h"
#include "glm\glm.hpp"

class Box : public DIYRigidBody
{
public:
	Box(glm::vec3 position, glm::vec3 extents, glm::vec3 velocity, float mass, glm::vec4 colour);
	virtual void Draw();

	glm::vec3 m_extents;
	glm::vec4 m_colour;
};

#endif