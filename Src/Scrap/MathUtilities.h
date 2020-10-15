#pragma once

#include "glm/glm.hpp"



namespace Project001
{
	glm::vec3 GetForwardVector(float yaw, float pitch);

	glm::mat3 ViewMatrixYawPitchRoll(float const& yaw, float const& pitch, float const& roll);
}