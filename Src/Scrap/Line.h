#pragma once

#include "glm/glm.hpp"

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

		void addVertex(glm::vec3 vertex);

	protected:
		std::vector<glm::vec3> verticies_;
		std::vector<glm::vec3> triangleVerticies_;

	private:
		
	};
}