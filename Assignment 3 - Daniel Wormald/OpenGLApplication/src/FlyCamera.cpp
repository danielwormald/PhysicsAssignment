

#define GLM_SWIZZLE
#include "FlyCamera.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

Camera::Camera(float fieldOfView, float aspectRatio, float near, float far)
	: m_speed(10),
	m_up(0,1,0),
	m_transform(1),
	m_view(1)
{
	setPerspective(fieldOfView, aspectRatio, near, far);
}

Camera::~Camera()
{

}

void Camera::setPerspective(float fovY, float aspectRatio, float near, float far)
{
	m_projection = glm::perspective(fovY, aspectRatio, near, far);
	m_projectionView = m_projection * m_view;
}

void Camera::setLookAtFrom(const glm::vec3& from, const glm::vec3& to)
{
	m_view = glm::lookAt(from, to, glm::vec3(0, 1, 0));
	m_transform = glm::inverse(m_view);
	m_projectionView = m_projection * m_view;
}

void Camera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	float frameSpeed = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? deltaTime * m_speed * 2 : deltaTime * m_speed;	
	float extraSpeed = 2.0f;
	// translate
	if (glfwGetKey(window,'W') == GLFW_PRESS)
		m_transform[3] -= m_transform[2] * frameSpeed * extraSpeed;
	if (glfwGetKey(window,'S') == GLFW_PRESS)
		m_transform[3] += m_transform[2] * frameSpeed * extraSpeed;
	if (glfwGetKey(window,'D') == GLFW_PRESS)
		m_transform[3] += m_transform[0] * frameSpeed * extraSpeed;
	if (glfwGetKey(window,'A') == GLFW_PRESS)
		m_transform[3] -= m_transform[0] * frameSpeed * extraSpeed;
	if (glfwGetKey(window,'E') == GLFW_PRESS)
		m_transform[3] += m_transform[1] * frameSpeed * extraSpeed;
	if (glfwGetKey(window,'Q') == GLFW_PRESS)
		m_transform[3] -= m_transform[1] * frameSpeed * extraSpeed;
	
	// check for rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window,&siPrevMouseX,&siPrevMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window,&mouseX,&mouseY);

		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 mMat;
		
		// pitch
		if (iDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix(m_transform[0].xyz(), (float)-iDeltaY / 150.0f);
			m_transform[0] = mMat * m_transform[0];
			m_transform[1] = mMat * m_transform[1];
			m_transform[2] = mMat * m_transform[2];
		}

		// yaw
		if (iDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix( m_up, (float)-iDeltaX / 150.0f );
			m_transform[0] = mMat * m_transform[0];
			m_transform[1] = mMat * m_transform[1];
			m_transform[2] = mMat * m_transform[2];
		}
	}
	else
	{
		sbMouseButtonDown = false;
	}

	m_view = glm::inverse(m_transform);
	m_projectionView = m_projection * m_view;
}

glm::vec3 Camera::screenPositionToDirection(float x, float y) const {
	
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	return glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();
}

glm::vec3 Camera::pickAgainstPlane(float x, float y, const glm::vec4& plane) const {

	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	glm::vec3 dir = glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();

	float d = (plane.w - glm::dot(m_transform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return m_transform[3].xyz() + dir * d;
}

//#include "FlyCamera.h"
//#include <GLFW\glfw3.h>
//#include <iostream>
//
//using glm::vec3;
//using glm::vec4;
//using glm::mat4;
//
//FlyCamera::FlyCamera(GLFWwindow* window)
//{
//	m_bViewButtonClicked = false;
//
//	this->SetInputWindow(window);
//	this->SetupPerspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 1000.f);
//	this->SetFlySpeed(1.0f);
//	this->LookAt(vec3(15,15,15), vec3(0), vec3(0,1,0));
//}
//
//void FlyCamera::Update(double dt)
//{
//	HandleKeyboardInput(dt);
//	HandleMouseInput(dt);
//}
//
//void FlyCamera::HandleKeyboardInput(double dt)
//{
//	//Get the cameras forward/up/right
//	glm::vec3 vRight = m_worldTransform[0].xyz;
//	glm::vec3 vUp = m_worldTransform[1].xyz;
//	glm::vec3 vForward = m_worldTransform[2].xyz;
//
//	glm::vec3 moveDir(0.0f);
//
//	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
//	{
//		moveDir -= vForward * glm::vec3(30.0f,30.0f,30.0f);
//	}
//
//	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
//	{
//		moveDir += vForward;
//	}
//
//	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
//	{
//		moveDir -= vRight;
//	}
//
//	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
//	{
//		moveDir += vRight;
//	}
//
//	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//	{
//		moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
//	}
//	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//	{
//		moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);
//	}
//
//	float fLength = glm::length(moveDir);
//	if (fLength > 0.01f)
//	{
//		moveDir = ((float)dt * m_fFlySpeed) * glm::normalize(moveDir);
//		SetPosition(GetPosition() + moveDir);
//	}
//}
//
//void FlyCamera::HandleMouseInput(double dt)
//{
//	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
//	{
//		if (m_bViewButtonClicked == false)
//		{
//			int width, height;
//			glfwGetFramebufferSize(m_pWindow, &width, &height);
//
//			m_dCursorX = width / 2.0;
//			m_dCursorY = height / 2.0;
//			glfwSetCursorPos(m_pWindow, m_dCursorX, m_dCursorY);
//			m_bViewButtonClicked = true;
//		}
//		else
//		{
//			double mouseX, mouseY;
//			glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);
//
//			double xOffset = mouseX - m_dCursorX;
//			double yOffset = mouseY - m_dCursorY;
//
//			m_dCursorX = mouseX;
//			m_dCursorY = mouseY;
//
//			CalculateRotation(xOffset, yOffset);
//
//			std::cout << xOffset << " " << yOffset << std::endl;
//		}
//	}
//	else
//	{
//		m_bViewButtonClicked = false;
//	}
//}
//
//void FlyCamera::CalculateRotation(double xOffset, double yOffset)
//{
//	if( xOffset != 0.0 )
//	{
//		glm::mat4 rot = glm::rotate((float)xOffset * 0.001f, glm::vec3(0,-1,0));
//		SetTransform( GetTransform() * rot );
//	}
//
//	if( xOffset != 0.0 )
//	{
//		glm::mat4 rot = glm::rotate((float)yOffset * 0.001f, glm::vec3(1, 0, 0));
//		SetTransform( GetTransform() * rot );
//	}
//
//	// TODO: normalise the matrix...
//	//glm::normalize(GetTransform());
//}
//
//void FlyCamera::SetFlySpeed(float speed)
//{
//	m_fFlySpeed = speed;
//}