#pragma once

#include <vector>

#include "glm/glm.hpp"



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
		float specularIndex_;
		float shininess_;
		glm::vec3 position_;
		glm::vec4 orientation_;
		
		// TODO:
		//bool transparent_;
	};
}