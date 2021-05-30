#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"



namespace Project001
{
    struct ModelVertex
    {
        glm::vec3 position;
        glm::vec2 textureCoordinte;
        glm::vec3 normal;
    };

    class ModelStores
    {
    public:
        ModelStores();

        ~ModelStores();

        void ClearModels();

        // If more models are added, the pointers returned are nolonger valid.
        bool GetModel(unsigned int index, ModelVertex*& firstVertexPtr, glm::uint& vertexCount, glm::uint*& firstIndexPtr, glm::uint& indexCount);

        bool LoadModel(const std::string& path, unsigned int& index, bool triangulate = false);

        bool Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, unsigned int& index, bool triangulate = false);

        bool Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, unsigned int& index, bool triangulate = false);

        bool Generate2DLine(const std::vector<glm::vec2>& vertices, float width, unsigned int& index);

    protected:
        struct ModelData
        {
            glm::uint vertexIndex;
            glm::uint vertexCount;
            glm::uint indexIndex;
            glm::uint indexCount;
        };

        std::vector<ModelVertex> modelVertexArray_;
        std::vector<glm::uint> modelIndexArray_;
        std::vector<ModelData> modelDataArray_;

    private:
    };
}