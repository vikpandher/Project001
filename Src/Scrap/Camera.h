#pragma once

#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"




namespace Project001
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		Camera(Camera& other) = delete;
		void operator=(const Camera&) = delete;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetPerspectiveProjectionMatrix() const;
		glm::mat4 GetOrthographicProjectionMatrix() const;

		glm::vec3 cameraPosition_;
		glm::quat cameraOrientation_;

		float fieldOfVisionDegrees_; // only used in perspective projection
		float aspectRatio_; // only used in perspective projection
		float leftCutOff_;  // only used in othographic projection
		float rightCutOff_; // only used in othographic projection
		float bottomCutOff_; // only used in othographic projection
		float topCutOff_; // only used in othographic projection
		float nearCutOff_;
		float farCutOff_;

		// World Coordinate System
		// ---------------------------------------------------------------------

		// NOTE:
		// positive x is to the right, positive y is up, and positive z is out of the screen

		glm::vec3 worldForward_;
		glm::vec3 worldUp_;
		glm::vec3 worldRight_;

	protected:

	private:
		
	};
}