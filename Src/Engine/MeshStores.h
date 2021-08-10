#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"



namespace Project001
{
    struct MeshVertex
    {
        MeshVertex()
            : position(0.0f)
            , textureCoordinte(0.0f)
            , normal(0.0f)
        {}

        glm::vec3 position;
        glm::vec2 textureCoordinte;
        glm::vec3 normal;
    };

    class MeshStores
    {
    public:
        MeshStores();

        ~MeshStores();

        void ClearMeshes();

        // If more meshes are added, the pointers returned are nolonger valid.
        bool GetMesh(glm::uint index, MeshVertex*& firstVertexPtr, glm::uint& vertexCount, glm::uint*& firstIndexPtr, glm::uint& indexCount);

        bool LoadMesh(const std::string& path, glm::uint& index, bool normalizeSize = true, bool recenter = true, bool triangulate = true);

        bool Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter = true, bool triangulate = true);

        bool Generate2DTriangles(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter = true);

        bool Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter = true, bool triangulate = true);

        bool Generate2DLine(const std::vector<glm::vec2>&vertices, float width, glm::uint & index, bool recenter = true, bool triangulate = true);

    protected:
        struct FaceVertex
        {
            FaceVertex()
                : positionIndex(0)
                , textureCoordinateIndex(0)
                , normalIndex(0)
            {}

            int positionIndex;
            int textureCoordinateIndex;
            int normalIndex;
        };

        struct MeshData
        {
            MeshData()
                : vertexIndex(0)
                , vertexCount(0)
                , indexIndex(0)
                , indexCount(0)
            {
                maxVertexPosition.x = -1.0f * std::numeric_limits<float>::infinity();
                maxVertexPosition.y = -1.0f * std::numeric_limits<float>::infinity();
                maxVertexPosition.z = -1.0f * std::numeric_limits<float>::infinity();
                minVertexPosition.x = std::numeric_limits<float>::infinity();
                minVertexPosition.y = std::numeric_limits<float>::infinity();
                minVertexPosition.z = std::numeric_limits<float>::infinity();
            }

            glm::vec3 GetSize() const
            {
                return maxVertexPosition - minVertexPosition;
            }

            glm::uint vertexIndex;
            glm::uint vertexCount;
            glm::uint indexIndex;
            glm::uint indexCount;
            glm::vec3 maxVertexPosition;
            glm::vec3 minVertexPosition;
        };

        MeshVertex GetMeshVertexFromFaceVertex(
            const FaceVertex& faceVertex,
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            const std::vector<glm::vec3>& normals);

        std::vector<MeshVertex> meshVertexArray_;
        std::vector<glm::uint> meshIndexArray_;
        std::vector<MeshData> meshDataArray_;

    private:
    };
}