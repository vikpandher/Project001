#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <vector>



namespace Project001
{		
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
		glm::vec3 position_;
		glm::quat orientation_;
	};

	struct TextureData
	{
		unsigned char* data;
		int width;
		int height;
		int numberOfComponents; // alternativly bytes per pixel
	};
}