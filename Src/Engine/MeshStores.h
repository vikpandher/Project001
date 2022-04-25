#pragma once

#include <string>
#include <vector>

#include "glm/gtc/quaternion.hpp"

#include "Engine/MeshVertex.h"



namespace Project001
{
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
            const MeshVertex*& firstVertexPtr,
            unsigned int& vertexCount,
            const unsigned int*& firstIndexPtr,
            unsigned int& indexCount) const;

        bool GetMeshData(
            unsigned int index,
            MeshData& meshData) const;

        bool LoadMeshOBJ(
            unsigned int& index,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault);

        bool LoadTriangleMesh(
            unsigned int& index,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec3>& normals,
            std::vector<glm::ivec2>& faces,
            bool normalizeSize = true,
            bool recenter = true);

        bool Generate2DTriangleFan(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault);

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
            bool trangulate = s_triangulateByDefault);

        // Arc grows according to right hand rule
        bool Generate2DArc(
            unsigned int& index,
            float innerRadius,
            float outerRadius,
            size_t subdivisions,
            float startAngle,
            float endAngle,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        bool Generate2DCapsule(
            unsigned int& index,
            float rectangleHeight,
            float capsuleWidth,
            size_t radialSections,
            bool trangulate = s_triangulateByDefault);

        bool Generate2DLine(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        bool Generate2DLineLoop(
            unsigned int& index,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        bool Generate2DRegularPolygon(
            unsigned int& index,
            float radius,
            size_t sides,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        bool Generate2DBezeledRectangle(
            unsigned int& index,
            float width,
            float height,
            float bezelSize,
            float bezelSections,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        bool Generate2DSprite(
            unsigned int& index,
            float width,
            float height,
            float textureBottom,
            float textureTop,
            float textureLeft,
            float textureRight,
            bool trangulate = s_triangulateByDefault);

        bool GenerateBox(
            unsigned int& index,
            float xLength,
            float yLength,
            float zLength,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool GenerateCapsule(
            unsigned int& index,
            float cylindricalHeight,
            float radius,
            size_t faces,
            size_t stacks,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool GenerateCone(
            unsigned int& index,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool GenerateCylinder(
            unsigned int& index,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool GenerateIcosphere(
            unsigned int& index,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool GenerateSphere(
            unsigned int& index,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        bool NormalizeMeshSize(unsigned int& index);

        bool RecenterMesh(unsigned int& index);

        bool TranslateMesh(unsigned int& index, glm::vec3 translation);

        bool RotateMesh(unsigned int& index, glm::quat rotation);

        bool RotateMeshX(unsigned int& index, float rotationInRadians);

        bool RotateMeshY(unsigned int& index, float rotationInRadians);

        bool RotateMeshZ(unsigned int& index, float rotationInRadians);

        bool ScaleMesh(unsigned int& index, glm::vec3 scale);

        bool SizeMeshAlongNormals(unsigned int& index, float size);

        bool TurnInsideOut(unsigned int& index, bool wasTriangulated = true);

        static bool LoadMeshOBJ(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::string& path,
            bool normalizeSize = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault);

        static bool LoadTriangleMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec3>& normals,
            std::vector<glm::ivec2>& faces,
            bool normalizeSize = true,
            bool recenter = true);

        static bool Generate2DTriangleFan(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault);

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
            bool trangulate = s_triangulateByDefault);

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
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        static bool Generate2DCapsule(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float rectangleHeight,
            float capsuleWidth,
            size_t radialSections,
            bool trangulate = s_triangulateByDefault);

        static bool Generate2DLine(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        static bool Generate2DLineLoop(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        static bool Generate2DRegularPolygon(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t sides,
            bool recenter = true,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        static bool Generate2DBezeledRectangle(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float width,
            float height,
            float bezelSize,
            float bezelSections,
            bool trangulate = s_triangulateByDefault,
            bool positionalTexture = true);

        static bool Generate2DSprite(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float width,
            float height,
            float textureBottom,
            float textureTop,
            float textureLeft,
            float textureRight,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateBox(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float xLength,
            float yLength,
            float zLength,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateCapsule(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float cylindricalHeight,
            float radius,
            size_t faces,
            size_t stacks,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateCone(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateCylinder(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateIcosphere(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

        static bool GenerateSphere(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
            bool trangulate = s_triangulateByDefault);

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

        static void RotateMeshX(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            float rotationInRadians);

        static void RotateMeshY(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            float rotationInRadians);

        static void RotateMeshZ(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            float rotationInRadians);

        static void ScaleMesh(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            glm::vec3 scale);

        static void SizeMeshAlongNormals(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            float size);

        static void TurnInsideOut(
            MeshData& meshData,
            std::vector<MeshVertex>& meshVertexArray,
            std::vector<unsigned int>& meshIndexArray,
            bool wasTriangulated = s_triangulateByDefault);

    protected:
        // used by LoadMeshOBJ
        static bool GetMeshVertexFromFaceVertex(
            MeshVertex& meshVertex,
            const glm::ivec3& faceVertex,
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            const std::vector<glm::vec3>& normals);

        // used by Generate2DLine
        static glm::vec2 GetLineLineIntersection2d(
            const glm::vec2& point1,
            const float& slope1,
            const glm::vec2& point2,
            const float& slope2);

        static const bool s_triangulateByDefault = true;

        std::vector<MeshVertex> meshVertexArray_;
        std::vector<unsigned int> meshIndexArray_;
        std::vector<MeshData> meshDataArray_;

    private:
    };
}