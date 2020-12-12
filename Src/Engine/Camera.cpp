#include "Camera.h"



namespace Project001
{		
	// public: -----------------------------------------------------------------

	Camera::Camera()
		: cameraPosition_(0.0f, 0.0f, 2.0f)
		, cameraOrientation_(1.0f, 0.0f, 0.0f, 0.0f)
		, fieldOfVisionDegrees_(45.0f)
		, aspectRatio_(1.0f)
		, leftCutOff_(5.0f)
		, rightCutOff_(5.0f)
		, bottomCutOff_(5.0f)
		, topCutOff_(5.0f)
		, nearCutOff_(0.1)
		, farCutOff_(10.0f)
		, worldForward_(0.0f, 0.0f, -1.0f) // used for Roll "right hand rule"
		, worldUp_(0.0f, 1.0f, 0.0f) // used for Yaw "right hand rule"
		, worldRight_(1.0f, 0.0f, 0.0f) // used for Pitch "right hand rule"
	{

	}

	Camera::~Camera()
	{

	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		glm::mat3 rotationMatrix = glm::mat3_cast(cameraOrientation_);
		glm::mat4 viewMatrix = glm::mat4(rotationMatrix);
		viewMatrix = glm::translate(viewMatrix, cameraPosition_ * -1.0f);
		return viewMatrix;
	}

	glm::mat4 Camera::GetPerspectiveProjectionMatrix() const
	{
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fieldOfVisionDegrees_), aspectRatio_, nearCutOff_, farCutOff_);
		return projectionMatrix;
	}

	glm::mat4 Camera::GetOrthographicProjectionMatrix() const
	{
		glm::mat4 projectionMatrix = glm::ortho(leftCutOff_, rightCutOff_, bottomCutOff_, topCutOff_, nearCutOff_, farCutOff_);
		return projectionMatrix;
	}
}