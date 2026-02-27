// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-02-26

#pragma once

#include "MeshData.h"

#include "glm/gtc/quaternion.hpp"

#include <string>



namespace Project001
{
namespace Mesh
{
    // This makes it so vertices aren't reused by multiple indices.
    // Each vertex has only one associated index. Vertices will repeat.
    extern bool g_triangulate;

    bool LoadMeshOBJ(
        MeshData& meshData,
        const std::string& filePath,
        bool triangulate = g_triangulate);

    bool LoadMeshOBJFromMemory(
        MeshData& meshData,
        const unsigned char* dataPtr, // must be null terminated
        bool triangulate = g_triangulate);

    bool LoadMeshOBJFromMemory(
        MeshData& meshData,
        const char* dataPtr, // must be null terminated
        bool triangulate = g_triangulate);

    bool LoadTriangleMesh(
        MeshData& meshData,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::ivec2>& faces);

    bool WriteMeshOBJ(
        MeshData& meshData,
        const std::string& filePath);

    bool Generate2DTriangleFan(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        bool triangulate = g_triangulate);

    bool Generate2DTriangles(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions);

    bool Generate2DTriangleStrip(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        bool triangulate = g_triangulate);

    bool Generate2DTriangleStrip(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool triangulate = g_triangulate);

    bool Generate2DArc(
        MeshData& meshData,
        float innerRadius,
        float outerRadius,
        size_t subdivisions,
        float startAngle,
        float endAngle,
        bool triangulate = g_triangulate);

    bool Generate2DArc(
        MeshData& meshData,
        const glm::vec2& focalPoint,
        const glm::vec2& arcCenterPoint,
        float arcLength,
        size_t subdivisions,
        float width,
        bool beveledCorners = true,
        bool triangulate = g_triangulate);

    bool Generate2DCapsule(
        MeshData& meshData,
        float rectangleHeight,
        float capsuleWidth,
        size_t radialSections,
        bool triangulate = g_triangulate);

    bool Generate2DCapsule(
        MeshData& meshData,
        const glm::vec2& start,
        const glm::vec2& end,
        float radius,
        size_t radialSections,
        bool triangulate = g_triangulate);

    bool Generate2DDashedLine(
        MeshData& meshData,
        const glm::vec2& start,
        const glm::vec2& direction,
        float dashLength,
        float gapLength,
        float width,
        size_t dashCount,
        bool gapsIncludedInTextureCoordinateCalculation = false,
        bool triangulate = g_triangulate);

    bool Generate2DLine(
        MeshData& meshData,
        const glm::vec2& start,
        const glm::vec2& end,
        float width,
        bool triangulate = g_triangulate);

    bool Generate2DLine(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners = true,
        bool triangulate = g_triangulate);

    // TODO:
    bool Generate2DLine_v2(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        size_t endStyle = 0,
        size_t cornerStyle = 0,
        bool beveledCorners = true,
        bool triangulate = g_triangulate);

    bool Generate2DLineLoop(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners = true,
        bool triangulate = g_triangulate);

    bool Generate2DPolygon(
        MeshData& meshData,
        const std::vector<glm::vec2>& corners,
        bool triangulate = g_triangulate);

    // Works a bit better
    bool Generate2DPolygon_v2(
        MeshData& meshData,
        const std::vector<glm::vec2>& corners,
        bool triangulate = g_triangulate);

    bool Generate2DQuads(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        bool triangulate = g_triangulate);

    bool Generate2DRectangle(
        MeshData& meshData,
        float width,
        float height,
        bool triangulate = g_triangulate);

    bool Generate2DRegularPolygon(
        MeshData& meshData,
        float radius,
        size_t sides,
        bool triangulate = g_triangulate);

    bool Generate2DBezeledRectangle(
        MeshData& meshData,
        float width,
        float height,
        float bezelSize,
        size_t bezelSections,
        bool triangulate = g_triangulate);

    bool Generate2DRectangleFrame(
        MeshData& meshData,
        const glm::vec2& bottomLeft,
        const glm::vec2& topRight,
        float frameThickness,
        bool triangulate = g_triangulate);

    // texture origin is bottom left
    bool Generate2DSprite(
        MeshData& meshData,
        float width,
        float height,
        float textureLeft,
        float textureRight,
        float textureBottom,
        float textureTop,
        bool triangulate = g_triangulate);

