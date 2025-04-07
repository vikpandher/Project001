// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-07

#pragma once

#include "MeshData.h"

#include "glm/gtc/quaternion.hpp"

#include <string>



namespace Project001
{
    class MeshLoader
    {
    public:
        static bool LoadMeshOBJ(
            MeshData& meshData,
            const std::string& filePath,
            bool triangulate = s_triangulate);

        static bool LoadMeshOBJFromMemory(
            MeshData& meshData,
            const char* dataPtr, // must be null terminated
            bool triangulate = s_triangulate);

        static bool LoadTriangleMesh(
            MeshData& meshData,
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec3>& normals,
            const std::vector<glm::ivec2>& faces);

        static bool Generate2DTriangleFan(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions,
            bool triangulate = s_triangulate);

        static bool Generate2DTriangles(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions);

        static bool Generate2DTriangleStrip(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions,
            bool triangulate = s_triangulate);

        static bool Generate2DArc(
            MeshData& meshData,
            float innerRadius,
            float outerRadius,
            size_t subdivisions,
            float startAngle,
            float endAngle,
            bool triangulate = s_triangulate);

        static bool Generate2DArc(
            MeshData& meshData,
            const glm::vec2& focalPoint,
            const glm::vec2& arcCenterPoint,
            float arcLength,
            size_t subdivisions,
            float width,
            bool beveledCorners = true,
            bool triangulate = s_triangulate);

        static bool Generate2DCapsule(
            MeshData& meshData,
            float rectangleHeight,
            float capsuleWidth,
            size_t radialSections,
            bool triangulate = s_triangulate);

        static bool Generate2DCapsule(
            MeshData& meshData,
            const glm::vec2& start,
            const glm::vec2& end,
            float radius,
            size_t radialSections,
            bool triangulate = s_triangulate);

        static bool Generate2DLine(
            MeshData& meshData,
            const glm::vec2& start,
            const glm::vec2& end,
            float width,
            bool triangulate = s_triangulate);

        static bool Generate2DLine(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool triangulate = s_triangulate);

        // TODO:
        static bool Generate2DLine_v2(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions,
            float width,
            size_t endStyle = 0,
            size_t cornerStyle = 0,
            bool beveledCorners = true,
            bool triangulate = s_triangulate);

        static bool Generate2DLineLoop(
            MeshData& meshData,
            const std::vector<glm::vec2>& positions,
            float width,
            bool beveledCorners = true,
            bool triangulate = s_triangulate);

        static bool Generate2DPolygon(
            MeshData& meshData,
            const std::vector<glm::vec2>& corners,
            bool triangulate = s_triangulate);

        // Works a bit better
        static bool Generate2DPolygon_v2(
            MeshData& meshData,
            const std::vector<glm::vec2>& corners,
            bool triangulate = s_triangulate);

        static bool Generate2DRegularPolygon(
            MeshData& meshData,
            float radius,
            size_t sides,
            bool triangulate = s_triangulate);

        static bool Generate2DBezeledRectangle(
            MeshData& meshData,
            float width,
            float height,
            float bezelSize,
            size_t bezelSections,
            bool triangulate = s_triangulate);

        static bool Generate2DRectangleFrame(
            MeshData& meshData,
            const glm::vec2& bottomLeft,
            const glm::vec2& topRight,
            float frameThickness,
            bool triangulate = s_triangulate);

        // texture origin is bottom left
        static bool Generate2DSprite(
            MeshData& meshData,
            float width,
            float height,
            float textureLeft,
            float textureRight,
            float textureBottom,
            float textureTop,
            bool triangulate = s_triangulate);

