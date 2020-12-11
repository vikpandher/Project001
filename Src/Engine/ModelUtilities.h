#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <vector>



namespace Project001
{		
	struct DirectionalLight {
		DirectionalLight()
			: direction(0.0f, 0.0f, 0.0f)
			, ambient(0.0f, 0.0f, 0.0f)
			, diffuse(0.0f, 0.0f, 0.0f)
			, specular(0.0f, 0.0f, 0.0f)
		{}

		// Direction the light is going
		glm::vec3 direction;

		// Base light applied to everything from all directions evenly
		glm::vec3 ambient;

		// Light comming from a direction
		glm::vec3 diffuse;

		// The color of the shine
		glm::vec3 specular;
	};

	struct PointLight {
		PointLight()
			: position(0.0f, 0.0f, 0.0f)
			, constant(1.0f)
			, linear(0.0f)
			, quadratic(0.0f)
			, ambient(0.0f, 0.0f, 0.0f)
			, diffuse(0.0f, 0.0f, 0.0f)
			, specular(0.0f, 0.0f, 0.0f)
		{}

		// Position of the light's source
		glm::vec3 position;

		// This term is usually kept at 1.0 which is maily there to make sure
		// the denominator for the attenuation calculation never gets smaller
		// than 1 since otherwise it would boost the intensity with certain
		// distances
		float constant;

		// This reduces the intesity of the light in a linear fashion
		// Higher equals faster dimming
		float linear;

		// This reduces the intesity of the light in a quadration fashin
		// Higher equals faster dimming
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct SpotLight {
		SpotLight()
			: position(0.0f, 0.0f, 0.0f)
			, direction(0.0f, 0.0f, 0.0f)
			, cutOff(0.0f)
			, outerCutOff(0.0f)
			, constant(1.0f)
			, linear(0.0f)
			, quadratic(0.0f)
			, ambient(0.0f, 0.0f, 0.0f)
			, diffuse(0.0f, 0.0f, 0.0f)
			, specular(0.0f, 0.0f, 0.0f)
		{}

		glm::vec3 position;
		glm::vec3 direction;

		// The cut-offs determine the angle the cone of light spreads from the
		// source: 
		// A value of 0.0 equals a 180 degree angle of light, a completely
		// flattened cone. 
		// A value of 1.0 equals a 0 degree angle of light, an infinitly skinny
		// cone.
		// Light will dim from the cutOff to the outerCutOff. Valid values are
		// from 0.0 to 1.0.
		// 
		// An example of some good values are:
		// cutOff = 0.99 & outterCutOff = 0.97
		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		// Amblient light is cutoff as well, but will light the target from
		// behind.
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	
	struct IndexGroup
	{
		 glm::uint positonIndex;
		 glm::uint textureCoordinateIndex;
		 glm::uint normalIndex;
	};
	
	struct MeshData
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> textureCoordinates;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<IndexGroup>> faces;
	};

	struct ModelData
	{
		MeshData* meshDataPtr_;
		glm::vec4 color_;
		float textureIndex_;
		float specularIndex_;
		float shininess_;
		glm::vec3 position_;
		glm::quat orientation_;
		
		// TODO:
		//bool transparent_;
	};

	struct TextureData
	{
		unsigned char* data;
		int width;
		int height;
		int numberOfComponents; // alternativly bytes per pixel
	};
}