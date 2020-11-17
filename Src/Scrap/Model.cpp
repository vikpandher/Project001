#include "Model.h"

#include "ModelUtilities.h"

#include "Logger.h"



namespace Project001
{		
	// public: -----------------------------------------------------------------

	Model::Model()
		: meshDataPtr_(nullptr)
		, color_(1.0f, 1.0f, 1.0f, 1.0f)
		, textureIndex_(-1)
		, position_(0.0f, 0.0f, 0.0f)
		, orientation_(1.0f, 0.0f, 0.0f, 0.0f)
	{

	}

	Model::Model(MeshData* meshDataPtr, const glm::vec4& color, const glm::int32& textureIndex, const glm::vec3& position, const glm::quat& orientation)
		: meshDataPtr_(meshDataPtr)
		, color_(color)
		, textureIndex_(textureIndex)
		, position_(position)
		, orientation_(orientation)
	{

	}

	Model::~Model()
	{

	}
}