    // texture origin is bottom left
    bool Generate2DSprite(
        MeshData& meshData,
        const glm::vec3& bottomLeft,
        const glm::vec3& bottomRight,
        const glm::vec3& topRight,
        const glm::vec3& topLeft,
        float textureLeft,
        float textureRight,
        float textureBottom,
        float textureTop,
        bool triangulate = g_triangulate);

    bool Generate2DStarBurst(
        MeshData& meshData,
        size_t points,
        float radius0,
        float radius1,
        float radius2,
        float radius3,
        float sectionAngle,
        uint32_t offsetSeed,
        float maxOffset0, // offsets go inward
        float maxOffset1,
        float maxOffset2,
        float maxOffset3,
        bool triangulate = g_triangulate);

    bool Generate2DStarRing(
        MeshData& meshData,
        size_t points,
        float radius0,
        float radius1,
        float radius2,
        float radius3,
        uint32_t offsetSeed,
        float maxOffset0, // offsets go inward
        float maxOffset1,
        float maxOffset2,
        float maxOffset3,
        bool triangulate = g_triangulate);

    bool GenerateBox(
        MeshData& meshData,
        const glm::vec3& min,
        const glm::vec3& max,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateBox(
        MeshData& meshData,
        float xLength,
        float yLength,
        float zLength,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateBoxWithTexture(
        MeshData& meshData,
        const glm::vec3& min,
        const glm::vec3& max,
        const std::vector<glm::vec2>& textureCoordinates,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateCapsule(
        MeshData& meshData,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // No texture
    bool GenerateCapsule_v2(
        MeshData& meshData,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateCone(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // No texture
    bool GenerateCone_v2(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateCylinder(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // No texture
    bool GenerateCylinder_v2(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateHemisphere(
        MeshData& meshData,
        float radius,
        size_t longitudinalSections,
        size_t latitudinalSections,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateIceCreamCone(
        MeshData& meshData,
        float coneHeight, // without scoop
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateIceCreamCup(
        MeshData& meshData,
        float cupHeight, // without scoop
        float radius0,
        float radius1,
        size_t faces,
        size_t stacks,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateIcosphere(
        MeshData& meshData,
        float radius,
        size_t subdivisions,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateIcosphere_v2(
        MeshData& meshData,
        float radius,
        size_t subdivisions,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateIcosphereAsteroid(
        MeshData& meshData,
        std::vector<glm::vec2>& borderPoints,
        float radius,
        uint32_t offsetSeed,
        float maxOffsetLength, // offset goes inward
        size_t subdivisions,
        std::vector<float>& vertexOffsetDistances,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateTruncatedCone(
        MeshData& meshData,
        float height,
        float radius0,
        float radius1,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateSpikeyCrown(
        MeshData& meshData,
        float radius0, // for spiike 0 & 2
        float radius1, // for spiike 1 & 3
        size_t points,
        float spikeHeight0, // heights are along +y axis
        float spikeHeight1,
        float spikeHeight2,
        float spikeHeight3,
        uint32_t offsetSeed,
        float maxOffset0, // offsets are along +y axis
        float maxOffset1,
        float maxOffset2,
        float maxOffset3,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateSphere(
        MeshData& meshData,
        float radius,
        size_t longitudinalSections,
        size_t latitudinalSections,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // The longitudeAngle goes from 0 to 2pi counter clockwise around the
    // sphere looking down from the top. 0 starts at the +z axis.
    // The latitudeAngle goes from 0 to pi from the top of the sphere to the
    // bottom.
    // The min/max longitude/latitude angles are bound within these ranges.
    bool GenerateSphereSection(
        MeshData& meshData,
        float radius,
        size_t longitudinalSections,
        size_t latitudinalSections,
        float minLongitudeAngle,
        float maxLongitudeAngle,
        float minLatitudeAngle,
        float maxLatitudeAngle,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    bool GenerateTriangles(
        MeshData& meshData,
        const std::vector<glm::vec3>& positions);

    bool GenerateTube(
        MeshData& meshData,
        const glm::vec3& start,
        const glm::vec3& end,
        float radius,
        size_t faces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    void CopyMesh(
        MeshData& destinationMeshData,
        const MeshData& sourceMeshData);

    void NormalizeMeshSize(MeshData& meshData);

    void RecenterMesh(MeshData& meshData);

    void RecalculateMeshMinMax(MeshData& meshData);

    void RotateMesh(
        MeshData& meshData,
        glm::quat rotation);

    void RotateMeshX(
        MeshData& meshData,
        float rotationInRadians);

    void RotateMeshY(
        MeshData& meshData,
        float rotationInRadians);

    void RotateMeshZ(
        MeshData& meshData,
        float rotationInRadians);

    void ScaleMesh(
        MeshData& meshData,
        glm::vec3 scale);

    void TranslateMesh(
        MeshData& meshData,
        glm::vec3 translation);

    void SizeMeshAlongNormals(
        MeshData& meshData,
        float size);

    void TurnInsideOut(
        MeshData& meshData,
        bool wasTriangulated = g_triangulate);

    // number of vertices depends on if mesh was triangulated
    void ApplyTextureCoordinates(
        MeshData& meshData,
        const std::vector<glm::vec2>& textureCoordinates);

    void ApplyPositionalTextureCoordinates(MeshData& meshData);

    void RotateTextureCoordinates(
        MeshData& meshData,
        float rotation);

    void ScaleTextureCoordinates(
        MeshData& meshData,
        glm::vec2 scale);

    void SliceMeshWithAPlane(
        MeshData& outputMeshData0, // dot(N,p)-d >= 0
        MeshData& outputMeshData1, // dot(N,p)-d < 0
        std::vector<glm::vec3>& capCorners, // raw independent intersection points (unordered)
        const MeshData& inputMeshData,
        const glm::vec3& plane_normal, // expected normalized
        const float& plane_distance,
        float mergeEpsilon = 1e-6f);

    void TranslateTextureCoordinates(
        MeshData& meshData,
        glm::vec2 translation);

    // Helper Functions --------------------------------------------------------

    // Used in LoadMeshOBJFromMemory
    bool GetLineFromConstChar_H(
        const char*& incrementingPtr, // must be null terminated
        std::string& currentLine);

    // Used in LoadMeshOBJ and LoadMeshOBJFromMemory
    bool ProcessMeshOBJLine_H(
        std::string& line,
        MeshData& meshData,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::vec3>& normals,
        bool triangulate = g_triangulate);

    // Used in ProcessLineMeshOBJ
    bool GetMeshVertexFromFaceVertex_H(
        MeshVertex& meshVertex,
        const glm::ivec3& faceVertex,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        const std::vector<glm::vec3>& normals);

    // Used in Generate2DLine
    glm::vec2 GetLineLineIntersection2D_H(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2);

    // Used in Generate2DPolygon and Generate2DPolygon_v2
    void PopulateMeshDataWithIndicesAndVerticesFor2DPolygon_H(
        MeshData& meshData,
        const std::vector<glm::vec2>& vertices,
        const std::vector<size_t>& indices,
        bool triangulate = g_triangulate);

    // Used in GenerateIcosphere and GenerateIcosphere_v2
    void SubdivideIcosphereTriangleFaces_H(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions);

    // Used in GenerateIcosphereAsteroid
    // This version includes the offset seeds
    void SubdivideIcosphereTriangleFaces_H2(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<uint32_t>& offsetSeeds,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions);

    // This version avoids duplicate positions, but doesn't handle textures.
    void SubdivideIcosphereTriangleFaces_H3(
        std::vector<glm::vec3>& positions,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions);

    // Used in GenerateIcosphere and GenerateIcosphere_v2
    // radius must be > 0
    void GenerateIcosphereMeshVerticesAndIndices_H(
        MeshData& meshData,
        float radius,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::uvec3>& triangleFaces,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // Used in GenerateCapsule, GenerateCone, GenerateCylinder,
    // GenerateHemisphere, and GenerateSphere
    // longitudionalSections must be >= 3
    // latitudionalSections must be >= 2
    void GenerateSphereMeshVerticesAndIndices_H(
        MeshData& meshData,
        size_t longitudinalSections,
        size_t latitudinalSections,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::vec3>& normals,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);

    // Used in GenerateSpikeyCrown
    // sections must be >= 2
    void GenerateMeshStripVerticesAndIndices_H(
        MeshData& meshData,
        size_t sections,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::vec3>& normals,
        bool smoothNormals = true,
        bool triangulate = g_triangulate);
}
}