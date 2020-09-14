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
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 textureCoordinte;
		float textureIndex;
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

	private:
		static const char* s_vertexShaderSource01_;
		static const char* s_fragmentShaderSource01_;
		
	};
}