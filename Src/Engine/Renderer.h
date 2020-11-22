#pragma once

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

		virtual void SetAbiantLightColor(const glm::vec3& ambiantLightColor) = 0;

		virtual void SetAmbiantLightStrength(float ambiantLightStrength) = 0;

		virtual void SetViewMatrix(const glm::mat4& viewMatrix) = 0;

		virtual void SetProjectionMatrix(const glm::mat4& projectionMatrix) = 0;

	protected:

	private:
		
	};
}