
#include "DIYPhysicScene.h"

DIYPhysicScene::DIYPhysicScene()
{
	m_gravity = glm::vec3(0, -9.8, 0);
	m_timeStep = 0.001f;

	for (int i = 1; i <= 11; i++)
	{
		m_spheres.push_back(new SphereClass(i * 0.5f, 1, vec4(1, 0, 1, 1), vec3(-50 + (5 * i), 10 * i, 0), vec3(0, 0, 0)));

		AddActor(m_spheres[i - 1]);
	}

	m_box[0] = new Box(vec3(-100, 10, 0), vec3(2, 10, 102), vec3(0), 1000, vec4(0.87, 0.72, 0.52, 1));
	m_box[1] = new Box(vec3(0, 10, -100), vec3(102, 10, 2), vec3(0), 1000, vec4(0.87, 0.72, 0.52, 1));
	m_box[2] = new Box(vec3(100, 10, 0), vec3(2, 10, 102), vec3(0), 1000, vec4(0.87, 0.72, 0.52, 1));
	m_box[3] = new Box(vec3(0, 10, 100), vec3(102, 10, 2), vec3(0), 1000, vec4(0.87, 0.72, 0.52, 1));
	m_box[4] = new Box(vec3(0, 30, 0), vec3(5, 5, 5), vec3(0), 1000, vec4(0.87, 0.72, 0.52, 1));

	for (int i = 0; i < 5; i++)
	{
		if (i <= 3)
			m_box[i]->m_static = true;

		AddActor(m_box[i]);
	}

	AddActor(new Plane(vec3(0, 1, 0), 0));

	AddActor(new Spring(Spring(m_spheres[9], m_spheres[10], 0.1f)));
}

void DIYPhysicScene::AddActor(PhysicsObject* object)
{
	m_actors.push_back(object);
}

void DIYPhysicScene::RemoveActor(PhysicsObject* object)
{
	auto item = std::find(m_actors.begin(), m_actors.end(), object);
	if (item < m_actors.end())
	{
		m_actors.erase(item);
	}
}

void DIYPhysicScene::Update(float _dt)
{
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it)
	{
		(*it)->Update(m_gravity, m_timeStep);

		if ((*it)->m_shapeId == ShapeType::SPHERE)
		{
			for (auto it2 = m_actors.begin(); it2 != m_actors.end(); ++it2)
			{
				if ((*it) != (*it2)) {
					if ((*it2)->m_shapeId == ShapeType::PLANE)
					{
						SphereToPlane((*it), (*it2));
					}
					if ((*it2)->m_shapeId == ShapeType::SPHERE)
					{
						SphereToSphere((*it), (*it2));
					}
					if ((*it2)->m_shapeId == ShapeType::BOX)
					{
						SphereToBox((*it), (*it2));
					}
				}
			}
		}
		else if ((*it)->m_shapeId == ShapeType::BOX)
		{
			for (auto it2 = m_actors.begin(); it2 != m_actors.end(); ++it2)
			{
				if ((*it) != (*it2)) {
					if ((*it2)->m_shapeId == ShapeType::BOX)
					{
						BoxToBox((*it), (*it2));
					}
					if ((*it2)->m_shapeId == ShapeType::PLANE)
					{
						BoxToPlane((*it), (*it2));
					}
				}
			}
		}
	}
}

bool DIYPhysicScene::SphereToSphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	SphereClass* sphere1 = dynamic_cast<SphereClass*>(obj1);
	SphereClass* sphere2 = dynamic_cast<SphereClass*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr) {
		float dist = glm::distance(sphere1->m_position, sphere2->m_position);
		float intersect = sphere1->m_radius + sphere2->m_radius - dist;

		if (intersect > 0) {
			glm::vec3 delta = sphere1->m_position - sphere2->m_position;
			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = sphere1->m_velocity - sphere2->m_velocity;
			glm::vec3 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
			glm::vec3 forceVector = collisionVector * 1.0f / (1 / sphere1->m_mass + 1 / sphere2->m_mass);

			sphere1->ApplyForce(2.0f * -forceVector);
			sphere2->ApplyForce(2.0f * forceVector);

			float totalMass = sphere1->m_mass + sphere2->m_mass;

			glm::vec3 seperationVector = collisionNormal * intersect;
			sphere1->m_position += (seperationVector * (sphere2->m_mass / totalMass));
			sphere2->m_position -= (seperationVector * (sphere1->m_mass / totalMass));

			return true;
		}
	}
	return false;
}

