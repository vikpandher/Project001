#pragma once

#include "../Engine/Renderer.h"



namespace Project001
{
	class OpenGLShader;

	struct VertexDataStruct
	{
		glm::vec3 position;

		/// glm::vec3 normal;
		/// glm::vec4 color;
		/// glm::vec2 textureCoordinte;
		/// float textureIndex;
	};
	
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();
		virtual ~OpenGLRenderer();

		void AddMesh(const Mesh* mesh) override;

		void Render() override;

	///	void LoadModel(std::string modelName, std::vector<VertexDataStruct> vertexData, )

	protected:
		void AddVertex(VertexDataStruct& vertex);

		void AddIndex(glm::uint& index);

		OpenGLShader* shaderPtr_;

		// this holds the buffer's id
		// the buffer holds the blob of data that will be displayed
		glm::uint vertexBufferId_;

		// this holds the index buffer's id
		glm::uint indexBufferId_;

		// this holds the vertex array's id
		// the vertex array holds information about the size, shape, and type of array
		glm::uint vertexArrayId_;

		int vertexBufferSize_;
		VertexDataStruct* vertexBufferPtr_;

		int indexBufferSize_;
		glm::uint* indexBufferPtr_;

		static const unsigned int s_vertexBufferCapacity_;
		static const unsigned int s_indexBufferCapacity_;

		static const char* s_vertexShaderSource01_;
		static const char* s_fragmentShaderSource01_;

	private:
		
	};

	// protected ---------------------------------------------------------------

	inline void OpenGLRenderer::AddVertex(VertexDataStruct& vertex)
	{
		vertexBufferPtr_[vertexBufferSize_++] = vertex;
	}

	inline void OpenGLRenderer::AddIndex(glm::uint& index)
	{
		indexBufferPtr_[indexBufferSize_++] = index;
	}

}