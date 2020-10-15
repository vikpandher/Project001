#include "MathUtilities.h"



namespace Project001
{
	glm::vec3 GetForwardVector(float yaw, float pitch)
	{
		glm::vec3 forward;
		forward.x = glm::sin(yaw) * glm::cos(pitch);
		forward.y = glm::sin(pitch);
		forward.z = (-1) * glm::cos(yaw) * glm::cos(pitch);
		return forward;
	}

	glm::mat3 ViewMatrixYawPitchRoll(float const& yaw, float const& pitch, float const& roll)
	{
		float cosYaw = glm::cos(yaw);
		float sinYaw = glm::sin(yaw);
		float cosPit = glm::cos(pitch);
		float sinPit = glm::sin(pitch);
		float cosRol = glm::cos(roll);
		float sinRol = glm::sin(roll);

		glm::mat3 Result;
		Result[0][0] = cosYaw * cosRol + sinYaw * sinPit * sinRol;
		Result[0][1] = sinRol * cosPit;
		Result[0][2] = -sinYaw * cosRol + cosYaw * sinPit * sinRol;
		Result[1][0] = -cosYaw * sinRol + sinYaw * sinPit * cosRol;
		Result[1][1] = cosRol * cosPit;
		Result[1][2] = sinRol * sinYaw + cosYaw * sinPit * cosRol;
		Result[2][0] = sinYaw * cosPit;
		Result[2][1] = -sinPit;
		Result[2][2] = cosYaw * cosPit;
		return Result;
	}
}