bool DIYPhysicScene::SphereToPlane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	SphereClass *sphere = dynamic_cast<SphereClass*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr) {
		glm::vec3 collisionNormal = plane->m_normal;
		float sphereToPlane = glm::dot(sphere->m_position, plane->m_normal) - plane->m_distance;

		float intersection = sphere->m_radius - sphereToPlane;

		if (intersection > 0) {
			glm::vec3 relativeVelocity = sphere->m_velocity;
			glm::vec3 planeNormal = collisionNormal;
			glm::vec3 forceVector = -1 * sphere->m_mass * planeNormal * (glm::dot(planeNormal, sphere->m_velocity));

			sphere->ApplyForce(forceVector * glm::vec3(2));
			sphere->m_position += collisionNormal * intersection;

			return true;
		}
	}

	return false;
}

bool DIYPhysicScene::BoxToPlane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box *box = dynamic_cast<Box*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (box->m_static == true)
		return false;

	if (box != nullptr && plane != nullptr) {
		glm::vec3 collisionNormal = plane->m_normal;
		float sphereToPlane = glm::dot(box->m_position, plane->m_normal) - plane->m_distance;

		float intersection = box->m_extents.x/2 - sphereToPlane;

		if (intersection > 0) {
			glm::vec3 relativeVelocity = box->m_velocity;
			glm::vec3 planeNormal = collisionNormal;
			glm::vec3 forceVector = -1 * box->m_mass * planeNormal * (glm::dot(planeNormal, box->m_velocity));

			box->ApplyForce(forceVector * glm::vec3(2));
			box->m_position += collisionNormal * intersection;

			return true;
		}
	}

	return false;
}

