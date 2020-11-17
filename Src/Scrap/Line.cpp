#include "Line.h"

#include "Logger.h"



namespace Project001
{		
	// public: -----------------------------------------------------------------

	Line::Line()
		: meshDataPtr_(nullptr)
	{

	}

	Line::~Line()
	{
		if (meshDataPtr_ != nullptr)
		{
			delete meshDataPtr_;
			meshDataPtr_ = nullptr;
		}
	}

	void Line::AddVertex(glm::vec3 vertex)
	{
		verticies_.push_back(vertex);
	}

	void Line::ClearVertices()
	{
		verticies_.clear();
	}

	void Line::SetColor(glm::vec4 color)
	{
		color_ = color;
	}

	// protected: --------------------------------------------------------------

	void Line::GeneratreMesh()
	{

	}
}