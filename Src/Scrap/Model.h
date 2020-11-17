#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <vector>



namespace Project001
{
	struct MeshData;
	
	class Model
	{
	public:
		Model();
		Model(MeshData* meshDataPtr, const glm::vec4& color, const glm::int32& textureIndex, const glm::vec3& position, const glm::quat& orientation);

		~Model();

		Model(Model& other) = delete;
		void operator=(const Model&) = delete;

		void SetMeshDataPtr(MeshData* meshDataPtr);

		void SetColor(const glm::vec4& color);

		void SetTextureIndex(const glm::int32& textureIndex);

		void SetPosition(const glm::vec3& position);

		void SetOrientation(const glm::quat& orientation);

	protected:
		MeshData* meshDataPtr_;
		glm::vec4 color_;
		glm::int32 textureIndex_;
		glm::vec3 position_;
		glm::quat orientation_;

	private:
		
	};

	inline void Model::SetMeshDataPtr(MeshData* meshDataPtr)
	{
		meshDataPtr_ = meshDataPtr;
	}

	inline void Model::SetColor(const glm::vec4& color)
	{
		color_ = color;
	}

	inline void Model::SetTextureIndex(const glm::int32& textureIndex)
	{
		textureIndex_ = textureIndex;
	}

	inline void Model::SetPosition(const glm::vec3& position)
	{
		position_ = position;
	}

	inline void Model::SetOrientation(const glm::quat& orientation)
	{
		orientation_ = orientation;
	}
}