bool DIYPhysicScene::SphereToBox(PhysicsObject* _actor1, PhysicsObject* _actor2) {
	SphereClass* sphere = dynamic_cast<SphereClass*>(_actor1);
	Box* box = dynamic_cast<Box*>(_actor2);

	float boxExtentsX = box->m_extents.x;
	float boxExtentsY = box->m_extents.y;
	float boxExtentsZ = box->m_extents.z;

	glm::vec3 TopBackLeft = box->m_position - glm::vec3(boxExtentsX, boxExtentsY, -boxExtentsZ);
	glm::vec3 TopBackRight = box->m_position - glm::vec3(-boxExtentsX, boxExtentsY, -boxExtentsZ);
	glm::vec3 TopFrontLeft = box->m_position - glm::vec3(boxExtentsX, boxExtentsY, boxExtentsZ);
	glm::vec3 TopFrontRight = box->m_position - glm::vec3(-boxExtentsX, boxExtentsY, boxExtentsZ);

	glm::vec3 BottomBackLeft = box->m_position - glm::vec3(boxExtentsX, -boxExtentsY, -boxExtentsZ);
	glm::vec3 BottomBackRight = box->m_position - glm::vec3(-boxExtentsX, -boxExtentsY, -boxExtentsZ);
	glm::vec3 BottomFrontLeft = box->m_position - glm::vec3(boxExtentsX, -boxExtentsY, boxExtentsZ);
	glm::vec3 BottomFrontRight = box->m_position - glm::vec3(-boxExtentsX, -boxExtentsY, boxExtentsZ);

	float fIntersectDist;
	glm::vec3 collisionNormal;
	float fXSmallestOverlap = 10000000;
	float fYSmallestOverlap = 10000000;
	float fZSmallestOverlap = 10000000;
	float fSmallestOverlap = 10000000;

	float fCubeMinX = BottomFrontLeft.x;
	float fCubeMaxX = BottomFrontRight.x;

	float fSphereMinX = sphere->m_position.x - sphere->m_radius;
	float fSphereMaxX = sphere->m_position.x + sphere->m_radius;

	float fCubeMinY = TopFrontLeft.y;
	float fCubeMaxY = BottomBackRight.y;

	float fSphereMinY = sphere->m_position.y - sphere->m_radius;
	float fSphereMaxY = sphere->m_position.y + sphere->m_radius;

	float fCubeMinZ = BottomFrontLeft.z;
	float fCubeMaxZ = BottomBackRight.z;

	float fSphereMinZ = sphere->m_position.z - sphere->m_radius;
	float fSphereMaxZ = sphere->m_position.z + sphere->m_radius;


	if (fCubeMaxX > fSphereMinX && fSphereMaxX > fCubeMinX) {
		float fXOverlap1 = fCubeMaxX - fSphereMinX;
		float fXOverlap2 = fCubeMinX - fSphereMaxX;

		if (glm::abs(fXOverlap1) < glm::abs(fXOverlap2)) {
			fXSmallestOverlap = fXOverlap1;
		}
		else {
			fXSmallestOverlap = fXOverlap2;
		}
		collisionNormal = glm::vec3(1, 0, 0);
		fIntersectDist = fXSmallestOverlap;
		fSmallestOverlap = fXSmallestOverlap;
	}
	else {
		return false;
	}
	if (fCubeMaxY > fSphereMinY && fSphereMaxY > fCubeMinY) {
		float fYOverlap1 = fCubeMaxY - fSphereMinY;
		float fYOverlap2 = fCubeMinY - fSphereMaxY;

		if (glm::abs(fYOverlap1) < glm::abs(fYOverlap2)) {
			fYSmallestOverlap = fYOverlap1;
		}
		else {
			fYSmallestOverlap = fYOverlap2;
		}
		if (glm::abs(fYSmallestOverlap) < glm::abs(fSmallestOverlap)) {
			collisionNormal = glm::vec3(0, 1, 0);
			fIntersectDist = fYSmallestOverlap;
			fSmallestOverlap = fYSmallestOverlap;
		}
	}
	else {
		return false;
	}
	if (fCubeMaxZ > fSphereMinZ && fSphereMaxZ > fCubeMinZ) {
		float fZOverlap1 = fCubeMaxZ - fSphereMinZ;
		float fZOverlap2 = fCubeMinZ - fSphereMaxZ;

		if (glm::abs(fZOverlap1) < glm::abs(fZOverlap2)) {
			fZSmallestOverlap = fZOverlap1;
		}
		else {
			fZSmallestOverlap = fZOverlap2;
		}
		if (glm::abs(fZSmallestOverlap) < glm::abs(fSmallestOverlap)) {
			collisionNormal = glm::vec3(0, 0, 1);
			fIntersectDist = fZSmallestOverlap;
			fSmallestOverlap = fZSmallestOverlap;
		}
	}
	else {
		return false;
	}

	sphere->m_position += collisionNormal * fIntersectDist;
	glm::vec3 force = -1 * sphere->m_mass * collisionNormal * (glm::dot(collisionNormal, sphere->m_velocity));
	sphere->ApplyForce(force + force);

	return true;
}

