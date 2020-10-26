#pragma once

#include "ModelUtilities.h"

#include <map>
#include <string>



namespace Project001
{
	class Stores
	{
	public:
		Stores();
		~Stores();

		Stores(Stores& other) = delete;
		void operator=(const Stores&) = delete;

		MeshData* GetMesh(std::string name);

		TextureData* GetTexture(std::string name);

		bool LoadOBJFile(std::string name, std::string path);

		bool LoadTextureFile(std::string name, std::string path);

	protected:
		std::string GetFirstToken(const std::string& input) const;

		std::map<std::string, MeshData*> meshMap_;

		std::map<std::string, TextureData*> textureMap_;

	private:
		
	};
}