        static bool GenerateBox(
            MeshData& meshData,
            const glm::vec3& min,
            const glm::vec3& max,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateBox(
            MeshData& meshData,
            float xLength,
            float yLength,
            float zLength,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateBoxWithTexture(
            MeshData& meshData,
            const glm::vec3& min,
            const glm::vec3& max,
            const std::vector<glm::vec2>& textureCoordinates,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateCapsule(
            MeshData& meshData,
            float cylindricalHeight,
            float radius,
            size_t faces,
            size_t stacks,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        // No texture
        static bool GenerateCapsule_v2(
            MeshData& meshData,
            float cylindricalHeight,
            float radius,
            size_t faces,
            size_t stacks,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateCone(
            MeshData& meshData,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        // No texture
        static bool GenerateCone_v2(
            MeshData& meshData,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateCylinder(
            MeshData& meshData,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        // No texture
        static bool GenerateCylinder_v2(
            MeshData& meshData,
            float height,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateHemisphere(
            MeshData& meshData,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateIcosphere(
            MeshData& meshData,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateIcosphere_v2(
            MeshData& meshData,
            float radius,
            size_t subdivisions,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateIcosphereAsteroid(
            MeshData& meshData,
            std::vector<glm::vec2>& borderPoints,
            float radius,
            uint32_t offsetSeed,
            float maxOffsetLength,
            size_t subdivisions,
            std::vector<float>& vertexOffsetDistances,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateSphere(
            MeshData& meshData,
            float radius,
            size_t longitudinalSections,
            size_t latitudinalSections,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static bool GenerateTriangles(
            MeshData& meshData,
            const std::vector<glm::vec3>& positions);

        static bool GenerateTube(
            MeshData& meshData,
            const glm::vec3& start,
            const glm::vec3& end,
            float radius,
            size_t faces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static void CopyMesh(
            MeshData& destinationMeshData,
            const MeshData& sourceMeshData);

        static void NormalizeMeshSize(MeshData& meshData);

        static void RecenterMesh(MeshData& meshData);

        static void RecalculateMeshMinMax(MeshData& meshData);

        static void RotateMesh(
            MeshData& meshData,
            glm::quat rotation);

        static void RotateMeshX(
            MeshData& meshData,
            float rotationInRadians);

        static void RotateMeshY(
            MeshData& meshData,
            float rotationInRadians);

        static void RotateMeshZ(
            MeshData& meshData,
            float rotationInRadians);

        static void ScaleMesh(
            MeshData& meshData,
            glm::vec3 scale);

        static void TranslateMesh(
            MeshData& meshData,
            glm::vec3 translation);

        static void SizeMeshAlongNormals(
            MeshData& meshData,
            float size);

        static void TurnInsideOut(
            MeshData& meshData,
            bool wasTriangulated = s_triangulate);

        // number of verticies depends on if mesh was triangulated
        static void ApplyTextureCoordinates(
            MeshData& meshData,
            const std::vector<glm::vec2>& textureCoordinates);

        static void ApplyPositionalTextureCoordinates(MeshData& meshData);

        static void RotateTextureCoordinates(
            MeshData& meshData,
            float rotation);

        static void ScaleTextureCoordinates(
            MeshData& meshData,
            glm::vec2 scale);

        static void TranslateTextureCoordinates(
            MeshData& meshData,
            glm::vec2 translation);

        // This makes it so verticies aren't reused by multiple indicies.
        // One vertex has one associated index.
        static bool s_triangulate;

    protected:
        // Used in LoadMeshOBJFromMemory
        static bool GetLineFromConstChar(
            const char*& incrementingPtr, // must be null terminated
            std::string& currentLine);

        // Used in LoadMeshOBJ and LoadMeshOBJFromMemory
        static bool ProcessMeshOBJLine(
            std::string& line,
            MeshData& meshData,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec2>& textureCoordinates,
            std::vector<glm::vec3>& normals,
            bool triangulate = s_triangulate);

        // Used in ProcessLineMeshOBJ
        static bool GetMeshVertexFromFaceVertex(
            MeshVertex& meshVertex,
            const glm::ivec3& faceVertex,
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec2>& textureCoordinates,
            const std::vector<glm::vec3>& normals);

        // Used in Generate2DLine
        static glm::vec2 GetLineLineIntersection2D(
            const glm::vec2& point1,
            const float& slope1,
            const glm::vec2& point2,
            const float& slope2);

        // Used in Generate2DPolygon and Generate2DPolygon_v2
        static void PopulateMeshDataWithIndicesAndVerticesFor2DPolygon(
            MeshData& meshData,
            const std::vector<glm::vec2>& vertices,
            const std::vector<size_t>& indices,
            bool triangulate = s_triangulate);

        // Used in GenerateIcosphere and GenerateIcosphere_v2
        static void SubdivideIcosphereTriangleFaces(
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec2>& textureCoordinates,
            std::vector<glm::uvec3>& triangleFaces,
            float radius,
            size_t subdivisions);

        // Used in GenerateIcosphereAsteroid
        // This version includes the offset seeds
        static void SubdivideIcosphereTriangleFaces_v2(
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec2>& textureCoordinates,
            std::vector<uint32_t>& offsetSeeds,
            std::vector<glm::uvec3>& triangleFaces,
            float radius,
            size_t subdivisions);

        // This version avoids duplicate positions, but doesn't handle textures.
        static void SubdivideIcosphereTriangleFaces_v3(
            std::vector<glm::vec3>& positions,
            std::vector<glm::uvec3>& triangleFaces,
            float radius,
            size_t subdivisions);

        // Used in GenerateIcosphere and GenerateIcosphere_v2
        // radius must be > 0
        static void GenerateIcosphereMeshVerticesAndIndices(
            MeshData& meshData,
            float radius,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec2>& textureCoordinates,
            std::vector<glm::uvec3>& triangleFaces,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        // Used in GenerateCapsule, GenerateCone, GenerateCylinder,
        // GenerateHemisphere, and GenerateSphere
        // longitudionalSections must be >= 3
        // latitudionalSections must be >= 2
        static void GenerateSphereMeshVerticesAndIndices(
            MeshData& meshData,
            size_t longitudinalSections,
            size_t latitudinalSections,
            std::vector<glm::vec3>& positions,
            std::vector<glm::vec2>& textureCoordinates,
            std::vector<glm::vec3>& normals,
            bool smoothNormals = true,
            bool triangulate = s_triangulate);

        static void RecalculateMeshMinMax_v2(MeshData& meshData);
    };
}