//box collision routines
//bool DIYPhysicScene::BoxToBox(PhysicsObject* obj1, PhysicsObject* obj2)
//{
//	//try to cast objects to sphere and plane
//	Box *box1 = dynamic_cast<Box*>(obj1);
//	Box *box2 = dynamic_cast<Box*>(obj2);
//	const int numberAxis = 4;
//	if (box1 != NULL && box2 != NULL)
//	{
//		glm::vec2 axis1[numberAxis];
//		glm::vec2 axis2[numberAxis];
//		float width1 = box1->width;
//		float height1 = box1->height;
//		float width2 = box2->width;
//		float height2 = box2->height;
//		//create our vertices, they must be in correct order
//		glm::vec2 verts1[4] = { glm::vec2(-width1, height1), glm::vec2(width1, height1), glm::vec2(width1, -height1), glm::vec2(-width1, -height1) };
//		glm::vec2 verts2[4] = { glm::vec2(-width2, height2), glm::vec2(width2, height2), glm::vec2(width2, -height2), glm::vec2(-width2, -height2) };
//
//		//transform to workd space
//		for (int count = 0; count < numberAxis; count++)
//		{
//			verts1[count] = (box1->rotationMatrix*glm::vec4(verts1[count], 0, 1)).xy() + box1->position;
//			verts2[count] = (box2->rotationMatrix*glm::vec4(verts2[count], 0, 1)).xy() + box2->position;
//		}
//		//get the axis
//		getAxis(verts1, axis1, numberAxis);
//		getAxis(verts2, axis2, numberAxis);
//		for (int i = 0; i<numberAxis; i++)
//		{
//			Projection projection1 = project(verts1, axis1[i], numberAxis);
//			Projection projection2 = project(verts2, axis1[i], numberAxis);
//			if (!overlap(projection1, projection2))
//			{
//				return false;
//			}
//		}
//		for (int i = 0; i<numberAxis; i++)
//		{
//			Projection projection1 = project(verts1, axis2[i], numberAxis);
//			Projection projection2 = project(verts2, axis2[i], numberAxis);
//			if (!overlap(projection1, projection2))
//			{
//				return false;
//			}
//		}
//	}
//	glm::vec2 closestPoint; //how to find this...
//	box1->collisionResponse(closestPoint);
//	box2->collisionResponse(closestPoint);
//	return true;
//
//	////try to cast objects to sphere and plane
//	//Box *box1 = dynamic_cast<Box*>(obj1);
//	//Box *box2 = dynamic_cast<Box*>(obj2);
//	//const int numberAxis = 4;
//	//if (box1 != NULL && box2 != NULL)
//	//{
//	//	glm::vec2 axis1[numberAxis];
//	//	glm::vec2 axis2[numberAxis];
//	//	float width1 = box1->width;
//	//	float height1 = box1->height;
//	//	float width2 = box2->width;
//	//	float height2 = box2->height;
//	//	//create our vertices, they must be in correct order
//	//	glm::vec2 verts1[4] = { glm::vec2(-width1, height1), glm::vec2(width1, height1), glm::vec2(width1, -height1), glm::vec2(-width1, -height1) };
//	//	glm::vec2 verts2[4] = { glm::vec2(-width2, height2), glm::vec2(width2, height2), glm::vec2(width2, -height2), glm::vec2(-width2, -height2) };
//
//	//	//transform to workd space
//	//	for (int count = 0; count < numberAxis; count++)
//	//	{
//	//		verts1[count] = (box1->rotationMatrix*glm::vec4(verts1[count], 0, 1)).xy() + box1->position;
//	//		verts2[count] = (box2->rotationMatrix*glm::vec4(verts2[count], 0, 1)).xy() + box2->position;
//	//	}
//	//	//get the axis
//	//	getAxis(verts1, axis1, numberAxis);
//	//	getAxis(verts2, axis2, numberAxis);
//	//	for (int i = 0; i<numberAxis; i++)
//	//	{
//	//		Projection projection1 = project(verts1, axis1[i], numberAxis);
//	//		Projection projection2 = project(verts2, axis1[i], numberAxis);
//	//		if (!overlap(projection1, projection2))
//	//		{
//	//			return false;
//	//		}
//	//	}
//	//	for (int i = 0; i<numberAxis; i++)
//	//	{
//	//		Projection projection1 = project(verts1, axis2[i], numberAxis);
//	//		Projection projection2 = project(verts2, axis2[i], numberAxis);
//	//		if (!overlap(projection1, projection2))
//	//		{
//	//			return false;
//	//		}
//	//	}
//	//}
//	//glm::vec2 closestPoint; //how to find this...
//	//box1->collisionResponse(closestPoint);
//	//box2->collisionResponse(closestPoint);
//	//return true;
//}

