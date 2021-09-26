#pragma once

#include <string>
#include <vector>

#include "glm/gtc/quaternion.hpp"



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

    class MeshStores
    {
    public:
        MeshStores();

        ~MeshStores();

        void ClearMeshes();

        // If more meshes are added, the pointers returned are nolonger valid.
        bool GetMesh(
            glm::uint index,
            MeshVertex*& firstVertexPtr,
            glm::uint& vertexCount,
            glm::uint*& firstIndexPtr,
            glm::uint& indexCount);

        bool GetMeshData(
            glm::uint index,
            MeshData& meshData);

        bool LoadMeshOBJ(
            glm::uint& index,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DTriangleFan(
            glm::uint& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DTriangles(
            glm::uint& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true);

        bool Generate2DTriangleStrip(
            glm::uint& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DLine(
            glm::uint& index,
            const std::vector<glm::vec2>& positions,
            float width,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        bool Generate2DRegularPolygon(
            glm::uint& index,
            float radius,
            glm::uint sides,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        // Arc grows according to right hand rule
        bool Generate2DArc(
            glm::uint& index,
            float innerRadius,
            float outerRadius,
            glm::uint subdivisions,
            float startAngle,
            float endAngle,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        void GenerateBox(
            glm::uint& index,
            float xLength,
            float yLength,
            float zLength,
            bool triangulate = true);

        bool NormalizeMeshSize(glm::uint& index);

        bool RecenterMesh(glm::uint& index);

        bool TranslateMesh(glm::uint& index, glm::vec3 translation);

        bool RotateMesh(glm::uint& index, glm::quat rotation);

        bool ScaleMesh(glm::uint& index, glm::vec3 scale);

        static bool LoadMeshOBJ(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DTriangleFan(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DTriangles(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true);

        static bool Generate2DTriangleStrip(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DLine(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            float width,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static bool Generate2DRegularPolygon(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            float radius,
            glm::uint sides,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static bool Generate2DArc(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            float innerRadius,
            float outerRadius,
            glm::uint subdivisions,
            float startAngle,
            float endAngle,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static void GenerateBox(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<glm::uint>& meshIndexArray,
            float xLength,
            float yLength,
            float zLength,
            bool triangulate = true);

        static void NormalizeMeshSize(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray);

        static void RecenterMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray);

        static void TranslateMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            glm::vec3 translation);

        static void RotateMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            glm::quat rotation);

        static void ScaleMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            glm::vec3 scale);

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

        // used by LoadMeshOBJ
        static bool GetMeshVertexFromFaceVertex(
            MeshVertex& meshVertex,
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