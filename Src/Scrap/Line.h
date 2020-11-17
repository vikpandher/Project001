#pragma once

#include "glm/glm.hpp"

#include "ModelUtilities.h"

#include <vector>



namespace Project001
{
	class Line
	{
	public:
		Line();
		~Line();

		Line(Line& other) = delete;
		void operator=(const Line&) = delete;

		void AddVertex(glm::vec3 vertex);

		void ClearVertices();

		void SetColor(glm::vec4 color);

	protected:
		void GeneratreMesh();

		std::vector<glm::vec3> verticies_;
		glm::vec4 color_;

		MeshData* meshDataPtr_;

	private:
		
	};
}