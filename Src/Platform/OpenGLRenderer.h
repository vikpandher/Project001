#pragma once

#include "../Engine/Renderer.h"
//^includes "ModelUtilities.h.h"

#include <map>
#include <string>
#include <tuple>



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

		void SetViewportSize(int lowerLeftX, int lowerLeftY, int width, int height) override;

	protected:
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

		static const unsigned int s_bufferCapacity_ = 36 * 5;

		std::vector<VertexDataStruct> vertexBuffer_;
		std::vector<glm::uint> indexBuffer_;

		// TODO:
		//std::vector<const ModelData*> modelDataPointers_;
		//std::vector<std::pair<const ModelData*, glm::uint>> modelDataPointersPendingSort_;

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
}