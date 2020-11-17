#pragma once

#include "ModelUtilities.h"



namespace Project001
{
	class ModelInterface
	{
	public:
		virtual const MeshData* meshDataPtr GetMeshDataPtr() const = 0;

		virtual glm::vec4 color GetColor() const = 0;

		virtual glm::int32 textureIndex GetTextureIndex() const = 0;

		virtual glm::vec3 position GetPosition() const = 0;

		virtual glm::quat orientation GetOrientation() const = 0;

	protected:

	private:
		
	};
}