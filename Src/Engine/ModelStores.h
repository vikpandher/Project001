#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"



namespace Project001
{
    struct ModelData
    {
        glm::uint vertexIndex;
        glm::uint vertexCount;
    };
    
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

        bool GetModel(unsigned int index, ModelVertex*& modelVerticies, glm::uint& vertexCount);

        bool LoadModel(const std::string& path, unsigned int& index);

        bool Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, unsigned int& index);

        bool Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, unsigned int& index);

        bool Generate2DLine(const std::vector<glm::vec2>& vertices, float width, unsigned int& index);

    protected:

    private:
        std::vector<ModelVertex> modelVertexArray_;
        std::vector<ModelData> modelDataArray_;

    };
}