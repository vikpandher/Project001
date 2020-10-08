#pragma once

#include "../Engine/ModelUtilities.h"



namespace Project001
{
	class Renderer
	{
	public:
		virtual void AddMesh(const Mesh* mesh) = 0;

		virtual void Render() = 0;

	protected:

	private:
		
	};
}