bool DIYPhysicScene::BoxToBox(PhysicsObject* _actor1, PhysicsObject* _actor2) {
	Box* box = dynamic_cast<Box*>(_actor1);
	Box* box2 = dynamic_cast<Box*>(_actor2);

	if (box->m_static == true)
		return false;

	//Box 1
	float boxExtentsX = box->m_extents.x;
	float boxExtentsY = box->m_extents.y;
	float boxExtentsZ = box->m_extents.z;

	glm::vec3 TopBackLeft = box->m_position - glm::vec3(boxExtentsX, boxExtentsY, -boxExtentsZ);
	glm::vec3 TopBackRight = box->m_position - glm::vec3(-boxExtentsX, boxExtentsY, -boxExtentsZ);
	glm::vec3 TopFrontLeft = box->m_position - glm::vec3(boxExtentsX, boxExtentsY, boxExtentsZ);
	glm::vec3 TopFrontRight = box->m_position - glm::vec3(-boxExtentsX, boxExtentsY, boxExtentsZ);

	glm::vec3 BottomBackLeft = box->m_position - glm::vec3(boxExtentsX, -boxExtentsY, -boxExtentsZ);
	glm::vec3 BottomBackRight = box->m_position - glm::vec3(-boxExtentsX, -boxExtentsY, -boxExtentsZ);
	glm::vec3 BottomFrontLeft = box->m_position - glm::vec3(boxExtentsX, -boxExtentsY, boxExtentsZ);
	glm::vec3 BottomFrontRight = box->m_position - glm::vec3(-boxExtentsX, -boxExtentsY, boxExtentsZ);

	//Box 1
	float boxExtentsX2 = box2->m_extents.x;
	float boxExtentsY2 = box2->m_extents.y;
	float boxExtentsZ2 = box2->m_extents.z;

	glm::vec3 TopBackLeft2 = box2->m_position - glm::vec3(boxExtentsX2, boxExtentsY2, -boxExtentsZ2);
	glm::vec3 TopBackRight2 = box2->m_position - glm::vec3(-boxExtentsX2, boxExtentsY2, -boxExtentsZ2);
	glm::vec3 TopFrontLeft2 = box2->m_position - glm::vec3(boxExtentsX2, boxExtentsY2, boxExtentsZ2);
	glm::vec3 TopFrontRight2 = box2->m_position - glm::vec3(-boxExtentsX2, boxExtentsY2, boxExtentsZ2);

	glm::vec3 BottomBackLeft2 = box2->m_position - glm::vec3(boxExtentsX2, -boxExtentsY2, -boxExtentsZ2);
	glm::vec3 BottomBackRight2 = box2->m_position - glm::vec3(-boxExtentsX2, -boxExtentsY2, -boxExtentsZ2);
	glm::vec3 BottomFrontLeft2 = box2->m_position - glm::vec3(boxExtentsX2, -boxExtentsY2, boxExtentsZ2);
	glm::vec3 BottomFrontRight2 = box2->m_position - glm::vec3(-boxExtentsX2, -boxExtentsY2, boxExtentsZ2);

	float fIntersectDist;
	glm::vec3 collisionNormal;
	float fXSmallestOverlap = 10000000;
	float fYSmallestOverlap = 10000000;
	float fZSmallestOverlap = 10000000;
	float fSmallestOverlap = 10000000;

	float fCubeMinX = BottomFrontLeft.x;
	float fCubeMaxX = BottomFrontRight.x;

	float fCubeMinX2 = BottomFrontLeft2.x;
	float fCubeMaxX2 = BottomFrontRight2.x;

	/*float fSphereMinX = sphere->m_position.x - sphere->m_radius;
	float fSphereMaxX = sphere->m_position.x + sphere->m_radius;*/

	float fCubeMinY = TopFrontLeft.y;
	float fCubeMaxY = BottomBackRight.y;

	float fCubeMinY2 = TopFrontLeft2.y;
	float fCubeMaxY2 = BottomBackRight2.y;

	/*float fSphereMinY = sphere->m_position.y - sphere->m_radius;
	float fSphereMaxY = sphere->m_position.y + sphere->m_radius;*/

	float fCubeMinZ = BottomFrontLeft.z;
	float fCubeMaxZ = BottomBackRight.z;

	float fCubeMinZ2 = BottomFrontLeft2.z;
	float fCubeMaxZ2 = BottomBackRight2.z;

	/*float fSphereMinZ = sphere->m_position.z - sphere->m_radius;
	float fSphereMaxZ = sphere->m_position.z + sphere->m_radius;*/


	if (fCubeMaxX > fCubeMinX2 && fCubeMaxX2 > fCubeMinX) {
		float fXOverlap1 = fCubeMaxX - fCubeMinX2;
		float fXOverlap2 = fCubeMinX - fCubeMaxX2;

		if (glm::abs(fXOverlap1) < glm::abs(fXOverlap2)) {
			fXSmallestOverlap = fXOverlap1;
		}
		else {
			fXSmallestOverlap = fXOverlap2;
		}
		collisionNormal = glm::vec3(1, 0, 0);
		fIntersectDist = fXSmallestOverlap;
		fSmallestOverlap = fXSmallestOverlap;
	}
	else {
		return false;
	}
	if (fCubeMaxY > fCubeMinY2 && fCubeMaxY > fCubeMinY) {
		float fYOverlap1 = fCubeMaxY - fCubeMinY2;
		float fYOverlap2 = fCubeMinY - fCubeMaxY;

		if (glm::abs(fYOverlap1) < glm::abs(fYOverlap2)) {
			fYSmallestOverlap = fYOverlap1;
		}
		else {
			fYSmallestOverlap = fYOverlap2;
		}
		if (glm::abs(fYSmallestOverlap) < glm::abs(fSmallestOverlap)) {
			collisionNormal = glm::vec3(0, 1, 0);
			fIntersectDist = fYSmallestOverlap;
			fSmallestOverlap = fYSmallestOverlap;
		}
	}
	else {
		return false;
	}
	if (fCubeMaxZ > fCubeMinZ2 && fCubeMaxZ2 > fCubeMinZ) {
		float fZOverlap1 = fCubeMaxZ - fCubeMinZ2;
		float fZOverlap2 = fCubeMinZ - fCubeMaxZ2;

		if (glm::abs(fZOverlap1) < glm::abs(fZOverlap2)) {
			fZSmallestOverlap = fZOverlap1;
		}
		else {
			fZSmallestOverlap = fZOverlap2;
		}
		if (glm::abs(fZSmallestOverlap) < glm::abs(fSmallestOverlap)) {
			collisionNormal = glm::vec3(0, 0, 1);
			fIntersectDist = fZSmallestOverlap;
			fSmallestOverlap = fZSmallestOverlap;
		}
	}
	else {
		return false;
	}

	//sphere->m_position += collisionNormal * fIntersectDist;
	box->m_position += collisionNormal * fIntersectDist;
	glm::vec3 force = -1 * box->m_mass * collisionNormal * (glm::dot(collisionNormal, box->m_velocity));
	box->ApplyForce(force + force);

	return true;
}

void DIYPhysicScene::Draw(Camera* camera) {
	glClearColor(0, 0, 0, 1);
	Gizmos::clear();
	glEnable(GL_DEPTH_TEST);

	for each (PhysicsObject* obj in m_actors)
	{
		obj->Draw();
	}

	Gizmos::draw(camera->getProjectionView());

}
