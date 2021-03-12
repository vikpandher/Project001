#pragma once

#include <map>
#include <string>



namespace Project001
{
    struct MeshData;
    struct TextureData;

    class ResourceStores
    {
    public:
        ResourceStores();
        ~ResourceStores();

        ResourceStores(ResourceStores& other) = delete;
        void operator=(const ResourceStores&) = delete;

        const MeshData* GetMesh(const std::string& name) const;

        const TextureData* GetTexture(const std::string& name) const;

        bool LoadOBJFile(const std::string& name, const std::string& path);

        bool LoadTextureFile(const std::string& name, const std::string& path);

        void ClearMeshes();

        void ClearTextures();

    protected:
        std::string GetFirstToken(const std::string& input) const;

        std::map<std::string, MeshData*> meshMap_;

        std::map<std::string, TextureData*> textureMap_;

    private:
    };
}