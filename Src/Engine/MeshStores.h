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
            , textureCoordinate(0.0f)
            , normal(0.0f)
        {}

        glm::vec3 position;
        glm::vec2 textureCoordinate;
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

        unsigned int vertexIndex;
        unsigned int vertexCount;
        unsigned int indexIndex;
        unsigned int indexCount;
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
            unsigned int index,
            MeshVertex*& firstVertexPtr,
            unsigned int& vertexCount,
            unsigned int*& firstIndexPtr,
            unsigned int& indexCount);

        bool GetMeshData(
            unsigned int index,
            MeshData& meshData);

        bool LoadMeshOBJ(
            unsigned int& index,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DTriangleFan(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DTriangles(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true);

        bool Generate2DTriangleStrip(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        bool Generate2DLine(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            float width,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        bool Generate2DRegularPolygon(
            unsigned int& index,
            float radius,
            size_t sides,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        // Arc grows according to right hand rule
        bool Generate2DArc(
            unsigned int& index,
            float innerRadius,
            float outerRadius,
            size_t subdivisions,
            float startAngle,
            float endAngle,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        bool GenerateBox(
            unsigned int& index,
            float xLength,
            float yLength,
            float zLength,
            bool triangulate = true);

        bool GenerateIcosphere(
            unsigned int& index,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool triangulate = true);

        bool GenerateSphere(
            unsigned int& index,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
            bool triangulate = true);

        bool NormalizeMeshSize(unsigned int& index);

        bool RecenterMesh(unsigned int& index);

        bool TranslateMesh(unsigned int& index, glm::vec3 translation);

        bool RotateMesh(unsigned int& index, glm::quat rotation);

        bool ScaleMesh(unsigned int& index, glm::vec3 scale);

        static bool LoadMeshOBJ(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DTriangleFan(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DTriangles(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true);

        static bool Generate2DTriangleStrip(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool triangulate = true);

        static bool Generate2DLine(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            float width,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static bool Generate2DRegularPolygon(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t sides,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static bool Generate2DArc(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float innerRadius,
            float outerRadius,
            size_t subdivisions,
            float startAngle,
            float endAngle,
            bool recenter = true,
            bool triangulate = true,
            bool positionalTexture = true);

        static bool GenerateBox(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float xLength,
            float yLength,
            float zLength,
            bool triangulate = true);

        static bool GenerateIcosphere(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool triangulate = true);

        static bool GenerateSphere(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
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
        std::vector<unsigned int> meshIndexArray_;
        std::vector<MeshData> meshDataArray_;

    private:
    };
}