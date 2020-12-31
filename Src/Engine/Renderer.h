#pragma once

#include <mutex>

#include "ModelUtilities.h"



namespace Project001
{
	class Renderer
	{
	public:
		virtual void AddMesh(const MeshData* meshData) = 0;

		virtual void AddModel(const ModelData* modelData) = 0;

		virtual void AddTexture(const TextureData* textureData, glm::u32 index) = 0;

		virtual void Render() = 0;

		virtual void SetViewMatrix(const glm::mat4& viewMatrix) = 0;

		virtual void SetViewPosition(const glm::vec3& viewPosition) = 0;

		virtual void SetProjectionMatrix(const glm::mat4& projectionMatrix) = 0;

		virtual void SetDirectionalLight(const DirectionalLight& light) = 0;

		virtual void SetPointLight(const PointLight& light, glm::u32 index) = 0;

		virtual void SetSpotLight(const SpotLight& light, glm::u32 index) = 0;

		virtual void SetViewportSize(int lowerLeftX, int lowerLeftY, int width, int height) = 0;

	protected:

	private:
		
	};
}