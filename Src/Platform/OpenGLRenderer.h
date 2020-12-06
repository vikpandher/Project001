#pragma once

#include "../Engine/Renderer.h"
//^includes "ModelUtilities.h.h"

#include <map>
#include <string>



namespace Project001
{
	class OpenGLShader;
	class OpenGLTexture;

	struct VertexDataStruct
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color;
		glm::vec2 textureCoordinte;
		float textureIndex;
		float specularIndex;
		float shininess;
		glm::vec3 translation;
		glm::quat orientation;
	};
	
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();
		virtual ~OpenGLRenderer();

		void AddMesh(const MeshData* meshData) override;

		void AddModel(const ModelData* modelData) override;

		void AddTexture(const TextureData* textureData, glm::u32 index) override;

		void Render() override;

		void SetViewMatrix(const glm::mat4& viewMatrix) override;

		void SetViewPosition(const glm::vec3& viewPosition) override;

		void SetProjectionMatrix(const glm::mat4& projectionMatrix) override;

		void SetDirectionalLight(const DirectionalLight& light) override;

		void SetPointLight(const PointLight& light, glm::u32 index) override;

		void SetSpotLight(const SpotLight& light, glm::u32 index) override;

	///	void LoadModel(std::string modelName, std::vector<VertexDataStruct> vertexData, )

	protected:
		void AddVertex(const VertexDataStruct& vertex);

		void AddIndex(const glm::uint& index);

		OpenGLShader* shaderPtr_;

		static const unsigned int s_numberOfTextureSlots_ = 16;
		OpenGLTexture* texturePtrs_[s_numberOfTextureSlots_];

		// this holds the buffer's id
		// the buffer holds the blob of data that will be displayed
		glm::uint vertexBufferId_;

		// this holds the index buffer's id
		glm::uint indexBufferId_;

		// this holds the vertex array's id
		// the vertex array holds information about the size, shape, and type of array
		glm::uint vertexArrayId_;

		static const unsigned int s_vertexBufferCapacity_ = 36 * 5;
		VertexDataStruct* vertexBufferPtr_;
		int vertexBufferSize_;

		glm::uint* indexBufferPtr_;
		int indexBufferSize_;

		static const unsigned int s_indexBufferCapacity_ = 36 * 5;

		glm::mat4 viewMatrix_;
		glm::vec3 viewPosition_;

		glm::mat4 projectionMatrix_;

		DirectionalLight directionalLight_;

		static const unsigned int s_numberOfPointLights_ = 8;
		PointLight pointLights_[s_numberOfPointLights_];

		static const unsigned int s_numberOfSpotLights_ = 4;
		SpotLight spotLights_[s_numberOfSpotLights_];

		static const char* s_vertexShaderSource01_;
		static const char* s_fragmentShaderSource01_;

	private:
		
	};

	// protected ---------------------------------------------------------------

	inline void OpenGLRenderer::AddVertex(const VertexDataStruct& vertex)
	{
		vertexBufferPtr_[vertexBufferSize_++] = vertex;
	}

	inline void OpenGLRenderer::AddIndex(const glm::uint& index)
	{
		indexBufferPtr_[indexBufferSize_++] = index;
	}

}