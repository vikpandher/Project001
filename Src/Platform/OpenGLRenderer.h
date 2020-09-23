#pragma once

#include "glm/glm.hpp"

#include "../Engine/Renderer.h"

//#include <string>
#include <vector>



namespace Project001
{
	class OpenGLShader;
	
	struct VertexDataStruct
	{
		glm::vec3 position;
		///glm::vec3 normal;
		///glm::vec4 color;
		///glm::vec2 textureCoordinte;
		///float textureIndex;
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
		virtual ~OpenGLRenderer();

		void Render() override;

	///	void LoadModel(std::string modelName, std::vector<VertexDataStruct> vertexData, )

	protected:
		OpenGLShader* shaderPtr_;

		// this holds the buffer's id
		// the buffer holds the blob of data that will be displayed
		glm::uint vertexBufferId_;

		// this holds the vertex array's id
		// the vertex array holds information about the size, shape, and type of array
		glm::uint vertexArrayId_;

		std::vector<VertexDataStruct> vertexBuffer_;

	private:
		static const char* s_vertexShaderSource01_;
		static const char* s_fragmentShaderSource01_;
		
	};
}