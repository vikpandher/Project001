#include "Tests.h"

#include "glm/gtc/constants.hpp" 
#include "glm/gtc/quaternion.hpp"

#include "Engine/Logger.h"
#include "Engine/MathUtilities.h"
#include "Engine/TimingUtilities.h"



namespace Project001
{
	void RunTests()
	{
		TestMathUtilities();
	}

	void TestTimingUtilities()
	{
		Project001::TimeProfiler::BeginSession();
		{
			Project001::ScopeTimer timer(__FUNCSIG__);
			for (int i = 0; i < 1000; ++i)
			{
				printf("%d\n", i);
			}
		}
		Project001::TimeProfiler::EndSession();
	}
	
	void TestMathUtilities()
	{
		Logger::Message(__FUNCSIG__);

		glm::mat4 r0 = glm::mat4(ViewMatrixYawPitchRoll(0.0f, 0.0f, 0.0f));
		Logger::Message("GetForwardVector(0.0f, 0.0f, 0.0f)");
		Logger::Message("r0     = {%f, %f, %f, %f}", r0[0][0], r0[0][1], r0[0][2], r0[0][3]);
		Logger::Message("         {%f, %f, %f, %f}", r0[1][0], r0[1][1], r0[1][2], r0[1][3]);
		Logger::Message("         {%f, %f, %f, %f}", r0[2][0], r0[2][1], r0[2][2], r0[2][3]);
		Logger::Message("         {%f, %f, %f, %f}", r0[3][0], r0[3][1], r0[3][2], r0[3][3]);

		glm::mat4 r1 = glm::mat4(ViewMatrixYawPitchRoll(glm::half_pi<float>(), 0.0f, 0.0f));
		Logger::Message("GetForwardVector(pi/2, 0.0f, 0.0f)");
		Logger::Message("r1     = {%f, %f, %f, %f}", r1[0][0], r1[0][1], r1[0][2], r1[0][3]);
		Logger::Message("         {%f, %f, %f, %f}", r1[1][0], r1[1][1], r1[1][2], r1[1][3]);
		Logger::Message("         {%f, %f, %f, %f}", r1[2][0], r1[2][1], r1[2][2], r1[2][3]);
		Logger::Message("         {%f, %f, %f, %f}", r1[3][0], r1[3][1], r1[3][2], r1[3][3]);

		glm::mat4 r2 = glm::mat4(ViewMatrixYawPitchRoll(-glm::half_pi<float>(), 0.0f, 0.0f));
		Logger::Message("GetForwardVector(-pi/2, 0.0f, 0.0f)");
		Logger::Message("r2     = {%f, %f, %f, %f}", r2[0][0], r2[0][1], r2[0][2], r2[0][3]);
		Logger::Message("         {%f, %f, %f, %f}", r2[1][0], r2[1][1], r2[1][2], r2[1][3]);
		Logger::Message("         {%f, %f, %f, %f}", r2[2][0], r2[2][1], r2[2][2], r2[2][3]);
		Logger::Message("         {%f, %f, %f, %f}", r2[3][0], r2[3][1], r2[3][2], r2[3][3]);

		glm::mat4 r3 = glm::mat4(ViewMatrixYawPitchRoll(glm::pi<float>(), 0.0f, 0.0f));
		Logger::Message("GetForwardVector(pi, 0.0f, 0.0f)");
		Logger::Message("r3     = {%f, %f, %f, %f}", r3[0][0], r3[0][1], r3[0][2], r3[0][3]);
		Logger::Message("         {%f, %f, %f, %f}", r3[1][0], r3[1][1], r3[1][2], r3[1][3]);
		Logger::Message("         {%f, %f, %f, %f}", r3[2][0], r3[2][1], r3[2][2], r3[2][3]);
		Logger::Message("         {%f, %f, %f, %f}", r3[3][0], r3[3][1], r3[3][2], r3[3][3]);

		glm::vec3 worldFront_(0.0f, 0.0f, -1.0f);
		glm::vec3 worldRight_(1.0f, 0.0f, 0.0f);
		glm::vec3 worldUp_(0.0f, 1.0f, 0.0f);

		glm::quat q0(1.0f, 0.0f, 0.0f, 0.0f);
		glm::mat4 r4 = glm::mat4_cast(q0);
		Logger::Message("r4     = {%f, %f, %f, %f}", r4[0][0], r4[0][1], r4[0][2], r4[0][3]);
		Logger::Message("         {%f, %f, %f, %f}", r4[1][0], r4[1][1], r4[1][2], r4[1][3]);
		Logger::Message("         {%f, %f, %f, %f}", r4[2][0], r4[2][1], r4[2][2], r4[2][3]);
		Logger::Message("         {%f, %f, %f, %f}", r4[3][0], r4[3][1], r4[3][2], r4[3][3]);
		
	}
}