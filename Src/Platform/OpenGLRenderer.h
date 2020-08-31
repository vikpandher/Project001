#pragma once

#include "glm/glm.hpp"

#include "../Engine/Renderer.h"

#include <string>
#include <vector>



namespace Project001
{
	struct VertexDataStruct
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture;
	};

	struct MeshDataStruct
	{
		glm::uint vertexStart;
		glm::uint vertexCount;
		glm::uint indexStart;
		glm::uint indexCount;
	};
	
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();

	///	void LoadModel(std::string modelName, std::vector<VertexDataStruct> vertexData, )

	protected:
	///	std::vector<VertexDataStruct> vertexData_;

	private:
		
	};
}