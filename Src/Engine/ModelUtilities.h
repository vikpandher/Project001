#pragma once

#include "glm/glm.hpp"

#include <vector>



namespace Project001
{		
	struct IndexGroup
	{
		 glm::uint positonIndex;
		 glm::uint textureCoordinateIndex;
		 glm::uint normalIndex;
	};
	
	struct Mesh
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> textureCoordinates;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<IndexGroup>> faces;
	};
}