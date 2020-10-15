#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"



namespace Project001
{
	struct Camera
	{
		glm::vec3 position;
		glm::quat orientation;

		float pitch; // right hand rotation around x
		float yaw; // right hand hand rotation around y
		float roll; // right hand rotation around z

		//glm::vec3 front;
		//glm::vec3 up;

		float fieldOfVisionDegrees;
		float aspectRatio;
		float nearCutOff;
		float farCutOff;
	};
}