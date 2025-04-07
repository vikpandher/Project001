// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-07

#include "MeshLoader.h"

#include "Math/MathUtilities.h"

#include <algorithm> // for std::sort
#include <fstream>



namespace Project001
{
    // public ------------------------------------------------------------------

    bool MeshLoader::LoadMeshOBJ(
        MeshData& meshData,
        const std::string& filePath,
        bool triangulate)
    {
        std::ifstream inputFileStream(filePath);

        if (!inputFileStream.is_open())
        {
            return false;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        std::string currentLine;
        while (std::getline(inputFileStream, currentLine))
        {
            if (!ProcessMeshOBJLine(
                currentLine,
                meshData,
                positions,
                textureCoordinates,
                normals,
                triangulate))
            {
                return false;
            }
        }

        if (positions.size() == 0)
        {
            return false;
        }

        return true;
    }

    bool MeshLoader::LoadMeshOBJFromMemory(
        MeshData& meshData,
        const char* dataPtr,
        bool triangulate)
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        std::string currentLine;
        while (GetLineFromConstChar(dataPtr, currentLine))
        {
            if (!ProcessMeshOBJLine(
                currentLine,
                meshData,
                positions,
                textureCoordinates,
                normals,
                triangulate))
            {
                return false;
            }
        }

        if (positions.size() == 0)
        {
            return false;
        }

        return true;
    }

    bool MeshLoader::LoadTriangleMesh(
        MeshData& meshData,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::ivec2>& faces)
    {
        if (faces.empty())
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        for (size_t i = 0; i < faces.size(); ++i)
        {
            const int& positionIndex = faces[i].x;
            const int& normalIndex = faces[i].y;

            if (positionIndex < 0 || positionIndex >= (int)positions.size() ||
                normalIndex < 0 || normalIndex >= (int)normals.size())
            {
                return false;
            }

            MeshVertex meshVertex;
            meshVertex.position = positions[positionIndex];
            meshVertex.normal = normals[normalIndex];

            meshIndexArray.push_back((unsigned int)meshVertexArray.size());
            meshVertexArray.push_back(meshVertex);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        if (meshVertexArray.size() == 0 || meshIndexArray.size() == 0)
        {
            return false;
        }

        return true;
    }

    bool MeshLoader::Generate2DTriangleFan(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            size_t currentVertexCount = meshVertexArray.size();

            for (size_t i = 0; i < positions.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(positions[i], 0.0f);
                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                meshIndexArray.push_back((unsigned int)currentVertexCount);
                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)i + 1);
                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)i + 2);
            }
        }
        else
        {
            for (size_t i = 1; i < positions.size() - 1; ++i)
            {
                MeshVertex centerMeshVertex;
                centerMeshVertex.position = glm::vec3(positions[0], 0.0f);
                centerMeshVertex.normal = normal;

                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);
                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);
                meshVertexB.normal = normal;

                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(centerMeshVertex);
                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(meshVertexA);
                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(meshVertexB);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec2& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DTriangles(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions)
    {
        if (positions.size() == 0 || positions.size() % 3 != 0)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        size_t currentVertexCount = meshVertexArray.size();
        for (size_t i = 0; i < positions.size(); ++i)
        {
            MeshVertex meshVertex;
            meshVertex.position = glm::vec3(positions[i], 0.0f);
            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((unsigned int)(currentVertexCount + i));
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec2& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DTriangleStrip(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            size_t currentVertexCount = meshVertexArray.size();

            for (size_t i = 0; i < positions.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(positions[i], 0.0f);
                meshVertex.normal = normal;

                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                if (i % 2 == 0)
                {
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i));
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 1));
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 2));
                }
                else
                {
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 2));
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 1));
                    meshIndexArray.push_back((unsigned int)(currentVertexCount + i));
                }
            }
        }
        else
        {
            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);
                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);
                meshVertexB.normal = normal;

                MeshVertex meshVertexC;
                meshVertexC.position = glm::vec3(positions[i + 2], 0.0f);
                meshVertexC.normal = normal;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                if (i % 2 == 0)
                {
                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);
                }
                else
                {
                    meshVertexArray.push_back(meshVertexC);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexA);
                }

                meshIndexArray.push_back(currentVertexCount++);
                meshIndexArray.push_back(currentVertexCount++);
                meshIndexArray.push_back(currentVertexCount);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec2& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DArc(
        MeshData& meshData,
        float innerRadius,
        float outerRadius,
        size_t subdivisions,
        float startAngle,
        float endAngle,
        bool triangulate)
    {
        if (subdivisions == 0 ||
            innerRadius >= outerRadius)
        {
            return false;
        }

        startAngle = std::fmodf(startAngle, 2.0f * glm::pi<float>());
        endAngle = std::fmodf(endAngle, 2.0f * glm::pi<float>());

        if (startAngle < 0.0f)
        {
            startAngle += 2.0f * glm::pi<float>();
        }

        if (endAngle < 0.0f)
        {
            endAngle += 2.0f * glm::pi<float>();
        }

        if (startAngle >= endAngle)
        {
            endAngle += 2.0f * glm::pi<float>();
        }

        float arcRadius = endAngle - startAngle;

        float sectionAngle = arcRadius / (float)subdivisions;

        if (sectionAngle > 2.0f * glm::pi<float>())
        {
            sectionAngle = 2.0f * glm::pi<float>();
        }

        if ((subdivisions == 1 && sectionAngle >= glm::pi<float>()) ||
            (subdivisions < 3 && sectionAngle == 2.0f * glm::pi<float>()))
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        glm::vec2 innerRadialVector(0.0f, innerRadius);
        innerRadialVector = Rotate2DVector(innerRadialVector, startAngle);
        positions.push_back(innerRadialVector);

        glm::vec2 outerRadialVector(0.0f, outerRadius);
        outerRadialVector = Rotate2DVector(outerRadialVector, startAngle);
        positions.push_back(outerRadialVector);

        for (size_t i = 0; i < subdivisions; ++i)
        {
            innerRadialVector = Rotate2DVector(innerRadialVector, sectionAngle);
            positions.push_back(innerRadialVector);

            outerRadialVector = Rotate2DVector(outerRadialVector, sectionAngle);
            positions.push_back(outerRadialVector);
        }

        return Generate2DTriangleStrip(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DArc(
        MeshData& meshData,
        const glm::vec2& focalPoint,
        const glm::vec2& arcCenterPoint,
        float arcLength,
        size_t subdivisions,
        float width,
        bool beveledCorners,
        bool triangulate)
    {
        if (subdivisions == 0 ||
            arcLength <= 0.0f)
        {
            return false;
        }

        glm::vec2 centerToArc = arcCenterPoint - focalPoint;
        float arcRadius = glm::sqrt(centerToArc.x * centerToArc.x + centerToArc.y * centerToArc.y);

        float theta = arcLength / arcRadius;

        centerToArc = Rotate2DVector(centerToArc, theta * 0.5f);

        float segmentTheta = -1.0f * theta / (float)subdivisions;

        std::vector<glm::vec2> positions;
        positions.push_back(centerToArc + focalPoint);
        for (size_t i = 0; i < subdivisions; ++i)
        {
            centerToArc = Rotate2DVector(centerToArc, segmentTheta);
            positions.push_back(centerToArc + focalPoint);
        }

        return Generate2DLine(meshData, positions, width, beveledCorners, triangulate);
    }

    bool MeshLoader::Generate2DCapsule(
        MeshData& meshData,
        float rectangleHeight,
        float capsuleWidth,
        size_t radialSections,
        bool triangulate)
    {
        if (rectangleHeight < 0.0f || capsuleWidth < 0.0f || radialSections < 2)
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        float radialStep = glm::pi<float>() / (float)radialSections;
        float halfHeight = rectangleHeight * 0.5f;
        float radius = capsuleWidth * 0.5f;

        for (size_t i = 0; i < radialSections + 1; ++i)
        {
            float stackAngle = radialStep * (float)i;
            float x = radius * glm::cos(stackAngle);
            float y = radius * glm::sin(stackAngle);

            positions.emplace_back(x, y + halfHeight);
        }

        for (size_t i = 0; i < radialSections + 1; ++i)
        {
            float stackAngle = glm::pi<float>() + radialStep * (float)i;
            float x = radius * glm::cos(stackAngle);
            float y = radius * glm::sin(stackAngle);

            positions.emplace_back(x, y - halfHeight);
        }

        return Generate2DTriangleFan(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DCapsule(
        MeshData& meshData,
        const glm::vec2& start,
        const glm::vec2& end,
        float radius,
        size_t radialSections,
        bool triangulate)
    {
        if (radius < 0.0f || radialSections < 2)
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        float radialStep = glm::pi<float>() / (float)radialSections;

        glm::vec2 direction = end - start;

        if (direction.x == 0.0f && direction.y == 0.0f)
        {
            // circle
            direction.x = 1.0f;
        }

        glm::vec2 offset(-1.0f * direction.y, direction.x);
        float magnitude = glm::sqrt(offset.x * offset.x + offset.y * offset.y);
        glm::vec2 scaledOffset = offset / magnitude * radius;

        positions.emplace_back(start + scaledOffset);

        for (size_t i = 0; i < radialSections; ++i)
        {
            scaledOffset = Rotate2DVector(scaledOffset, radialStep);
            positions.emplace_back(start + scaledOffset);
        }

        positions.emplace_back(end + scaledOffset);

        for (size_t i = 0; i < radialSections; ++i)
        {
            scaledOffset = Rotate2DVector(scaledOffset, radialStep);
            positions.emplace_back(end + scaledOffset);
        }

        return Generate2DTriangleFan(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DLine(
        MeshData& meshData,
        const glm::vec2& start,
        const glm::vec2& end,
        float width,
        bool triangulate)
    {
        glm::vec2 direction = end - start;
        if (direction.x == 0.0f && direction.y == 0.0f)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 offset(-1.0f * direction.y, direction.x);
        float magnitude = glm::sqrt(offset.x * offset.x + offset.y * offset.y);
        glm::vec2 scaled = offset / magnitude * width / 2.0f;

        const size_t positionsSize = 4;
        glm::vec2 positions[positionsSize] = {
            start + scaled,
            start - scaled,
            end - scaled,
            end + scaled
        };

        if (!triangulate)
        {
            size_t currentVertexCount = meshVertexArray.size();

            for (size_t i = 0; i < positionsSize; ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(positions[i], 0.0f);
                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < positionsSize - 2; ++i)
            {
                meshIndexArray.push_back((unsigned int)currentVertexCount);
                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)i + 1);
                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)i + 2);
            }
        }
        else
        {
            for (size_t i = 1; i < positionsSize - 1; ++i)
            {
                MeshVertex centerMeshVertex;
                centerMeshVertex.position = glm::vec3(positions[0], 0.0f);
                centerMeshVertex.normal = normal;

                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);
                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);
                meshVertexB.normal = normal;

                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(centerMeshVertex);
                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(meshVertexA);
                meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                meshVertexArray.push_back(meshVertexB);
            }
        }

        for (size_t i = 0; i < positionsSize; ++i)
        {
            const glm::vec2& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DLine(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool triangulate)
    {
        if (positions.size() < 2)
        {
            return false;
        }

        // ignore duplicate positions at the start
        size_t startIndex = 1;
        while (positions[0] == positions[startIndex])
        {
            startIndex++;
            if (startIndex == positions.size())
            {
                return false;
            }
        }

        // ignore duplicate positions at the end
        size_t endIndex = positions.size() - 1;
        while (positions[endIndex - 1] == positions[endIndex])
        {
            endIndex--;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        size_t initialVertexCount = meshVertexArray.size();

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 position1 = positions[0];
        glm::vec2 position2 = positions[startIndex];

        glm::vec2 direction1 = position2 - position1;

        glm::vec2 offset01(-1.0f * direction1.y, direction1.x);
        float magnitude01 = glm::sqrt(offset01.x * offset01.x + offset01.y * offset01.y);
        glm::vec2 scaled1 = offset01 / magnitude01 * width / 2.0f;

        glm::vec2 scaled2 = scaled1;

        glm::vec2 positionA = position1 + scaled1;
        glm::vec2 positionB = position1 - scaled1;
        glm::vec2 positionC;
        glm::vec2 positionD;

        float slope1 = direction1.y / direction1.x;

        MeshVertex meshVertexA;
        meshVertexA.normal = normal;
        MeshVertex meshVertexB;
        meshVertexB.normal = normal;
        MeshVertex meshVertexC;
        meshVertexC.normal = normal;
        MeshVertex meshVertexD;
        meshVertexD.normal = normal;

        for (size_t i = startIndex + 1; i <= endIndex; ++i)
        {
            // ignore duplicate positions in the middle
            if (positions[i] == position2)
            {
                continue;
            }

            glm::vec2 position3 = positions[i];
            glm::vec2 direction2 = position3 - position2;
            float slope2 = direction2.y / direction2.x;

            // if (slope1 != slope2)
            if (glm::normalize(direction1) != glm::normalize(direction2))
            {
                glm::vec2 offset02 = glm::vec2(-1.0f * direction2.y, direction2.x);
                float magnitude02 = glm::sqrt(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                glm::vec2 positionE = position2 + scaled2;
                glm::vec2 positionF = position2 - scaled2;

                bool sharpCorner = false;
                float angle = Get2DVectorAngle(direction1, direction2);
                if (beveledCorners)
                {
                    if ((angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                        && angle != glm::pi<float>())
                    {
                        sharpCorner = true;
                    }
                }

                if (sharpCorner)
                {
                    offset01 = glm::vec2(-1.0f * direction1.y, direction1.x);
                    magnitude01 = glm::sqrt(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        positionC = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
                        positionD = position2 - scaled1;
                    }
                    else
                    {
                        positionC = position2 + scaled1;
                        positionD = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
                    }
                }
                else if (angle == glm::pi<float>())
                {
                    positionC = position2 - scaled2;
                    positionD = position2 + scaled2;
                }
                else
                {
                    positionC = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
                    positionD = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
                }

                meshVertexA.position = glm::vec3(positionA, 0.0f);
                meshVertexB.position = glm::vec3(positionB, 0.0f);
                meshVertexC.position = glm::vec3(positionC, 0.0f);
                meshVertexD.position = glm::vec3(positionD, 0.0f);

                glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);
                glm::vec3 normalDCB = glm::cross(meshVertexC.position - meshVertexD.position, meshVertexB.position - meshVertexD.position);

                float dotABC = glm::dot(normal, normalABC);
                float dotDCB = glm::dot(normal, normalDCB);

                if (!triangulate)
                {
                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);
                    meshVertexArray.push_back(meshVertexD);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount + 3);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 3);
                    }

                    currentVertexCount += 4;
                }
                else
                {
                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    if (dotABC > 0.0f)
                    {
                        meshVertexArray.push_back(meshVertexA);
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexC);
                    }
                    // When the width of the line is too large, triangles can 
                    // flip.
                    else
                    {
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexA);
                    }

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);

                    if (dotDCB > 0.0f)
                    {
                        meshVertexArray.push_back(meshVertexD);
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexB);
                    }
                    else
                    {
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexD);
                    }

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                }

                if (sharpCorner)
                {
                    meshVertexA.position = glm::vec3(positionC, 0.0f);
                    meshVertexB.position = glm::vec3(positionD, 0.0f);

                    if (angle > 0.0f) // turn to the left
                    {
                        meshVertexC.position = glm::vec3(positionF, 0.0f);
                    }
                    else
                    {
                        meshVertexC.position = glm::vec3(positionE, 0.0f);
                    }

                    glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);

                    float dotABC = glm::dot(normal, normalABC);

                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount);
                    }

                    if (angle > 0.0f) // turn to the left
                    {
                        positionA = positionC;
                        positionB = positionF;
                    }
                    else
                    {
                        positionA = positionE;
                        positionB = positionD;
                    }
                }
                else if (angle == glm::pi<float>())
                {
                    positionA = positionD;
                    positionB = positionC;
                }
                else
                {
                    positionA = positionC;
                    positionB = positionD;
                }

                position1 = position2;
                direction1 = direction2;
                slope1 = slope2;
            }

            position2 = position3;
        }

        positionC = position2 + scaled2;
        positionD = position2 - scaled2;

        meshVertexA.position = glm::vec3(positionA, 0.0f);
        meshVertexB.position = glm::vec3(positionB, 0.0f);
        meshVertexC.position = glm::vec3(positionC, 0.0f);
        meshVertexD.position = glm::vec3(positionD, 0.0f);

        glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);
        glm::vec3 normalDCB = glm::cross(meshVertexC.position - meshVertexD.position, meshVertexB.position - meshVertexD.position);

        float dotABC = glm::dot(normal, normalABC);
        float dotDCB = glm::dot(normal, normalDCB);

        if (!triangulate)
        {
            unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

            meshVertexArray.push_back(meshVertexA);
            meshVertexArray.push_back(meshVertexB);
            meshVertexArray.push_back(meshVertexC);
            meshVertexArray.push_back(meshVertexD);

            if (dotABC > 0.0f)
            {
                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }
            else
            {
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount);
            }

            if (dotDCB > 0.0f)
            {
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 1);
            }
            else
            {
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 3);
            }
        }
        else
        {
            unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

            if (dotABC > 0.0f)
            {
                meshVertexArray.push_back(meshVertexA);
                meshVertexArray.push_back(meshVertexB);
                meshVertexArray.push_back(meshVertexC);
            }
            else
            {
                meshVertexArray.push_back(meshVertexC);
                meshVertexArray.push_back(meshVertexB);
                meshVertexArray.push_back(meshVertexA);
            }

            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);

            if (dotDCB > 0.0f)
            {
                meshVertexArray.push_back(meshVertexD);
                meshVertexArray.push_back(meshVertexC);
                meshVertexArray.push_back(meshVertexB);
            }
            else
            {
                meshVertexArray.push_back(meshVertexB);
                meshVertexArray.push_back(meshVertexC);
                meshVertexArray.push_back(meshVertexD);
            }

            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
        }

        for (size_t i = initialVertexCount; i < meshVertexArray.size(); ++i)
        {
            const glm::vec3& currentPosition = meshVertexArray[i].position;
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    // TODO:
    bool MeshLoader::Generate2DLine_v2(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        size_t endStyle,
        size_t cornerStyle,
        bool beveledCorners,
        bool triangulate)
    {
        // ignore duplicate positions at the start
        size_t startIndex = 0;
        while (startIndex + 1 < positions.size() &&
            positions[startIndex] == positions[startIndex + 1])
        {
            startIndex++;
        }

        // ignore duplicate positions at the end
        size_t endIndex = positions.size() - 1;
        while (endIndex - 1 > -1 &&
            positions[endIndex] == positions[endIndex - 1])
        {
            endIndex--;
        }

        size_t positionCount = endIndex - startIndex + 1;

        // can't draw a line with less then two positions
        if (positionCount < 2)
        {
            return false;
        }

        // initialize mesh data properties are known
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        size_t initialVertexCount = meshVertexArray.size();

        // these indicies represent a quad section of the line
        // +----(A)---(C)----+-----+-----+-----+
        // |    /|    /|    /|    /|    /|    /|
        // |   / |   / |   / |   / |   / |   / |
        // |  / (1) / (2) /  |  /  |  /  |  /  |
        // | /   | /   | /   | /   | /   | /   |
        // |/    |/    |/    |/    |/    |/    |
        // +----(B)---(D)----+-----+-----+-----+
        unsigned int indexA = (unsigned int)initialVertexCount;
        unsigned int indexB;
        unsigned int indexC;
        unsigned int indexD;

        // generating line start cap
        // ---------------------------------------------------------------------
        glm::vec2 position1 = positions[startIndex];
        glm::vec2 position2 = positions[startIndex + 1];

        glm::vec2 direction1 = position2 - position1;
        glm::vec2 unitDirection1 = glm::normalize(direction1);
        glm::vec2 unitPerpendicular1(-1.0f * unitDirection1.y, unitDirection1.x);
        float halfWidth = width / 2.0f;
        glm::vec2 scaledPerpendicular1 = unitPerpendicular1 * halfWidth;
        float segmentRotation = glm::pi<float>() / endStyle;

        MeshVertex tempMeshVertex;
        tempMeshVertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);

        if (endStyle > 0)
        {
            // the endStyle value is used to construct a triangle fan end
            //  ex: endStyle == 1       ex: endStyle == 3
            //                                +---------+
            //                               / .         \
            //                              /      .      \
            //                             /           .   \
            //                            /                .\
            //  (B)-------(1)-------(A) (B)-------(1)-------(A)
            //   |\                  |   |\                  |
            //   | \                 |   | \                 |
            glm::vec2 rotatingOffset = scaledPerpendicular1;
            tempMeshVertex.position = glm::vec3(position1 + rotatingOffset, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);

            for (size_t i = 0; i < endStyle; ++i)
            {
                rotatingOffset = Rotate2DVector(rotatingOffset, segmentRotation);

                tempMeshVertex.position = glm::vec3(position1 + rotatingOffset, 0.0f);
                meshVertexArray.push_back(tempMeshVertex);
            }

            indexB = indexA + (unsigned int)endStyle;

            for (unsigned int i = indexA + 2; i <= indexB; ++i)
            {
                meshIndexArray.push_back(indexA);
                meshIndexArray.push_back(i - 1);
                meshIndexArray.push_back(i);
            }
        }
        else if (endStyle == 0)
        {
            // this results in an end extended behind position1
            // ex: endStyle == 0
            // (B)-----------------(A)
            //  |\                  |
            //  | \                 |
            //  |  \                |
            //  |   \               |
            //  |    \   (1)        |
            //  |     \             |
            glm::vec2 scaledDirection1 = unitDirection1 * halfWidth;
            tempMeshVertex.position = glm::vec3(position1 + scaledPerpendicular1 - scaledDirection1, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);
            tempMeshVertex.position = glm::vec3(position1 - scaledPerpendicular1 - scaledDirection1, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);
            indexB = indexA + 1;
        }

        // float slope1 = direction1.y / direction1.x; // ----------------------

        // generate line body segments
        // ---------------------------------------------------------------------
        for (size_t i = startIndex + 1; i <= endIndex; ++i)
        {
            // ignore duplicate positions in the middle
            if (positions[i] == position2)
            {
                continue;
            }

            glm::vec2 position3 = positions[i];
            glm::vec2 direction2 = position3 - position2;

            // ignore positions are going in the same direction
            if (glm::normalize(direction1) == glm::normalize(direction2))
            {
                position2 = position3;
                continue;
            }

            float angle = Get2DVectorAngle(direction1, direction2);
        }

        // generating line end segment and cap
        // ---------------------------------------------------------------------
        indexC = (unsigned int)meshVertexArray.size();;

        if (endStyle > 0)
        {
            // the endStyle value is used to construct a triangle fan end
            //  ex: endStyle == 1       ex: endStyle == 3
            //   |                 \ |   |                 \ |
            //   |                  \|   |                  \|
            //  (D)-------(2)-------(C) (D)-------(2)-------(C)
            //                            \                ./
            //                             \           .   /
            //                              \      .      /
            //                               \ .         /
            //                                +---------+
            glm::vec2 rotatingOffset = scaledPerpendicular1;
            tempMeshVertex.position = glm::vec3(position2 + rotatingOffset, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);

            for (size_t i = 0; i < endStyle; ++i)
            {
                rotatingOffset = Rotate2DVector(rotatingOffset, -segmentRotation);

                tempMeshVertex.position = glm::vec3(position2 + rotatingOffset, 0.0f);
                meshVertexArray.push_back(tempMeshVertex);
            }

            indexD = indexC + (unsigned int)endStyle;

            for (unsigned int i = indexC + 2; i <= indexD; ++i)
            {
                meshIndexArray.push_back(indexC);
                meshIndexArray.push_back(i);
                meshIndexArray.push_back(i - 1);
            }
        }
        else if (endStyle == 0)
        {
            // this results in an end extended behind position1
            // ex: endStyle == 0
            //  |             \     |
            //  |        (2)   \    |
            //  |               \   |
            //  |                \  |
            //  |                 \ |
            //  |                  \|
            // (D)-----------------(C)
            glm::vec2 scaledDirection1 = unitDirection1 * halfWidth;
            tempMeshVertex.position = glm::vec3(position2 + scaledPerpendicular1 + scaledDirection1, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);
            tempMeshVertex.position = glm::vec3(position2 - scaledPerpendicular1 + scaledDirection1, 0.0f);
            meshVertexArray.push_back(tempMeshVertex);
            indexD = indexC + 1;
        }

        // (B)----(A)
        //  |\     |
        //  | \    |
        //  |  \   |
        //  |   \  |
        //  |    \ |
        //  |     \|
        // (D)----(C)
        meshIndexArray.push_back(indexA);
        meshIndexArray.push_back(indexB);
        meshIndexArray.push_back(indexC);
        meshIndexArray.push_back(indexD);
        meshIndexArray.push_back(indexC);
        meshIndexArray.push_back(indexB);

        // calculate remaining mesh data properties
        // ---------------------------------------------------------------------
        for (size_t i = initialVertexCount; i < meshVertexArray.size(); ++i)
        {
            const glm::vec3& currentPosition = meshVertexArray[i].position;
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DLineLoop(
        MeshData& meshData,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        // ignore duplicate positions at the start
        size_t startIndex = 1;
        while (positions[0] == positions[startIndex])
        {
            startIndex++;
            if (startIndex == positions.size())
            {
                return false;
            }
        }

        // ignore duplicate positions at the end
        size_t endIndex = positions.size() - 1;
        while (positions[endIndex - 1] == positions[endIndex])
        {
            endIndex--;
        }

        if (startIndex == endIndex)
        {
            // must have at least 3 unique points to make a loop
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        size_t initialVertexCount = meshVertexArray.size();

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 position1 = positions[endIndex];
        glm::vec2 position2 = positions[0];

        glm::vec2 direction1 = position2 - position1;
        float slope1 = direction1.y / direction1.x;

        glm::vec2 offset01(-1.0f * direction1.y, direction1.x);
        float magnitude01 = glm::sqrt(offset01.x * offset01.x + offset01.y * offset01.y);
        glm::vec2 scaled1 = offset01 / magnitude01 * width / 2.0f;

        glm::vec2 scaled2 = scaled1;

        glm::vec2 positionA = position1 + scaled1;
        glm::vec2 positionB = position1 - scaled1;
        glm::vec2 positionC;
        glm::vec2 positionD;

        glm::vec2 position3 = positions[startIndex];

        glm::vec2 direction2 = position3 - position2;
        float slope2 = direction2.y / direction2.x;

        glm::vec2 offset02(-1.0f * direction2.y, direction2.x);
        float magnitude02 = glm::sqrt(offset02.x * offset02.x + offset02.y * offset02.y);
        scaled2 = offset02 / magnitude02 * width / 2.0f;

        glm::vec2 positionE = position2 + scaled2;
        glm::vec2 positionF = position2 - scaled2;

        MeshVertex meshVertexA;
        meshVertexA.normal = normal;
        MeshVertex meshVertexB;
        meshVertexB.normal = normal;
        MeshVertex meshVertexC;
        meshVertexC.normal = normal;
        MeshVertex meshVertexD;
        meshVertexD.normal = normal;

        bool sharpCorner = false;
        float angle = Get2DVectorAngle(direction1, direction2);
        if (beveledCorners)
        {
            if ((angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                && angle != glm::pi<float>())
            {
                sharpCorner = true;
            }
        }

        if (sharpCorner)
        {
            if (angle > 0.0f) // turn to the left
            {
                positionA = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
                positionB = positionF;
            }
            else
            {
                positionA = positionE;
                positionB = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
            }
        }
        else if (angle == 0.0 || angle == glm::pi<float>())
        {
            positionA = positionE;
            positionB = positionF;
        }
        else
        {
            positionA = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
            positionB = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
        }

        position1 = position2;
        position2 = position3;
        direction1 = direction2;
        slope1 = slope2;

        size_t i = startIndex + 1;
        do
        {
            // ignore duplicate positions in the middle
            if (positions[i] == position2)
            {
                continue;
            }

            position3 = positions[i];
            direction2 = position3 - position2;
            slope2 = direction2.y / direction2.x;

            // if (slope1 != slope2)
            if (glm::normalize(direction1) != glm::normalize(direction2))
            {
                offset02 = glm::vec2(-1.0f * direction2.y, direction2.x);
                magnitude02 = glm::sqrt(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                positionE = position2 + scaled2;
                positionF = position2 - scaled2;

                sharpCorner = false;
                angle = Get2DVectorAngle(direction1, direction2);
                if (beveledCorners)
                {
                    if ((angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                        && angle != glm::pi<float>())
                    {
                        sharpCorner = true;
                    }
                }

                if (sharpCorner)
                {
                    offset01 = glm::vec2(-1.0f * direction1.y, direction1.x);
                    magnitude01 = glm::sqrt(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        positionC = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
                        positionD = position2 - scaled1;
                    }
                    else
                    {
                        positionC = position2 + scaled1;
                        positionD = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
                    }
                }
                else if (angle == glm::pi<float>())
                {
                    positionC = position2 - scaled2;
                    positionD = position2 + scaled2;
                }
                else
                {
                    positionC = GetLineLineIntersection2D(positionA, slope1, positionE, slope2);
                    positionD = GetLineLineIntersection2D(positionB, slope1, positionF, slope2);
                }

                meshVertexA.position = glm::vec3(positionA, 0.0f);
                meshVertexB.position = glm::vec3(positionB, 0.0f);
                meshVertexC.position = glm::vec3(positionC, 0.0f);
                meshVertexD.position = glm::vec3(positionD, 0.0f);

                glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);
                glm::vec3 normalDCB = glm::cross(meshVertexC.position - meshVertexD.position, meshVertexB.position - meshVertexD.position);

                float dotABC = glm::dot(normal, normalABC);
                float dotDCB = glm::dot(normal, normalDCB);

                if (!triangulate)
                {
                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);
                    meshVertexArray.push_back(meshVertexD);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount + 3);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 3);
                    }
                }
                else
                {
                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    if (dotABC > 0.0f)
                    {
                        meshVertexArray.push_back(meshVertexA);
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexC);
                    }
                    // When the width of the line is too large, triangles can 
                    // flip.
                    else
                    {
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexA);
                    }

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);

                    if (dotDCB > 0.0f)
                    {
                        meshVertexArray.push_back(meshVertexD);
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexB);
                    }
                    else
                    {
                        meshVertexArray.push_back(meshVertexB);
                        meshVertexArray.push_back(meshVertexC);
                        meshVertexArray.push_back(meshVertexD);
                    }

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                }

                if (sharpCorner)
                {
                    meshVertexA.position = glm::vec3(positionC, 0.0f);
                    meshVertexB.position = glm::vec3(positionD, 0.0f);

                    if (angle > 0.0f) // turn to the left
                    {
                        meshVertexC.position = glm::vec3(positionF, 0.0f);
                    }
                    else
                    {
                        meshVertexC.position = glm::vec3(positionE, 0.0f);
                    }

                    normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);

                    dotABC = glm::dot(normal, normalABC);

                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount);
                    }

                    if (angle > 0.0f) // turn to the left
                    {
                        positionA = positionC;
                        positionB = positionF;
                    }
                    else
                    {
                        positionA = positionE;
                        positionB = positionD;
                    }
                }
                else if (angle == glm::pi<float>())
                {
                    positionA = positionD;
                    positionB = positionC;
                }
                else
                {
                    positionA = positionC;
                    positionB = positionD;
                }

                position1 = position2;
                direction1 = direction2;
                slope1 = slope2;
            }

            position2 = position3;

            if (++i > endIndex)
            {
                i = 0;
            }
        } while (i != startIndex + 1);

        for (size_t i = initialVertexCount; i < meshVertexArray.size(); ++i)
        {
            const glm::vec3& currentPosition = meshVertexArray[i].position;
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }

        return true;
    }

    bool MeshLoader::Generate2DPolygon(
        MeshData& meshData,
        const std::vector<glm::vec2>& corners,
        bool triangulate)
    {
        std::vector<size_t> indices;
        bool validPolygon = EarClipPolygon(indices, corners);
        if (!validPolygon)
        {
            return false;
        }

        PopulateMeshDataWithIndicesAndVerticesFor2DPolygon(meshData, corners, indices, triangulate);

        return true;
    }

    bool MeshLoader::Generate2DPolygon_v2(
        MeshData& meshData,
        const std::vector<glm::vec2>& corners,
        bool triangulate)
    {
        std::vector<size_t> indices;
        bool validPolygon = EarClipPolygon_v2(indices, corners);
        if (!validPolygon)
        {
            return false;
        }

        PopulateMeshDataWithIndicesAndVerticesFor2DPolygon(meshData, corners, indices, triangulate);

        return true;
    }

    bool MeshLoader::Generate2DRegularPolygon(
        MeshData& meshData,
        float radius,
        size_t sides,
        bool triangulate)
    {
        if (sides < 3 || radius == 0.0f)
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        positions.push_back(radialVector);

        for (size_t i = 0; i < sides - 1; ++i)
        {
            radialVector = Rotate2DVector(radialVector, sectionAngle);
            positions.push_back(radialVector);
        }

        return Generate2DTriangleFan(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DBezeledRectangle(
        MeshData& meshData,
        float width,
        float height,
        float bezelSize,
        size_t bezelSections,
        bool triangulate)
    {
        if (height <= 0.0f || width <= 0.0f || bezelSize <= 0.0f || bezelSize >= width * 0.5f || bezelSections < 1)
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        float radialStep = 0.5f * glm::pi<float>() / (float)bezelSections;
        float halfHeight = height * 0.5f - bezelSize;
        float halfWidth = width * 0.5f - bezelSize;

        // top right
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = radialStep * (float)i;
            float x = bezelSize * glm::cos(stackAngle);
            float y = bezelSize * glm::sin(stackAngle);

            positions.emplace_back(x + halfWidth, y + halfHeight);
        }

        // top left
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = 0.5f * glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * glm::cos(stackAngle);
            float y = bezelSize * glm::sin(stackAngle);

            positions.emplace_back(x - halfWidth, y + halfHeight);
        }

        // bottom left
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * glm::cos(stackAngle);
            float y = bezelSize * glm::sin(stackAngle);

            positions.emplace_back(x - halfWidth, y - halfHeight);
        }

        // bottom right
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = 1.5f * glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * glm::cos(stackAngle);
            float y = bezelSize * glm::sin(stackAngle);

            positions.emplace_back(x + halfWidth, y - halfHeight);
        }

        return Generate2DTriangleFan(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DRectangleFrame(
        MeshData& meshData,
        const glm::vec2& bottomLeft,
        const glm::vec2& topRight,
        float frameThickness,
        bool triangulate)
    {
        float halfThickness = frameThickness * 0.5f;

        std::vector<glm::vec2> positions;
        positions.reserve(10);
        positions.emplace_back(bottomLeft.x - halfThickness, topRight.y + halfThickness);
        positions.emplace_back(bottomLeft.x + halfThickness, topRight.y - halfThickness);
        positions.emplace_back(topRight.x + halfThickness, topRight.y + halfThickness);
        positions.emplace_back(topRight.x - halfThickness, topRight.y - halfThickness);
        positions.emplace_back(topRight.x + halfThickness, bottomLeft.y - halfThickness);
        positions.emplace_back(topRight.x - halfThickness, bottomLeft.y + halfThickness);
        positions.emplace_back(bottomLeft.x - halfThickness, bottomLeft.y - halfThickness);
        positions.emplace_back(bottomLeft.x + halfThickness, bottomLeft.y + halfThickness);
        positions.emplace_back(positions[0]);
        positions.emplace_back(positions[1]);

        return Generate2DTriangleStrip(meshData, positions, triangulate);
    }

    bool MeshLoader::Generate2DSprite(
        MeshData& meshData,
        float width,
        float height,
        float textureLeft,
        float textureRight,
        float textureBottom,
        float textureTop,
        bool triangulate)
    {
        if (width <= 0.0f || height <= 0.0f)
        {
            return false;
        }

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        float vertexRadius = glm::length(glm::vec2(halfWidth, halfHeight));
        if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

        if (maxVertexPosition.x < halfWidth) maxVertexPosition.x = halfWidth;
        if (maxVertexPosition.y < halfHeight) maxVertexPosition.y = halfHeight;
        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;

        if (minVertexPosition.x > -halfWidth) minVertexPosition.x = -halfWidth;
        if (minVertexPosition.y > -halfHeight) minVertexPosition.y = -halfHeight;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        MeshVertex topLeftVertex;
        topLeftVertex.position.x = -halfWidth;
        topLeftVertex.position.y = halfHeight;
        topLeftVertex.textureCoordinate.x = textureLeft;
        topLeftVertex.textureCoordinate.y = textureTop;
        topLeftVertex.normal.z = 1.0f;

        MeshVertex topRightVertex;
        topRightVertex.position.x = halfWidth;
        topRightVertex.position.y = halfHeight;
        topRightVertex.textureCoordinate.x = textureRight;
        topRightVertex.textureCoordinate.y = textureTop;
        topRightVertex.normal.z = 1.0f;

        MeshVertex bottomLeftVertex;
        bottomLeftVertex.position.x = -halfWidth;
        bottomLeftVertex.position.y = -halfHeight;
        bottomLeftVertex.textureCoordinate.x = textureLeft;
        bottomLeftVertex.textureCoordinate.y = textureBottom;
        bottomLeftVertex.normal.z = 1.0f;

        MeshVertex bottomRightVertex;
        bottomRightVertex.position.x = halfWidth;
        bottomRightVertex.position.y = -halfHeight;
        bottomRightVertex.textureCoordinate.x = textureRight;
        bottomRightVertex.textureCoordinate.y = textureBottom;
        bottomRightVertex.normal.z = 1.0f;

        unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
        if (triangulate)
        {
            meshVertexArray.push_back(topLeftVertex);
            meshVertexArray.push_back(bottomLeftVertex);
            meshVertexArray.push_back(topRightVertex);
            meshVertexArray.push_back(bottomRightVertex);
            meshVertexArray.push_back(topRightVertex);
            meshVertexArray.push_back(bottomLeftVertex);

            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount++);
            meshIndexArray.push_back(currentVertexCount);
        }
        else
        {
            meshVertexArray.push_back(topLeftVertex);
            meshVertexArray.push_back(bottomLeftVertex);
            meshVertexArray.push_back(bottomRightVertex);
            meshVertexArray.push_back(topRightVertex);

            meshIndexArray.push_back(currentVertexCount);
            meshIndexArray.push_back(currentVertexCount + 1);
            meshIndexArray.push_back(currentVertexCount + 3);
            meshIndexArray.push_back(currentVertexCount + 2);
            meshIndexArray.push_back(currentVertexCount + 3);
            meshIndexArray.push_back(currentVertexCount + 1);
        }

        return true;
    }

    bool MeshLoader::GenerateBox(
        MeshData& meshData,
        const glm::vec3& min,
        const glm::vec3& max,
        bool smoothNormals,
        bool triangulate)
    {
        // if (min.x >= max.x || min.y >= max.y || min.z >= max.z)
        // {
        //     return false;
        // }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        float vertexRadius = glm::length(max);
        if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;
        vertexRadius = glm::length(min);
        if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

        if (maxVertexPosition.x < max.x) maxVertexPosition.x = max.x;
        if (maxVertexPosition.y < max.y) maxVertexPosition.y = max.y;
        if (maxVertexPosition.z < max.z) maxVertexPosition.z = max.z;

        if (minVertexPosition.x < min.x) minVertexPosition.x = min.x;
        if (minVertexPosition.y < min.y) minVertexPosition.y = min.y;
        if (minVertexPosition.z < min.z) minVertexPosition.z = min.z;

        glm::vec3 octant_posX_posY_posZ(max);
        glm::vec3 octant_posX_posY_negZ(max.x, max.y, min.z);
        glm::vec3 octant_posX_negY_posZ(max.x, min.y, max.z);
        glm::vec3 octant_posX_negY_negZ(max.x, min.y, min.z);
        glm::vec3 octant_negX_posY_posZ(min.x, max.y, max.z);
        glm::vec3 octant_negX_posY_negZ(min.x, max.y, min.z);
        glm::vec3 octant_negX_negY_posZ(min.x, min.y, max.z);
        glm::vec3 octant_negX_negY_negZ(min);

        unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
        if (smoothNormals)
        {
            const float& dimensionLength = 1.0f / glm::sqrt(3.0f);

            glm::vec3 normal_posX_posY_posZ(dimensionLength, dimensionLength, dimensionLength);
            glm::vec3 normal_posX_posY_negZ(dimensionLength, dimensionLength, -dimensionLength);
            glm::vec3 normal_posX_negY_posZ(dimensionLength, -dimensionLength, dimensionLength);
            glm::vec3 normal_posX_negY_negZ(dimensionLength, -dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_posY_posZ(-dimensionLength, dimensionLength, dimensionLength);
            glm::vec3 normal_negX_posY_negZ(-dimensionLength, dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_negY_posZ(-dimensionLength, -dimensionLength, dimensionLength);
            glm::vec3 normal_negX_negY_negZ(-dimensionLength, -dimensionLength, -dimensionLength);

            MeshVertex meshVertex_posX_posY_posZ;
            meshVertex_posX_posY_posZ.position = octant_posX_posY_posZ;
            meshVertex_posX_posY_posZ.normal = normal_posX_posY_posZ;
            MeshVertex meshVertex_posX_posY_negZ;
            meshVertex_posX_posY_negZ.position = octant_posX_posY_negZ;
            meshVertex_posX_posY_negZ.normal = normal_posX_posY_negZ;
            MeshVertex meshVertex_posX_negY_posZ;
            meshVertex_posX_negY_posZ.position = octant_posX_negY_posZ;
            meshVertex_posX_negY_posZ.normal = normal_posX_negY_posZ;
            MeshVertex meshVertex_posX_negY_negZ;
            meshVertex_posX_negY_negZ.position = octant_posX_negY_negZ;
            meshVertex_posX_negY_negZ.normal = normal_posX_negY_negZ;
            MeshVertex meshVertex_negX_posY_posZ;
            meshVertex_negX_posY_posZ.position = octant_negX_posY_posZ;
            meshVertex_negX_posY_posZ.normal = normal_negX_posY_posZ;
            MeshVertex meshVertex_negX_posY_negZ;
            meshVertex_negX_posY_negZ.position = octant_negX_posY_negZ;
            meshVertex_negX_posY_negZ.normal = normal_negX_posY_negZ;
            MeshVertex meshVertex_negX_negY_posZ;
            meshVertex_negX_negY_posZ.position = octant_negX_negY_posZ;
            meshVertex_negX_negY_posZ.normal = normal_negX_negY_posZ;
            MeshVertex meshVertex_negX_negY_negZ;
            meshVertex_negX_negY_negZ.position = octant_negX_negY_negZ;
            meshVertex_negX_negY_negZ.normal = normal_negX_negY_negZ;

            if (triangulate)
            {
                // front
                meshVertexArray.push_back(meshVertex_negX_posY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_posZ);
                meshVertexArray.push_back(meshVertex_posX_posY_posZ);
                meshVertexArray.push_back(meshVertex_posX_negY_posZ);
                meshVertexArray.push_back(meshVertex_posX_posY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_posZ);

                // right
                meshVertexArray.push_back(meshVertex_posX_posY_posZ);
                meshVertexArray.push_back(meshVertex_posX_negY_posZ);
                meshVertexArray.push_back(meshVertex_posX_posY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_negZ);
                meshVertexArray.push_back(meshVertex_posX_posY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_posZ);

                // left
                meshVertexArray.push_back(meshVertex_negX_posY_negZ);
                meshVertexArray.push_back(meshVertex_negX_negY_negZ);
                meshVertexArray.push_back(meshVertex_negX_posY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_posZ);
                meshVertexArray.push_back(meshVertex_negX_posY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_negZ);

                // top
                meshVertexArray.push_back(meshVertex_negX_posY_negZ);
                meshVertexArray.push_back(meshVertex_negX_posY_posZ);
                meshVertexArray.push_back(meshVertex_posX_posY_negZ);
                meshVertexArray.push_back(meshVertex_posX_posY_posZ);
                meshVertexArray.push_back(meshVertex_posX_posY_negZ);
                meshVertexArray.push_back(meshVertex_negX_posY_posZ);

                // bottom
                meshVertexArray.push_back(meshVertex_negX_negY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_posZ);
                meshVertexArray.push_back(meshVertex_posX_negY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_posZ);
                meshVertexArray.push_back(meshVertex_negX_negY_negZ);

                // back
                meshVertexArray.push_back(meshVertex_posX_posY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_negZ);
                meshVertexArray.push_back(meshVertex_negX_posY_negZ);
                meshVertexArray.push_back(meshVertex_negX_negY_negZ);
                meshVertexArray.push_back(meshVertex_negX_posY_negZ);
                meshVertexArray.push_back(meshVertex_posX_negY_negZ);

                for (size_t i = 0; i < 36; ++i)
                {
                    meshIndexArray.push_back(currentVertexCount + (unsigned int)i);
                }
            }
            else
            {
                meshVertexArray.push_back(meshVertex_posX_posY_posZ); // 0
                meshVertexArray.push_back(meshVertex_posX_posY_negZ); // 1
                meshVertexArray.push_back(meshVertex_posX_negY_posZ); // 2
                meshVertexArray.push_back(meshVertex_posX_negY_negZ); // 3
                meshVertexArray.push_back(meshVertex_negX_posY_posZ); // 4
                meshVertexArray.push_back(meshVertex_negX_posY_negZ); // 5
                meshVertexArray.push_back(meshVertex_negX_negY_posZ); // 6
                meshVertexArray.push_back(meshVertex_negX_negY_negZ); // 7

                // front
                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 6);
                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 6);

                // right
                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);

                // left
                meshIndexArray.push_back(currentVertexCount + 5);
                meshIndexArray.push_back(currentVertexCount + 7);
                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 6);
                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 7);

                // top
                meshIndexArray.push_back(currentVertexCount + 5);
                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 4);

                // bottom
                meshIndexArray.push_back(currentVertexCount + 6);
                meshIndexArray.push_back(currentVertexCount + 7);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 2);
                meshIndexArray.push_back(currentVertexCount + 7);

                // back
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 5);
                meshIndexArray.push_back(currentVertexCount + 7);
                meshIndexArray.push_back(currentVertexCount + 5);
                meshIndexArray.push_back(currentVertexCount + 3);

                // currentVertexCount += 8;
            }
        }
        else
        {
            glm::vec3 normal_posX(1.0f, 0.0f, 0.0f);
            glm::vec3 normal_negX(-1.0f, 0.0f, 0.0f);
            glm::vec3 normal_posY(0.0f, 1.0f, 0.0f);
            glm::vec3 normal_negY(0.0f, -1.0f, 0.0f);
            glm::vec3 normal_posZ(0.0f, 0.0f, 1.0f);
            glm::vec3 normal_negZ(0.0f, 0.0f, -1.0f);

            MeshVertex meshVertex_frontFace_topRight;
            meshVertex_frontFace_topRight.position = octant_posX_posY_posZ;
            meshVertex_frontFace_topRight.normal = normal_posZ;
            MeshVertex meshVertex_frontFace_topLeft;
            meshVertex_frontFace_topLeft.position = octant_negX_posY_posZ;
            meshVertex_frontFace_topLeft.normal = normal_posZ;
            MeshVertex meshVertex_frontFace_bottomRight;
            meshVertex_frontFace_bottomRight.position = octant_posX_negY_posZ;
            meshVertex_frontFace_bottomRight.normal = normal_posZ;
            MeshVertex meshVertex_frontFace_bottomLeft;
            meshVertex_frontFace_bottomLeft.position = octant_negX_negY_posZ;
            meshVertex_frontFace_bottomLeft.normal = normal_posZ;

            MeshVertex meshVertex_rightFace_topRight;
            meshVertex_rightFace_topRight.position = octant_posX_posY_negZ;
            meshVertex_rightFace_topRight.normal = normal_posX;
            MeshVertex meshVertex_rightFace_topLeft;
            meshVertex_rightFace_topLeft.position = octant_posX_posY_posZ;
            meshVertex_rightFace_topLeft.normal = normal_posX;
            MeshVertex meshVertex_rightFace_bottomRight;
            meshVertex_rightFace_bottomRight.position = octant_posX_negY_negZ;
            meshVertex_rightFace_bottomRight.normal = normal_posX;
            MeshVertex meshVertex_rightFace_bottomLeft;
            meshVertex_rightFace_bottomLeft.position = octant_posX_negY_posZ;
            meshVertex_rightFace_bottomLeft.normal = normal_posX;

            MeshVertex meshVertex_backFace_topRight;
            meshVertex_backFace_topRight.position = octant_negX_posY_negZ;
            meshVertex_backFace_topRight.normal = normal_negZ;
            MeshVertex meshVertex_backFace_topLeft;
            meshVertex_backFace_topLeft.position = octant_posX_posY_negZ;
            meshVertex_backFace_topLeft.normal = normal_negZ;
            MeshVertex meshVertex_backFace_bottomRight;
            meshVertex_backFace_bottomRight.position = octant_negX_negY_negZ;
            meshVertex_backFace_bottomRight.normal = normal_negZ;
            MeshVertex meshVertex_backFace_bottomLeft;
            meshVertex_backFace_bottomLeft.position = octant_posX_negY_negZ;
            meshVertex_backFace_bottomLeft.normal = normal_negZ;

            MeshVertex meshVertex_leftFace_topRight;
            meshVertex_leftFace_topRight.position = octant_negX_posY_posZ;
            meshVertex_leftFace_topRight.normal = normal_negX;
            MeshVertex meshVertex_leftFace_topLeft;
            meshVertex_leftFace_topLeft.position = octant_negX_posY_negZ;
            meshVertex_leftFace_topLeft.normal = normal_negX;
            MeshVertex meshVertex_leftFace_bottomRight;
            meshVertex_leftFace_bottomRight.position = octant_negX_negY_posZ;
            meshVertex_leftFace_bottomRight.normal = normal_negX;
            MeshVertex meshVertex_leftFace_bottomLeft;
            meshVertex_leftFace_bottomLeft.position = octant_negX_negY_negZ;
            meshVertex_leftFace_bottomLeft.normal = normal_negX;

            MeshVertex meshVertex_topFace_topRight;
            meshVertex_topFace_topRight.position = octant_posX_posY_negZ;
            meshVertex_topFace_topRight.normal = normal_posY;
            MeshVertex meshVertex_topFace_topLeft;
            meshVertex_topFace_topLeft.position = octant_negX_posY_negZ;
            meshVertex_topFace_topLeft.normal = normal_posY;
            MeshVertex meshVertex_topFace_bottomRight;
            meshVertex_topFace_bottomRight.position = octant_posX_posY_posZ;
            meshVertex_topFace_bottomRight.normal = normal_posY;
            MeshVertex meshVertex_topFace_bottomLeft;
            meshVertex_topFace_bottomLeft.position = octant_negX_posY_posZ;
            meshVertex_topFace_bottomLeft.normal = normal_posY;

            MeshVertex meshVertex_bottomFace_topRight;
            meshVertex_bottomFace_topRight.position = octant_posX_negY_posZ;
            meshVertex_bottomFace_topRight.normal = normal_negY;
            MeshVertex meshVertex_bottomFace_topLeft;
            meshVertex_bottomFace_topLeft.position = octant_negX_negY_posZ;
            meshVertex_bottomFace_topLeft.normal = normal_negY;
            MeshVertex meshVertex_bottomFace_bottomRight;
            meshVertex_bottomFace_bottomRight.position = octant_posX_negY_negZ;
            meshVertex_bottomFace_bottomRight.normal = normal_negY;
            MeshVertex meshVertex_bottomFace_bottomLeft;
            meshVertex_bottomFace_bottomLeft.position = octant_negX_negY_negZ;
            meshVertex_bottomFace_bottomLeft.normal = normal_negY;

            if (triangulate)
            {
                meshVertexArray.push_back(meshVertex_frontFace_topRight);
                meshVertexArray.push_back(meshVertex_frontFace_topLeft);
                meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_frontFace_topRight);
                meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_frontFace_bottomRight);

                meshVertexArray.push_back(meshVertex_rightFace_topRight);
                meshVertexArray.push_back(meshVertex_rightFace_topLeft);
                meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_rightFace_topRight);
                meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_rightFace_bottomRight);

                meshVertexArray.push_back(meshVertex_backFace_topRight);
                meshVertexArray.push_back(meshVertex_backFace_topLeft);
                meshVertexArray.push_back(meshVertex_backFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_backFace_topRight);
                meshVertexArray.push_back(meshVertex_backFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_backFace_bottomRight);

                meshVertexArray.push_back(meshVertex_leftFace_topRight);
                meshVertexArray.push_back(meshVertex_leftFace_topLeft);
                meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_leftFace_topRight);
                meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_leftFace_bottomRight);

                meshVertexArray.push_back(meshVertex_topFace_topRight);
                meshVertexArray.push_back(meshVertex_topFace_topLeft);
                meshVertexArray.push_back(meshVertex_topFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_topFace_topRight);
                meshVertexArray.push_back(meshVertex_topFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_topFace_bottomRight);

                meshVertexArray.push_back(meshVertex_bottomFace_topRight);
                meshVertexArray.push_back(meshVertex_bottomFace_topLeft);
                meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_bottomFace_topRight);
                meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);
                meshVertexArray.push_back(meshVertex_bottomFace_bottomRight);

                for (size_t i = 0; i < 36; ++i)
                {
                    meshIndexArray.push_back(currentVertexCount + (unsigned int)i);
                }
            }
            else
            {
                meshVertexArray.push_back(meshVertex_frontFace_topRight);     // 0
                meshVertexArray.push_back(meshVertex_frontFace_topLeft);      // 1
                meshVertexArray.push_back(meshVertex_frontFace_bottomRight);  // 2
                meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);   // 3

                meshVertexArray.push_back(meshVertex_rightFace_topRight);     // 4
                meshVertexArray.push_back(meshVertex_rightFace_topLeft);      // 5
                meshVertexArray.push_back(meshVertex_rightFace_bottomRight);  // 6
                meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);   // 7

                meshVertexArray.push_back(meshVertex_backFace_topRight);      // 8
                meshVertexArray.push_back(meshVertex_backFace_topLeft);       // 9
                meshVertexArray.push_back(meshVertex_backFace_bottomRight);   // 10
                meshVertexArray.push_back(meshVertex_backFace_bottomLeft);    // 11

                meshVertexArray.push_back(meshVertex_leftFace_topRight);      // 12
                meshVertexArray.push_back(meshVertex_leftFace_topLeft);       // 13
                meshVertexArray.push_back(meshVertex_leftFace_bottomRight);   // 14
                meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);    // 15

                meshVertexArray.push_back(meshVertex_topFace_topRight);       // 16
                meshVertexArray.push_back(meshVertex_topFace_topLeft);        // 17
                meshVertexArray.push_back(meshVertex_topFace_bottomRight);    // 18
                meshVertexArray.push_back(meshVertex_topFace_bottomLeft);     // 19

                meshVertexArray.push_back(meshVertex_bottomFace_topRight);    // 20
                meshVertexArray.push_back(meshVertex_bottomFace_topLeft);     // 21
                meshVertexArray.push_back(meshVertex_bottomFace_bottomRight); // 22
                meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);  // 23

                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 0);
                meshIndexArray.push_back(currentVertexCount + 3);
                meshIndexArray.push_back(currentVertexCount + 2);

                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 5);
                meshIndexArray.push_back(currentVertexCount + 7);
                meshIndexArray.push_back(currentVertexCount + 4);
                meshIndexArray.push_back(currentVertexCount + 7);
                meshIndexArray.push_back(currentVertexCount + 6);

                meshIndexArray.push_back(currentVertexCount + 8);
                meshIndexArray.push_back(currentVertexCount + 9);
                meshIndexArray.push_back(currentVertexCount + 11);
                meshIndexArray.push_back(currentVertexCount + 8);
                meshIndexArray.push_back(currentVertexCount + 11);
                meshIndexArray.push_back(currentVertexCount + 10);

                meshIndexArray.push_back(currentVertexCount + 12);
                meshIndexArray.push_back(currentVertexCount + 13);
                meshIndexArray.push_back(currentVertexCount + 15);
                meshIndexArray.push_back(currentVertexCount + 12);
                meshIndexArray.push_back(currentVertexCount + 15);
                meshIndexArray.push_back(currentVertexCount + 14);

                meshIndexArray.push_back(currentVertexCount + 16);
                meshIndexArray.push_back(currentVertexCount + 17);
                meshIndexArray.push_back(currentVertexCount + 19);
                meshIndexArray.push_back(currentVertexCount + 16);
                meshIndexArray.push_back(currentVertexCount + 19);
                meshIndexArray.push_back(currentVertexCount + 18);

                meshIndexArray.push_back(currentVertexCount + 20);
                meshIndexArray.push_back(currentVertexCount + 21);
                meshIndexArray.push_back(currentVertexCount + 23);
                meshIndexArray.push_back(currentVertexCount + 20);
                meshIndexArray.push_back(currentVertexCount + 23);
                meshIndexArray.push_back(currentVertexCount + 22);
            }

            // currentVertexCount += 24;
        }

        return true;
    }

    bool MeshLoader::GenerateBox(
        MeshData& meshData,
        float xLength,
        float yLength,
        float zLength,
        bool smoothNormals,
        bool triangulate)
    {
        // if (xLength <= 0.0f || yLength <= 0.0f || zLength <= 0.0f)
        // {
        //     return false;
        // }

        glm::vec3 min(-0.5f * xLength, -0.5f * yLength, -0.5f * zLength);
        glm::vec3 max(0.5f * xLength, 0.5f * yLength, 0.5f * zLength);

        return GenerateBox(
            meshData,
            min,
            max,
            smoothNormals,
            triangulate
        );
    }

    bool MeshLoader::GenerateBoxWithTexture(
        MeshData& meshData,
        const glm::vec3& min,
        const glm::vec3& max,
        const std::vector<glm::vec2>& textureCoordinates,
        bool smoothNormals,
        bool triangulate)
    {
        // if (min.x >= max.x || min.y >= max.y || min.z >= max.z)
        // {
        //     return false;
        // }

        if (textureCoordinates.size() < 24)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        float vertexRadius = glm::length(max);
        if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;
        vertexRadius = glm::length(min);
        if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

        if (maxVertexPosition.x < max.x) maxVertexPosition.x = max.x;
        if (maxVertexPosition.y < max.y) maxVertexPosition.y = max.y;
        if (maxVertexPosition.z < max.z) maxVertexPosition.z = max.z;

        if (minVertexPosition.x < min.x) minVertexPosition.x = min.x;
        if (minVertexPosition.y < min.y) minVertexPosition.y = min.y;
        if (minVertexPosition.z < min.z) minVertexPosition.z = min.z;

        glm::vec3 octant_maxX_maxY_maxZ(max);
        glm::vec3 octant_maxX_maxY_minZ(max.x, max.y, min.z);
        glm::vec3 octant_maxX_minY_maxZ(max.x, min.y, max.z);
        glm::vec3 octant_maxX_minY_minZ(max.x, min.y, min.z);
        glm::vec3 octant_minX_maxY_maxZ(min.x, max.y, max.z);
        glm::vec3 octant_minX_maxY_minZ(min.x, max.y, min.z);
        glm::vec3 octant_minX_minY_maxZ(min.x, min.y, max.z);
        glm::vec3 octant_minX_minY_minZ(min);

        // The faces are labeled from the viewers perspective.

        MeshVertex meshVertex_frontFace_topLeft;
        meshVertex_frontFace_topLeft.position = octant_minX_maxY_maxZ;
        meshVertex_frontFace_topLeft.textureCoordinate = textureCoordinates[0];
        MeshVertex meshVertex_frontFace_bottomLeft;
        meshVertex_frontFace_bottomLeft.position = octant_minX_minY_maxZ;
        meshVertex_frontFace_bottomLeft.textureCoordinate = textureCoordinates[1];
        MeshVertex meshVertex_frontFace_topRight;
        meshVertex_frontFace_topRight.position = octant_maxX_maxY_maxZ;
        meshVertex_frontFace_topRight.textureCoordinate = textureCoordinates[2];
        MeshVertex meshVertex_frontFace_bottomRight;
        meshVertex_frontFace_bottomRight.position = octant_maxX_minY_maxZ;
        meshVertex_frontFace_bottomRight.textureCoordinate = textureCoordinates[3];

        MeshVertex meshVertex_rightFace_topLeft;
        meshVertex_rightFace_topLeft.position = octant_maxX_maxY_maxZ;;
        meshVertex_rightFace_topLeft.textureCoordinate = textureCoordinates[4];
        MeshVertex meshVertex_rightFace_bottomLeft;
        meshVertex_rightFace_bottomLeft.position = octant_maxX_minY_maxZ;
        meshVertex_rightFace_bottomLeft.textureCoordinate = textureCoordinates[5];
        MeshVertex meshVertex_rightFace_topRight;
        meshVertex_rightFace_topRight.position = octant_maxX_maxY_minZ;
        meshVertex_rightFace_topRight.textureCoordinate = textureCoordinates[6];
        MeshVertex meshVertex_rightFace_bottomRight;
        meshVertex_rightFace_bottomRight.position = octant_maxX_minY_minZ;
        meshVertex_rightFace_bottomRight.textureCoordinate = textureCoordinates[7];

        MeshVertex meshVertex_backFace_topLeft;
        meshVertex_backFace_topLeft.position = octant_maxX_maxY_minZ;;
        meshVertex_backFace_topLeft.textureCoordinate = textureCoordinates[8];
        MeshVertex meshVertex_backFace_bottomLeft;
        meshVertex_backFace_bottomLeft.position = octant_maxX_minY_minZ;
        meshVertex_backFace_bottomLeft.textureCoordinate = textureCoordinates[9];
        MeshVertex meshVertex_backFace_topRight;
        meshVertex_backFace_topRight.position = octant_minX_maxY_minZ;
        meshVertex_backFace_topRight.textureCoordinate = textureCoordinates[10];
        MeshVertex meshVertex_backFace_bottomRight;
        meshVertex_backFace_bottomRight.position = octant_minX_minY_minZ;
        meshVertex_backFace_bottomRight.textureCoordinate = textureCoordinates[11];

        MeshVertex meshVertex_leftFace_topLeft;
        meshVertex_leftFace_topLeft.position = octant_minX_maxY_minZ;;
        meshVertex_leftFace_topLeft.textureCoordinate = textureCoordinates[12];
        MeshVertex meshVertex_leftFace_bottomLeft;
        meshVertex_leftFace_bottomLeft.position = octant_minX_minY_minZ;
        meshVertex_leftFace_bottomLeft.textureCoordinate = textureCoordinates[13];
        MeshVertex meshVertex_leftFace_topRight;
        meshVertex_leftFace_topRight.position = octant_minX_maxY_maxZ;
        meshVertex_leftFace_topRight.textureCoordinate = textureCoordinates[14];
        MeshVertex meshVertex_leftFace_bottomRight;
        meshVertex_leftFace_bottomRight.position = octant_minX_minY_maxZ;
        meshVertex_leftFace_bottomRight.textureCoordinate = textureCoordinates[15];

        MeshVertex meshVertex_topFace_topLeft;
        meshVertex_topFace_topLeft.position = octant_minX_maxY_minZ;;
        meshVertex_topFace_topLeft.textureCoordinate = textureCoordinates[16];
        MeshVertex meshVertex_topFace_bottomLeft;
        meshVertex_topFace_bottomLeft.position = octant_minX_maxY_maxZ;
        meshVertex_topFace_bottomLeft.textureCoordinate = textureCoordinates[17];
        MeshVertex meshVertex_topFace_topRight;
        meshVertex_topFace_topRight.position = octant_maxX_maxY_minZ;
        meshVertex_topFace_topRight.textureCoordinate = textureCoordinates[18];
        MeshVertex meshVertex_topFace_bottomRight;
        meshVertex_topFace_bottomRight.position = octant_maxX_maxY_maxZ;
        meshVertex_topFace_bottomRight.textureCoordinate = textureCoordinates[19];

        MeshVertex meshVertex_bottomFace_topLeft;
        meshVertex_bottomFace_topLeft.position = octant_maxX_minY_minZ;
        meshVertex_bottomFace_topLeft.textureCoordinate = textureCoordinates[20];
        MeshVertex meshVertex_bottomFace_bottomLeft;
        meshVertex_bottomFace_bottomLeft.position = octant_maxX_minY_maxZ;
        meshVertex_bottomFace_bottomLeft.textureCoordinate = textureCoordinates[21];
        MeshVertex meshVertex_bottomFace_topRight;
        meshVertex_bottomFace_topRight.position = octant_minX_minY_minZ;
        meshVertex_bottomFace_topRight.textureCoordinate = textureCoordinates[22];
        MeshVertex meshVertex_bottomFace_bottomRight;
        meshVertex_bottomFace_bottomRight.position = octant_minX_minY_maxZ;
        meshVertex_bottomFace_bottomRight.textureCoordinate = textureCoordinates[23];

        unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
        if (smoothNormals)
        {
            const float& dimensionLength = 1.0f / glm::sqrt(3.0f);

            glm::vec3 normal_posX_posY_posZ(dimensionLength, dimensionLength, dimensionLength);
            glm::vec3 normal_posX_posY_negZ(dimensionLength, dimensionLength, -dimensionLength);
            glm::vec3 normal_posX_negY_posZ(dimensionLength, -dimensionLength, dimensionLength);
            glm::vec3 normal_posX_negY_negZ(dimensionLength, -dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_posY_posZ(-dimensionLength, dimensionLength, dimensionLength);
            glm::vec3 normal_negX_posY_negZ(-dimensionLength, dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_negY_posZ(-dimensionLength, -dimensionLength, dimensionLength);
            glm::vec3 normal_negX_negY_negZ(-dimensionLength, -dimensionLength, -dimensionLength);

            meshVertex_frontFace_topLeft.normal = normal_negX_posY_posZ;
            meshVertex_frontFace_bottomLeft.normal = normal_negX_negY_posZ;
            meshVertex_frontFace_bottomRight.normal = normal_posX_negY_posZ;
            meshVertex_frontFace_topRight.normal = normal_posX_posY_posZ;

            meshVertex_rightFace_topLeft.normal = normal_posX_posY_posZ;
            meshVertex_rightFace_bottomLeft.normal = normal_posX_negY_posZ;
            meshVertex_rightFace_bottomRight.normal = normal_posX_negY_negZ;
            meshVertex_rightFace_topRight.normal = normal_posX_posY_negZ;

            meshVertex_backFace_topLeft.normal = normal_posX_posY_negZ;
            meshVertex_backFace_bottomLeft.normal = normal_posX_negY_negZ;
            meshVertex_backFace_bottomRight.normal = normal_negX_negY_negZ;
            meshVertex_backFace_topRight.normal = normal_negX_posY_negZ;

            meshVertex_leftFace_topLeft.normal = normal_negX_posY_negZ;
            meshVertex_leftFace_bottomLeft.normal = normal_negX_negY_negZ;
            meshVertex_leftFace_bottomRight.normal = normal_negX_negY_posZ;
            meshVertex_leftFace_topRight.normal = normal_negX_posY_posZ;

            meshVertex_topFace_topLeft.normal = normal_negX_posY_negZ;;
            meshVertex_topFace_bottomLeft.normal = normal_negX_posY_posZ;
            meshVertex_topFace_bottomRight.normal = normal_posX_posY_posZ;
            meshVertex_topFace_topRight.normal = normal_posX_posY_negZ;

            meshVertex_bottomFace_topLeft.normal = normal_posX_negY_negZ;
            meshVertex_bottomFace_bottomLeft.normal = normal_posX_negY_posZ;
            meshVertex_bottomFace_bottomRight.normal = normal_negX_negY_posZ;
            meshVertex_bottomFace_topRight.normal = normal_negX_negY_negZ;
        }
        else
        {
            glm::vec3 normal_posX(1.0f, 0.0f, 0.0f);
            glm::vec3 normal_negX(-1.0f, 0.0f, 0.0f);
            glm::vec3 normal_posY(0.0f, 1.0f, 0.0f);
            glm::vec3 normal_negY(0.0f, -1.0f, 0.0f);
            glm::vec3 normal_posZ(0.0f, 0.0f, 1.0f);
            glm::vec3 normal_negZ(0.0f, 0.0f, -1.0f);

            meshVertex_frontFace_topLeft.normal = normal_posZ;
            meshVertex_frontFace_bottomLeft.normal = normal_posZ;
            meshVertex_frontFace_bottomRight.normal = normal_posZ;
            meshVertex_frontFace_topRight.normal = normal_posZ;

            meshVertex_rightFace_topLeft.normal = normal_posX;
            meshVertex_rightFace_bottomLeft.normal = normal_posX;
            meshVertex_rightFace_bottomRight.normal = normal_posX;
            meshVertex_rightFace_topRight.normal = normal_posX;

            meshVertex_backFace_topLeft.normal = normal_negZ;
            meshVertex_backFace_bottomLeft.normal = normal_negZ;
            meshVertex_backFace_bottomRight.normal = normal_negZ;
            meshVertex_backFace_topRight.normal = normal_negZ;

            meshVertex_leftFace_topLeft.normal = normal_negX;
            meshVertex_leftFace_bottomLeft.normal = normal_negX;
            meshVertex_leftFace_bottomRight.normal = normal_negX;
            meshVertex_leftFace_topRight.normal = normal_negX;

            meshVertex_topFace_topLeft.normal = normal_posY;;
            meshVertex_topFace_bottomLeft.normal = normal_posY;
            meshVertex_topFace_bottomRight.normal = normal_posY;
            meshVertex_topFace_topRight.normal = normal_posY;

            meshVertex_bottomFace_topLeft.normal = normal_negY;
            meshVertex_bottomFace_bottomLeft.normal = normal_negY;
            meshVertex_bottomFace_bottomRight.normal = normal_negY;
            meshVertex_bottomFace_topRight.normal = normal_negY;
        }

        if (triangulate)
        {
            meshVertexArray.push_back(meshVertex_frontFace_topLeft);
            meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_frontFace_topRight);
            meshVertexArray.push_back(meshVertex_frontFace_bottomRight);
            meshVertexArray.push_back(meshVertex_frontFace_topRight);
            meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);

            meshVertexArray.push_back(meshVertex_rightFace_topLeft);
            meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_rightFace_topRight);
            meshVertexArray.push_back(meshVertex_rightFace_bottomRight);
            meshVertexArray.push_back(meshVertex_rightFace_topRight);
            meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);

            meshVertexArray.push_back(meshVertex_backFace_topLeft);
            meshVertexArray.push_back(meshVertex_backFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_backFace_topRight);
            meshVertexArray.push_back(meshVertex_backFace_bottomRight);
            meshVertexArray.push_back(meshVertex_backFace_topRight);
            meshVertexArray.push_back(meshVertex_backFace_bottomLeft);

            meshVertexArray.push_back(meshVertex_leftFace_topLeft);
            meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_leftFace_topRight);
            meshVertexArray.push_back(meshVertex_leftFace_bottomRight);
            meshVertexArray.push_back(meshVertex_leftFace_topRight);
            meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);

            meshVertexArray.push_back(meshVertex_topFace_topLeft);
            meshVertexArray.push_back(meshVertex_topFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_topFace_topRight);
            meshVertexArray.push_back(meshVertex_topFace_bottomRight);
            meshVertexArray.push_back(meshVertex_topFace_topRight);
            meshVertexArray.push_back(meshVertex_topFace_bottomLeft);

            meshVertexArray.push_back(meshVertex_bottomFace_topLeft);
            meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);
            meshVertexArray.push_back(meshVertex_bottomFace_topRight);
            meshVertexArray.push_back(meshVertex_bottomFace_bottomRight);
            meshVertexArray.push_back(meshVertex_bottomFace_topRight);
            meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);

            for (size_t i = 0; i < 36; ++i)
            {
                meshIndexArray.push_back(currentVertexCount + (unsigned int)i);
            }
        }
        else
        {
            meshVertexArray.push_back(meshVertex_frontFace_topLeft);      // 0
            meshVertexArray.push_back(meshVertex_frontFace_bottomLeft);   // 1
            meshVertexArray.push_back(meshVertex_frontFace_topRight);     // 2
            meshVertexArray.push_back(meshVertex_frontFace_bottomRight);  // 3
            meshVertexArray.push_back(meshVertex_rightFace_topLeft);      // 4
            meshVertexArray.push_back(meshVertex_rightFace_bottomLeft);   // 5
            meshVertexArray.push_back(meshVertex_rightFace_topRight);     // 6
            meshVertexArray.push_back(meshVertex_rightFace_bottomRight);  // 7
            meshVertexArray.push_back(meshVertex_backFace_topLeft);       // 8
            meshVertexArray.push_back(meshVertex_backFace_bottomLeft);    // 9
            meshVertexArray.push_back(meshVertex_backFace_topRight);      // 10
            meshVertexArray.push_back(meshVertex_backFace_bottomRight);   // 11
            meshVertexArray.push_back(meshVertex_leftFace_topLeft);       // 12
            meshVertexArray.push_back(meshVertex_leftFace_bottomLeft);    // 13
            meshVertexArray.push_back(meshVertex_leftFace_topRight);      // 14
            meshVertexArray.push_back(meshVertex_leftFace_bottomRight);   // 15
            meshVertexArray.push_back(meshVertex_topFace_topLeft);        // 16
            meshVertexArray.push_back(meshVertex_topFace_bottomLeft);     // 17
            meshVertexArray.push_back(meshVertex_topFace_topRight);       // 18
            meshVertexArray.push_back(meshVertex_topFace_bottomRight);    // 19
            meshVertexArray.push_back(meshVertex_bottomFace_topLeft);     // 20
            meshVertexArray.push_back(meshVertex_bottomFace_bottomLeft);  // 21
            meshVertexArray.push_back(meshVertex_bottomFace_topRight);    // 22
            meshVertexArray.push_back(meshVertex_bottomFace_bottomRight); // 23

            // front
            meshIndexArray.push_back(currentVertexCount + 0);
            meshIndexArray.push_back(currentVertexCount + 1);
            meshIndexArray.push_back(currentVertexCount + 2);
            meshIndexArray.push_back(currentVertexCount + 3);
            meshIndexArray.push_back(currentVertexCount + 2);
            meshIndexArray.push_back(currentVertexCount + 1);

            // right
            meshIndexArray.push_back(currentVertexCount + 4);
            meshIndexArray.push_back(currentVertexCount + 5);
            meshIndexArray.push_back(currentVertexCount + 6);
            meshIndexArray.push_back(currentVertexCount + 7);
            meshIndexArray.push_back(currentVertexCount + 6);
            meshIndexArray.push_back(currentVertexCount + 5);

            // left
            meshIndexArray.push_back(currentVertexCount + 8);
            meshIndexArray.push_back(currentVertexCount + 9);
            meshIndexArray.push_back(currentVertexCount + 10);
            meshIndexArray.push_back(currentVertexCount + 11);
            meshIndexArray.push_back(currentVertexCount + 10);
            meshIndexArray.push_back(currentVertexCount + 9);

            // top
            meshIndexArray.push_back(currentVertexCount + 12);
            meshIndexArray.push_back(currentVertexCount + 13);
            meshIndexArray.push_back(currentVertexCount + 14);
            meshIndexArray.push_back(currentVertexCount + 15);
            meshIndexArray.push_back(currentVertexCount + 14);
            meshIndexArray.push_back(currentVertexCount + 13);

            // bottom
            meshIndexArray.push_back(currentVertexCount + 16);
            meshIndexArray.push_back(currentVertexCount + 17);
            meshIndexArray.push_back(currentVertexCount + 18);
            meshIndexArray.push_back(currentVertexCount + 19);
            meshIndexArray.push_back(currentVertexCount + 18);
            meshIndexArray.push_back(currentVertexCount + 17);

            // back
            meshIndexArray.push_back(currentVertexCount + 20);
            meshIndexArray.push_back(currentVertexCount + 21);
            meshIndexArray.push_back(currentVertexCount + 22);
            meshIndexArray.push_back(currentVertexCount + 23);
            meshIndexArray.push_back(currentVertexCount + 22);
            meshIndexArray.push_back(currentVertexCount + 21);

            // currentVertexCount += 8;
        }

        return true;
    }

    bool MeshLoader::GenerateCapsule(
        MeshData& meshData,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals,
        bool triangulate)
    {
        if (cylindricalHeight <= 0.0f || radius <= 0.0f || faces < 3 || stacks < 1)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        float capsuleHalfHeight = cylindricalHeight / 2.0f + radius;

        if (maxVertexPosition.y < capsuleHalfHeight) maxVertexPosition.y = capsuleHalfHeight;
        if (minVertexPosition.y > -capsuleHalfHeight) minVertexPosition.y = -capsuleHalfHeight;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = (stacks + 1) * 2 * (faces + 1) - 2;
        positions.reserve(uniquePositions);
        textureCoordinates.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);
        const float stackStep = 0.5f * glm::pi<float>() / (float)(stacks);

        float halfCylindricalHeight = cylindricalHeight * 0.5f;
        float quarterCircleRadius = radius * glm::half_pi<float>();
        float capsuleHalfBoarder = quarterCircleRadius * 2.0f + cylindricalHeight;

        // top
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, capsuleHalfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 1.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        // body top
        for (size_t i = 1; i <= stacks; ++i)
        {
            float latiAngle = glm::pi<float>() / 2.0f - stackStep * (float)i;
            float xz = radius * glm::cos(latiAngle);
            float y = radius * glm::sin(latiAngle) + halfCylindricalHeight;

            for (size_t j = 0; j < faces + 1; ++j) // +1 for wrap around
            {
                float longAngle = glm::pi<float>() / 2.0f - faceStep * (float)j;
                float x = xz * glm::cos(longAngle);
                float z = xz * glm::sin(longAngle);

                positions.emplace_back(x, y, z);
                textureCoordinates.emplace_back(
                    (float)j / (float)faces,
                    (capsuleHalfBoarder - (float)i / (float)stacks * quarterCircleRadius) / capsuleHalfBoarder
                );
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // body bottom
        for (size_t i = stacks; i < stacks * 2; ++i)
        {
            float latiAngle = glm::pi<float>() / 2.0f - stackStep * (float)i;
            float xz = radius * glm::cos(latiAngle);
            float y = radius * glm::sin(latiAngle) - halfCylindricalHeight;

            for (size_t j = 0; j < faces + 1; ++j) // +1 for wrap around
            {
                float longAngle = glm::pi<float>() / 2.0f - faceStep * (float)j;
                float x = xz * glm::cos(longAngle);
                float z = xz * glm::sin(longAngle);

                positions.emplace_back(x, y, z);
                textureCoordinates.emplace_back(
                    (float)j / (float)faces,
                    (quarterCircleRadius - ((float)i - stacks) / (float)stacks * quarterCircleRadius) / capsuleHalfBoarder
                );
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // bottom
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, -1.0f * capsuleHalfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 0.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, -1.0f, 0.0f);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        GenerateSphereMeshVerticesAndIndices(
            meshData,
            faces,
            stacks * 2 + 1,
            positions,
            textureCoordinates,
            normals,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateCapsule_v2(
        MeshData& meshData,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals,
        bool triangulate)
    {
        if (cylindricalHeight <= 0.0f || radius <= 0.0f || faces < 3 || stacks < 1)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        float capsuleHalfHeight = cylindricalHeight / 2.0f + radius;

        if (maxVertexPosition.y < capsuleHalfHeight) maxVertexPosition.y = capsuleHalfHeight;
        if (minVertexPosition.y > -capsuleHalfHeight) minVertexPosition.y = -capsuleHalfHeight;

        size_t initialVertexCount = meshVertexArray.size();

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = stacks * 2 * faces + 2;
        positions.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float faceStep = -2.0f * glm::pi<float>() / (float)(faces);
        const float stackStep = 0.5f * glm::pi<float>() / (float)(stacks);

        float halfCylindricalHeight = cylindricalHeight * 0.5f;

        // top
        positions.emplace_back(0.0f, radius + halfCylindricalHeight, 0.0f);
        if (smoothNormals)
        {
            normals.emplace_back(0.0f, 1.0f, 0.0f);
        }

        // body top
        for (size_t i = 1; i < stacks + 1; ++i)
        {
            float stackAngle = glm::pi<float>() / 2.0f - stackStep * (float)i;
            float xz = radius * glm::cos(stackAngle);
            float y = radius * glm::sin(stackAngle);

            for (size_t j = 0; j < faces; ++j)
            {
                float faceAngle = faceStep * (float)j;
                float x = xz * glm::cos(faceAngle);
                float z = xz * glm::sin(faceAngle);

                positions.emplace_back(x, y + halfCylindricalHeight, z);
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // body bottom
        for (size_t i = 0; i < stacks; ++i)
        {
            float stackAngle = -1.0f * stackStep * (float)i;
            float xz = radius * glm::cos(stackAngle);
            float y = radius * glm::sin(stackAngle);

            for (size_t j = 0; j < faces; ++j)
            {
                float faceAngle = faceStep * (float)j;
                float x = xz * glm::cos(faceAngle);
                float z = xz * glm::sin(faceAngle);

                positions.emplace_back(x, y - halfCylindricalHeight, z);
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // bottom
        positions.emplace_back(0.0f, -radius - halfCylindricalHeight, 0.0f);
        if (smoothNormals)
        {
            normals.emplace_back(0.0f, -1.0f, 0.0f);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        if (smoothNormals)
        {
            if (triangulate)
            {
                std::vector<MeshVertex> tempMeshVertexArray;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.normal = normals[i];
                    tempMeshVertexArray.push_back(meshVertex);
                }

                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[0]);
                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                }
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[faces]);
                meshVertexArray.push_back(tempMeshVertexArray[1]);

                // body
                for (size_t i = 0; i < stacks * 2 - 1; ++i)
                {
                    for (size_t j = 1; j < faces; ++j)
                    {
                        // p1 --- p3
                        // |   /  |
                        // |  /   |
                        // p2 --- p4
                        size_t i1 = i * faces + j;
                        size_t i2 = (i + 1) * faces + j;
                        size_t i3 = i1 + 1;
                        size_t i4 = i2 + 1;

                        meshVertexArray.push_back(tempMeshVertexArray[i1]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2]);
                        meshVertexArray.push_back(tempMeshVertexArray[i3]);
                        meshVertexArray.push_back(tempMeshVertexArray[i4]);
                        meshVertexArray.push_back(tempMeshVertexArray[i3]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    }
                    size_t i1 = (i + 1) * faces;
                    size_t i2 = (i + 2) * faces;
                    size_t i3 = i * faces + 1;
                    size_t i4 = (i + 1) * faces + 1;

                    meshVertexArray.push_back(tempMeshVertexArray[i1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i4]);
                    meshVertexArray.push_back(tempMeshVertexArray[i3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                }

                // bottom
                size_t lastIndex = positions.size() - 1;
                for (size_t i = 1; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[lastIndex]);
                    meshVertexArray.push_back(tempMeshVertexArray[lastIndex - i]);
                    meshVertexArray.push_back(tempMeshVertexArray[lastIndex - i - 1]);
                }
                meshVertexArray.push_back(tempMeshVertexArray[lastIndex]);
                meshVertexArray.push_back(tempMeshVertexArray[lastIndex - faces]);
                meshVertexArray.push_back(tempMeshVertexArray[lastIndex - 1]);

                for (size_t i = 0; i < faces * stacks * 12; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.normal = normals[i];
                    meshVertexArray.push_back(meshVertex);
                }

                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)initialVertexCount);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) + 1);
                }
                meshIndexArray.push_back((unsigned int)initialVertexCount);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces));
                meshIndexArray.push_back((unsigned int)initialVertexCount + 1);

                // body
                for (size_t i = 0; i < stacks * 2 - 1; ++i)
                {
                    for (size_t j = 1; j < faces; ++j)
                    {
                        // p1 --- p3
                        // |   /  |
                        // |  /   |
                        // p2 --- p4
                        size_t i1 = i * faces + j;
                        size_t i2 = (i + 1) * faces + j;
                        size_t i3 = i1 + 1;
                        size_t i4 = i2 + 1;

                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i1));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i4));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                    }
                    size_t i1 = (i + 1) * faces;
                    size_t i2 = (i + 2) * faces;
                    size_t i3 = i * faces + 1;
                    size_t i4 = (i + 1) * faces + 1;

                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i1));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i4));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                }

                // bottom
                size_t lastIndex = positions.size() - 1;
                for (size_t i = 1; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex - i));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex - i) - 1);
                }
                meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex));
                meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex - faces));
                meshIndexArray.push_back((unsigned int)(initialVertexCount + lastIndex) - 1);
            }
        }
        else
        {
            // top
            for (size_t i = 1; i < faces; ++i)
            {
                const glm::vec3& p1 = positions[0];
                const glm::vec3& p2 = positions[i];
                const glm::vec3& p3 = positions[i + 1];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }
            {
                const glm::vec3& p1 = positions[0];
                const glm::vec3& p2 = positions[faces];
                const glm::vec3& p3 = positions[1];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }

            // body
            for (size_t i = 0; i < stacks * 2 - 1; ++i)
            {
                for (size_t j = 1; j < faces; ++j)
                {
                    // p1 --- p3
                    // |   /  |
                    // |  /   |
                    // p2 --- p4
                    size_t i1 = i * faces + j;
                    size_t i2 = (i + 1) * faces + j;
                    size_t i3 = i1 + 1;
                    size_t i4 = i2 + 1;

                    const glm::vec3& p1 = positions[i1];
                    const glm::vec3& p2 = positions[i2];
                    const glm::vec3& p3 = positions[i3];
                    const glm::vec3& p4 = positions[i4];

                    glm::vec3 p1_to_p2 = p2 - p1;
                    glm::vec3 p2_to_p3 = p3 - p2;
                    glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                    MeshVertex v1;
                    v1.position = p1;
                    v1.normal = normal123;

                    MeshVertex v2;
                    v2.position = p2;
                    v2.normal = normal123;

                    MeshVertex v3;
                    v3.position = p3;
                    v3.normal = normal123;

                    MeshVertex v4;
                    v4.position = p4;
                    v4.normal = normal123;

                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
                    if (triangulate)
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v2);

                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                    }
                    else
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);

                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 3);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                    }
                }
                size_t i1 = (i + 1) * faces;
                size_t i2 = (i + 2) * faces;
                size_t i3 = i * faces + 1;
                size_t i4 = (i + 1) * faces + 1;

                const glm::vec3& p1 = positions[i1];
                const glm::vec3& p2 = positions[i2];
                const glm::vec3& p3 = positions[i3];
                const glm::vec3& p4 = positions[i4];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.normal = normal123;

                MeshVertex v4;
                v4.position = p4;
                v4.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
                if (triangulate)
                {
                    meshVertexArray.push_back(v1);
                    meshVertexArray.push_back(v2);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v4);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v2);

                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                    meshIndexArray.push_back(currentVertexCount++);
                }
                else
                {
                    meshVertexArray.push_back(v1);
                    meshVertexArray.push_back(v2);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v4);

                    meshIndexArray.push_back(currentVertexCount);
                    meshIndexArray.push_back(currentVertexCount + 1);
                    meshIndexArray.push_back(currentVertexCount + 2);
                    meshIndexArray.push_back(currentVertexCount + 3);
                    meshIndexArray.push_back(currentVertexCount + 2);
                    meshIndexArray.push_back(currentVertexCount + 1);
                }
            }

            // bottom
            size_t lastIndex = positions.size() - 1;
            for (size_t i = 1; i < faces; ++i)
            {
                const glm::vec3& p1 = positions[lastIndex];
                const glm::vec3& p2 = positions[lastIndex - i];
                const glm::vec3& p3 = positions[lastIndex - i - 1];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }
            {
                const glm::vec3& p1 = positions[lastIndex];
                const glm::vec3& p2 = positions[lastIndex - faces];
                const glm::vec3& p3 = positions[lastIndex - 1];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }
        }

        return true;
    }

    bool MeshLoader::GenerateCone(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || height <= 0.0f || faces < 3)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < 0.5f * height) maxVertexPosition.y = 0.5f * height;
        if (minVertexPosition.y > -0.5f * height) minVertexPosition.y = -0.5f * height;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = (faces + 1) * 3 - 2;
        positions.reserve(uniquePositions);
        textureCoordinates.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        float halfHeight = height * 0.5f;
        float hypotenuse = glm::sqrt(height * height + radius * radius);
        float halfBoarder = radius + hypotenuse;

        // top
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, halfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 1.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        // body bottom
        for (size_t j = 0; j < faces + 1; ++j) // +1 for wrap around
        {
            float longAngle = glm::pi<float>() / 2.0f - faceStep * (float)j;
            float x = radius * glm::cos(longAngle);
            float z = radius * glm::sin(longAngle);

            positions.emplace_back(x, -1.0f * halfHeight, z);
            textureCoordinates.emplace_back(
                (float)j / (float)faces,
                (halfBoarder - hypotenuse) / halfBoarder
            );
            if (smoothNormals)
            {
                normals.emplace_back(x / radius, 0.0f, z / radius);
            }
        }

        // center bottom
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, -1.0f * halfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 0.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, -1.0f, 0.0f);
            }
        }

        for (size_t i = faces; i < positions.size() - faces; ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        GenerateSphereMeshVerticesAndIndices(
            meshData,
            faces,
            2,
            positions,
            textureCoordinates,
            normals,
            smoothNormals,
            triangulate);

        return true;

        return false;
    }

    bool MeshLoader::GenerateCone_v2(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || height <= 0.0f || faces < 3)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < 0.5f * height) maxVertexPosition.y = 0.5f * height;
        if (minVertexPosition.y > -0.5f * height) minVertexPosition.y = -0.5f * height;

        size_t initialVertexCount = meshVertexArray.size();

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        if (smoothNormals)
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces + 1);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            MeshVertex topVertex;
            topVertex.position = glm::vec3(0.0f, 0.5f * height, 0.0f);
            topVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            destMeshVertexPtrArray->push_back(topVertex);

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * glm::sin(faceAngle);
                float z = radius * glm::cos(faceAngle);

                float vertexRadius = glm::sqrt(x * x + (0.5f * height) * (0.5f * height) + z * z);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < x) maxVertexPosition.x = x;
                if (maxVertexPosition.z < z) maxVertexPosition.z = z;

                if (minVertexPosition.x > x) minVertexPosition.x = x;
                if (minVertexPosition.z > z) minVertexPosition.z = z;

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(x, -0.5f * height, z);
                bottomVertex.normal = glm::normalize(bottomVertex.position);
                destMeshVertexPtrArray->push_back(bottomVertex);
            }

            if (triangulate)
            {
                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[0]);
                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                }
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[faces]);
                meshVertexArray.push_back(tempMeshVertexArray[1]);

                // bottom
                for (size_t i = 2; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                }

                for (size_t i = 0; i < faces * 6 - 6; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)initialVertexCount);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) + 1);
                }
                meshIndexArray.push_back((unsigned int)initialVertexCount);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces));
                meshIndexArray.push_back((unsigned int)initialVertexCount + 1);

                // bottom
                for (size_t i = 2; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)initialVertexCount + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
        }
        else
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 6 - 6);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            float previousX = 0.0f;
            float previousZ = radius;
            for (size_t i = 1; i < faces + 1; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float newX = radius * glm::sin(faceAngle);
                float newZ = radius * glm::cos(faceAngle);

                float vertexRadius = glm::sqrt(previousX * previousX + (0.5f * height) * (0.5f * height) + previousZ * previousZ);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < previousX) maxVertexPosition.x = previousX;
                if (maxVertexPosition.z < previousZ) maxVertexPosition.z = previousZ;

                if (minVertexPosition.x > previousX) minVertexPosition.x = previousX;
                if (minVertexPosition.z > previousZ) minVertexPosition.z = previousZ;

                glm::vec3 vectorToTop(0.0f - newX, 0.5f * height, 0.0f - newZ);
                glm::vec3 vectorToPrevious(previousX - newX, 0.0f, previousZ - newZ);
                glm::vec3 normal = glm::normalize(glm::cross(vectorToTop, vectorToPrevious));

                MeshVertex topVertex;
                topVertex.position = glm::vec3(0.0f, 0.5f * height, 0.0f);
                topVertex.normal = normal;
                destMeshVertexPtrArray->push_back(topVertex);

                MeshVertex previousVertex;
                previousVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                previousVertex.normal = normal;
                destMeshVertexPtrArray->push_back(previousVertex);

                MeshVertex newVertex;
                newVertex.position = glm::vec3(newX, -0.5f * height, newZ);
                newVertex.normal = normal;
                destMeshVertexPtrArray->push_back(newVertex);

                previousX = newX;
                previousZ = newZ;
            }

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * glm::sin(faceAngle);
                float z = radius * glm::cos(faceAngle);

                MeshVertex vertex;
                vertex.position = glm::vec3(x, -0.5f * height, z);
                vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                destMeshVertexPtrArray->push_back(vertex);
            }

            if (triangulate)
            {
                // top
                for (size_t i = 0; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[i * 3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 3 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 3 + 2]);
                }

                // bottom
                size_t firstBottomIndex = faces * 3;
                size_t pastLastBottomIndex = faces * 4;
                for (size_t i = firstBottomIndex; i < pastLastBottomIndex - 2; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[firstBottomIndex]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                }

                for (size_t i = 0; i < faces * 6 - 6; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                // top
                for (size_t i = 0; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) * 3);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) * 3 + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) * 3 + 2);
                }

                // bottom
                size_t firstBottomIndex = faces * 3;
                size_t pastLastBottomIndex = faces * 4;
                for (size_t i = firstBottomIndex; i < pastLastBottomIndex - 2; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + firstBottomIndex));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i) + 1);
                }
            }
        }

        return true;
    }

    bool MeshLoader::GenerateCylinder(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || height <= 0.0f || faces < 3)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < 0.5f * height) maxVertexPosition.y = 0.5f * height;
        if (minVertexPosition.y > -0.5f * height) minVertexPosition.y = -0.5f * height;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = (faces + 1) * 4 - 2;
        positions.reserve(uniquePositions);
        textureCoordinates.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        float halfHeight = height * 0.5f;
        float halfBoarder = radius * 2.0f + height;

        // top
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, halfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 1.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        // body top
        for (size_t j = 0; j < faces + 1; ++j) // +1 for wrap around
        {
            float longAngle = glm::pi<float>() / 2.0f - faceStep * (float)j;
            float x = radius * glm::cos(longAngle);
            float z = radius * glm::sin(longAngle);

            positions.emplace_back(x, halfHeight, z);
            textureCoordinates.emplace_back(
                (float)j / (float)faces,
                (halfBoarder - radius) / halfBoarder
            );
            if (smoothNormals)
            {
                normals.emplace_back(x / radius, 0.0f, z / radius);
            }
        }

        // body bottom
        for (size_t j = 0; j < faces + 1; ++j) // +1 for wrap around
        {
            float longAngle = glm::pi<float>() / 2.0f - faceStep * (float)j;
            float x = radius * glm::cos(longAngle);
            float z = radius * glm::sin(longAngle);

            positions.emplace_back(x, -1.0f * halfHeight, z);
            textureCoordinates.emplace_back(
                (float)j / (float)faces,
                radius / halfBoarder
            );
            if (smoothNormals)
            {
                normals.emplace_back(x / radius, 0.0f, z / radius);
            }
        }

        // bottom
        for (size_t i = 0; i < faces; ++i)
        {
            positions.emplace_back(0.0f, -1.0f * halfHeight, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)faces, 0.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, -1.0f, 0.0f);
            }
        }

        for (size_t i = faces; i < positions.size() - faces; ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        GenerateSphereMeshVerticesAndIndices(
            meshData,
            faces,
            3,
            positions,
            textureCoordinates,
            normals,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateCylinder_v2(
        MeshData& meshData,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || height <= 0.0f || faces < 3)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < 0.5f * height) maxVertexPosition.y = 0.5f * height;
        if (minVertexPosition.y > -0.5f * height) minVertexPosition.y = -0.5f * height;

        size_t initialVertexCount = meshVertexArray.size();

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        if (smoothNormals)
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 2);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            float y = 1.0f / glm::sqrt(2.0f);

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * glm::sin(faceAngle);
                float z = radius * glm::cos(faceAngle);

                float vertexRadius = glm::sqrt(x * x + (0.5f * height) * (0.5f * height) + z * z);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < x) maxVertexPosition.x = x;
                if (maxVertexPosition.z < z) maxVertexPosition.z = z;

                if (minVertexPosition.x > x) minVertexPosition.x = x;
                if (minVertexPosition.z > z) minVertexPosition.z = z;

                MeshVertex topVertex;
                topVertex.position = glm::vec3(x, 0.5f * height, z);
                topVertex.normal = glm::vec3(x * y / radius, y, z * y / radius);
                destMeshVertexPtrArray->push_back(topVertex);

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(x, -0.5f * height, z);
                bottomVertex.normal = glm::vec3(x * y / radius, -1.0f * y, z * y / radius);
                destMeshVertexPtrArray->push_back(bottomVertex);
            }

            if (triangulate)
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[0]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[(i + 1) * 2]);
                }

                // body
                for (size_t i = 0; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 1]);
                }
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 2]);
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 1]);
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[1]);
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 1]);

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[(i + 1) * 2 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[1]);
                }

                // unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
                // for (unsigned int i = (unsigned int)initialVertexCount; i < currentVertexCount; ++i)
                // {
                //     meshIndexArray.push_back(i);
                // }

                for (size_t i = 0; i < (faces - 1) * 12; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)initialVertexCount);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + (i + 1) * 2));
                }

                // body
                for (size_t i = 0; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 3);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 1);
                }
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 2);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 1);
                meshIndexArray.push_back((unsigned int)initialVertexCount);
                meshIndexArray.push_back((unsigned int)initialVertexCount + 1);
                meshIndexArray.push_back((unsigned int)initialVertexCount);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 1);

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + (i + 1) * 2) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount)+1);
                }
            }
        }
        else
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 6);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            float y = 1.0f / glm::sqrt(2.0f);

            float previousX = 0.0f;
            float previousZ = radius;
            for (size_t i = 1; i < faces + 1; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float newX = radius * glm::sin(faceAngle);
                float newZ = radius * glm::cos(faceAngle);

                float vertexRadius = glm::sqrt(previousX * previousX + (0.5f * height) * (0.5f * height) + previousZ * previousZ);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < previousX) maxVertexPosition.x = previousX;
                if (maxVertexPosition.z < previousZ) maxVertexPosition.z = previousZ;

                if (minVertexPosition.x > previousX) minVertexPosition.x = previousX;
                if (minVertexPosition.z > previousZ) minVertexPosition.z = previousZ;

                glm::vec3 vectorToPrevious(previousX - newX, 0.0f, previousZ - newZ);
                glm::vec3 bodyNormal = glm::normalize(glm::cross(vectorToPrevious, glm::vec3(0.0f, -1.0f, 0.0f)));

                MeshVertex topVertex;
                topVertex.position = glm::vec3(previousX, 0.5f * height, previousZ);
                topVertex.normal = glm::vec3(0.0f, y, 0.0f);
                destMeshVertexPtrArray->push_back(topVertex);

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                bottomVertex.normal = glm::vec3(0.0f, -1.0f * y, 0.0f);
                destMeshVertexPtrArray->push_back(bottomVertex);

                MeshVertex topLeftBodyVertex;
                topLeftBodyVertex.position = glm::vec3(previousX, 0.5f * height, previousZ);
                topLeftBodyVertex.normal = bodyNormal;
                destMeshVertexPtrArray->push_back(topLeftBodyVertex);

                MeshVertex bottomLeftBodyVertex;
                bottomLeftBodyVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                bottomLeftBodyVertex.normal = bodyNormal;
                destMeshVertexPtrArray->push_back(bottomLeftBodyVertex);

                MeshVertex topRightBodyVertex;
                topRightBodyVertex.position = glm::vec3(newX, 0.5f * height, newZ);
                topRightBodyVertex.normal = bodyNormal;
                destMeshVertexPtrArray->push_back(topRightBodyVertex);

                MeshVertex bottomRightBodyVertex;
                bottomRightBodyVertex.position = glm::vec3(newX, -0.5f * height, newZ);
                bottomRightBodyVertex.normal = bodyNormal;
                destMeshVertexPtrArray->push_back(bottomRightBodyVertex);

                previousX = newX;
                previousZ = newZ;
            }

            if (triangulate)
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[0]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6]);
                    meshVertexArray.push_back(tempMeshVertexArray[(i + 1) * 6]);
                }

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[(i + 1) * 6 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[1]);
                }

                // body
                for (size_t i = 0; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 4]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 5]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 4]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 6 + 3]);
                }

                for (size_t i = 0; i < (faces - 1) * 12; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)initialVertexCount);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + (i + 1) * 6));
                }

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + (i + 1) * 6) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 1);
                    meshIndexArray.push_back((unsigned int)initialVertexCount + 1);
                }

                // body
                for (size_t i = 0; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 3);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 4);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 5);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 4);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 6) + 3);
                }
            }
        }

        return true;
    }

    bool MeshLoader::GenerateHemisphere(
        MeshData& meshData,
        float radius,
        size_t longitudinalSections,
        size_t latitudinalSections,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || longitudinalSections < 3 || latitudinalSections < 2)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < radius) maxVertexPosition.y = radius;
        if (minVertexPosition.y > -radius) minVertexPosition.y = -radius;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = (longitudinalSections + 1) * (latitudinalSections + 2) - 2;
        positions.reserve(uniquePositions);
        textureCoordinates.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float longStep = 2.0f * glm::pi<float>() / (float)(longitudinalSections);
        const float latiStep = 0.5f * glm::pi<float>() / (float)(latitudinalSections);

        float quarterCircleRadius = radius * glm::half_pi<float>();
        float halfBoarder = quarterCircleRadius + radius;

        // top
        for (size_t i = 0; i < longitudinalSections; ++i)
        {
            positions.emplace_back(0.0f, 0.5f * radius, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)longitudinalSections, 1.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        // body
        for (size_t i = 1; i < latitudinalSections + 1; ++i)
        {
            float latiAngle = glm::pi<float>() / 2.0f - latiStep * (float)i;
            float xz = radius * glm::cos(latiAngle);
            float y = radius * glm::sin(latiAngle) - 0.5f * radius;

            for (size_t j = 0; j < longitudinalSections + 1; ++j) // +1 for wrap around
            {
                float longAngle = glm::pi<float>() / 2.0f - longStep * (float)j;
                float x = xz * glm::cos(longAngle);
                float z = xz * glm::sin(longAngle);

                positions.emplace_back(x, y, z);
                textureCoordinates.emplace_back(
                    (float)j / (float)longitudinalSections,
                    (halfBoarder - (float)i / (float)latitudinalSections * quarterCircleRadius) / halfBoarder
                );
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // bottom
        for (size_t i = 0; i < longitudinalSections; ++i)
        {
            positions.emplace_back(0.0f, -0.5f * radius, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)longitudinalSections, 0.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, -1.0f, 0.0f);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        GenerateSphereMeshVerticesAndIndices(
            meshData,
            longitudinalSections,
            latitudinalSections + 1,
            positions,
            textureCoordinates,
            normals,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateIcosphere(
        MeshData& meshData,
        float radius,
        size_t subdivisions,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < radius) maxVertexPosition.y = radius;
        if (minVertexPosition.y > -radius) minVertexPosition.y = -radius;

        // Calculations initalialy done using Polar Coordinates:
        // x = radial distance of a point from the origin
        //     range = [0, infinity)
        // y = polar angle from the +z axis towards the -z axis
        //     range = [0, _PI]
        // z = azimouth angle of projection in the xy plane, starting from the
        //     +x axis and going around counter clockwise
        //     range = [0, 2 * _PI]

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;

        positions.reserve(22);
        textureCoordinates.reserve(22);

        // initial verticies
        // ---------------------------------------------------------------------
        //    00    01    02    03    04       <- top
        //   /  \  /  \  /  \  /  \  /  \
        // 05 -- 06 -- 07 -- 08 -- 09 -- 10    <- body
        //   \  /  \  /  \  /  \  /  \  /  \
        //    11 -- 12 -- 13 -- 14 -- 15 -- 16 <- body
        //      \  /  \  /  \  /  \  /  \  /
        //       17    18     19   20    21    <- bottom

        constexpr const float hAngle = 0.4f * glm::pi<float>();
        const float vAngle = 0.5f * glm::pi<float>() - glm::atan(0.5f);

        // top
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, 0.0f, ((float)i + 0.5f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 10.0f, 1.0f);
        }

        // body
        for (size_t i = 0; i < 6; ++i)
        {
            positions.emplace_back(radius, vAngle, (float)i * hAngle);
            textureCoordinates.emplace_back((float)(i * 2) / 10.0f, 2.0f / 3.0f);
        }

        for (size_t i = 0; i < 6; ++i)
        {
            positions.emplace_back(radius, 2.0f * vAngle, ((float)i + 0.5f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 10.0f, 1.0f / 3.0f);
        }

        // bottom
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>(), ((float)i + 1.0f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 2) / 10.0f, 0.0f);
        }

        // convert positions from polar coordinates to cartesian coordinates
        // ---------------------------------------------------------------------
        for (size_t i = 0; i < positions.size(); ++i)
        {
            float x = positions[i].x;
            float y = positions[i].y;
            float z = positions[i].z;
            positions[i].z = x * glm::sin(y) * glm::cos(z);
            positions[i].x = x * glm::sin(y) * glm::sin(z);
            positions[i].y = x * glm::cos(y);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        // initial triangles
        // ---------------------------------------------------------------------
        std::vector<glm::uvec3> triangleFaces;

        triangleFaces.emplace_back(0, 5, 6);
        triangleFaces.emplace_back(1, 6, 7);
        triangleFaces.emplace_back(2, 7, 8);
        triangleFaces.emplace_back(3, 8, 9);
        triangleFaces.emplace_back(4, 9, 10);

        triangleFaces.emplace_back(5, 11, 6);
        triangleFaces.emplace_back(6, 12, 7);
        triangleFaces.emplace_back(7, 13, 8);
        triangleFaces.emplace_back(8, 14, 9);
        triangleFaces.emplace_back(9, 15, 10);

        triangleFaces.emplace_back(6, 11, 12);
        triangleFaces.emplace_back(7, 12, 13);
        triangleFaces.emplace_back(8, 13, 14);
        triangleFaces.emplace_back(9, 14, 15);
        triangleFaces.emplace_back(10, 15, 16);

        triangleFaces.emplace_back(11, 17, 12);
        triangleFaces.emplace_back(12, 18, 13);
        triangleFaces.emplace_back(13, 19, 14);
        triangleFaces.emplace_back(14, 20, 15);
        triangleFaces.emplace_back(15, 21, 16);

        // subdivide
        // ---------------------------------------------------------------------
        SubdivideIcosphereTriangleFaces(
            positions,
            textureCoordinates,
            triangleFaces,
            radius,
            subdivisions);

        // generate meshVerticies
        // ---------------------------------------------------------------------
        GenerateIcosphereMeshVerticesAndIndices(
            meshData,
            radius,
            positions,
            textureCoordinates,
            triangleFaces,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateIcosphere_v2(
        MeshData& meshData,
        float radius,
        size_t subdivisions,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < radius) maxVertexPosition.y = radius;
        if (minVertexPosition.y > -radius) minVertexPosition.y = -radius;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;

        positions.reserve(10);
        textureCoordinates.reserve(10);

        // initial verticies
        // ---------------------------------------------------------------------
        //    00    01    02    <- top
        //   /  \  /  \  /  \
        // 03 -- 04 -- 05 -- 06 <- body
        //   \  /  \  /  \  /
        //    07    08     09   <- bottom

        constexpr const float hAngle = 2.0f / 3.0f * glm::pi<float>();

        // top
        for (size_t i = 0; i < 3; ++i)
        {
            positions.emplace_back(radius, 0.0f, 0.0f);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 6.0f, 1.0f);
        }

        // body
        for (size_t i = 0; i < 4; ++i)
        {
            positions.emplace_back(radius, 0.5f * glm::pi<float>(), (float)i * hAngle);
            textureCoordinates.emplace_back((float)(i * 2) / 6.0f, 0.5f);
        }

        // bottom
        for (size_t i = 0; i < 3; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>(), 0.0f);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 6.0f, 0.0f);
        }

        // convert positions from polar coordinates to cartesian coordinates
        // ---------------------------------------------------------------------
        for (size_t i = 0; i < positions.size(); ++i)
        {
            float x = positions[i].x;
            float y = positions[i].y;
            float z = positions[i].z;
            positions[i].z = x * glm::sin(y) * glm::cos(z);
            positions[i].x = x * glm::sin(y) * glm::sin(z);
            positions[i].y = x * glm::cos(y);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        // initial triangles
        // ---------------------------------------------------------------------
        std::vector<glm::uvec3> triangleFaces;

        triangleFaces.emplace_back(0, 3, 4);
        triangleFaces.emplace_back(1, 4, 5);
        triangleFaces.emplace_back(2, 5, 6);

        triangleFaces.emplace_back(3, 7, 4);
        triangleFaces.emplace_back(4, 8, 5);
        triangleFaces.emplace_back(5, 9, 6);

        // subdivide
        // ---------------------------------------------------------------------
        SubdivideIcosphereTriangleFaces(
            positions,
            textureCoordinates,
            triangleFaces,
            radius,
            subdivisions);

        // generate meshVerticies
        // ---------------------------------------------------------------------
        GenerateIcosphereMeshVerticesAndIndices(
            meshData,
            radius,
            positions,
            textureCoordinates,
            triangleFaces,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateIcosphereAsteroid(
        MeshData& meshData,
        std::vector<glm::vec2>& borderPoints,
        float radius,
        uint32_t offsetSeed,
        float maxOffsetLength,
        size_t subdivisions,
        std::vector<float>& vertexOffsetDistances,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < radius) maxVertexPosition.y = radius;
        if (minVertexPosition.y > -radius) minVertexPosition.y = -radius;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<uint32_t> offsetSeeds;

        positions.reserve(10);
        textureCoordinates.reserve(10);
        offsetSeeds.reserve(10);

        // initial verticies
        // ---------------------------------------------------------------------
        //    00    01    02    <- top
        //   /  \  /  \  /  \
        // 03 -- 04 -- 05 -- 06 <- body
        //   \  /  \  /  \  /
        //    07    08     09   <- bottom

        offsetSeeds.push_back(0);
        offsetSeeds.push_back(0);
        offsetSeeds.push_back(0);
        offsetSeeds.push_back(1);
        offsetSeeds.push_back(2);
        offsetSeeds.push_back(3);
        offsetSeeds.push_back(1);
        offsetSeeds.push_back(4);
        offsetSeeds.push_back(4);
        offsetSeeds.push_back(4);

        constexpr const float hAngle = 2.0f / 3.0f * glm::pi<float>();

        // top
        for (size_t i = 0; i < 3; ++i)
        {
            positions.emplace_back(radius, 0.0f, 0.0f);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 6.0f, 1.0f);
        }

        // body
        for (size_t i = 0; i < 4; ++i)
        {
            positions.emplace_back(radius, 0.5f * glm::pi<float>(), (float)i * hAngle);
            textureCoordinates.emplace_back((float)(i * 2) / 6.0f, 0.5f);
        }

        // bottom
        for (size_t i = 0; i < 3; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>(), 0.0f);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 6.0f, 0.0f);
        }

        // convert positions from polar coordinates to cartesian coordinates
        // ---------------------------------------------------------------------
        for (size_t i = 0; i < positions.size(); ++i)
        {
            float x = positions[i].x;
            float y = positions[i].y;
            float z = positions[i].z;
            positions[i].z = x * glm::sin(y) * glm::cos(z);
            positions[i].x = x * glm::sin(y) * glm::sin(z);
            positions[i].y = x * glm::cos(y);
        }

        // initial triangles
        // ---------------------------------------------------------------------
        std::vector<glm::uvec3> triangleFaces;

        triangleFaces.emplace_back(0, 3, 4);
        triangleFaces.emplace_back(1, 4, 5);
        triangleFaces.emplace_back(2, 5, 6);

        triangleFaces.emplace_back(3, 7, 4);
        triangleFaces.emplace_back(4, 8, 5);
        triangleFaces.emplace_back(5, 9, 6);

        // subdivide
        // ---------------------------------------------------------------------
        SubdivideIcosphereTriangleFaces_v2(
            positions,
            textureCoordinates,
            offsetSeeds,
            triangleFaces,
            radius,
            subdivisions);

        for (size_t i = 0; i < positions.size(); ++i)
        {
            glm::vec3& currentPosition = positions[i];
            uint32_t currentOffsetSeed = offsetSeed + offsetSeeds[i];
            float offset = maxOffsetLength * (GetRandomFloat(currentOffsetSeed) - 1.0f);

            float positionLength = glm::sqrt(currentPosition.x * currentPosition.x + currentPosition.y * currentPosition.y + currentPosition.z * currentPosition.z);
            currentPosition *= (offset + positionLength) / positionLength;
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            if (FloatEqualToFloat(currentPosition.y, 0.0f))
            {
                borderPoints.emplace_back(currentPosition.x, -currentPosition.z); // negative z because right handed coordinate system
            }
        }

        for (size_t i = 0; i < borderPoints.size(); ++i)
        {
            const glm::vec2& currentPosition = borderPoints[i];
            borderPoints[i] = CartesianToPolar(currentPosition);
        }

        std::sort(borderPoints.begin(), borderPoints.end(),
            [](const glm::vec2& a, const glm::vec2& b)
            {
                return a.y < b.y; // Sort in ascending order based on t value
            }
        );

        // the start and end points will be the same, so the duplicate needs to be removed
        for (size_t i = 0; i < borderPoints.size() - 1; ++i)
        {
            if (FloatEqualToFloat(borderPoints[i].y, borderPoints[i + 1].y))
            {
                borderPoints.erase(borderPoints.begin() + i);
            }
        }

        for (size_t i = 0; i < borderPoints.size(); ++i)
        {
            const glm::vec2& currentPosition = borderPoints[i];
            borderPoints[i] = PolarToCartesian(currentPosition);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        // generate meshVerticies
        // ---------------------------------------------------------------------
        GenerateIcosphereMeshVerticesAndIndices(
            meshData,
            radius,
            positions,
            textureCoordinates,
            triangleFaces,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateSphere(
        MeshData& meshData,
        float radius,
        size_t longitudinalSections, // west/east
        size_t latitudinalSections, // north/south
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || longitudinalSections < 3 || latitudinalSections < 2)
        {
            return false;
        }

        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.y < radius) maxVertexPosition.y = radius;
        if (minVertexPosition.y > -radius) minVertexPosition.y = -radius;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        size_t uniquePositions = (longitudinalSections + 1) * (latitudinalSections + 1) - 2;
        positions.reserve(uniquePositions);
        textureCoordinates.reserve(uniquePositions);
        if (smoothNormals)
        {
            normals.reserve(uniquePositions);
        }

        const float longStep = 2.0f * glm::pi<float>() / (float)(longitudinalSections);
        const float latiStep = glm::pi<float>() / (float)(latitudinalSections);

        // top
        for (size_t i = 0; i < longitudinalSections; ++i)
        {
            positions.emplace_back(0.0f, radius, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)longitudinalSections, 1.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        // body
        for (size_t i = 1; i < latitudinalSections; ++i)
        {
            float latiAngle = glm::pi<float>() / 2.0f - latiStep * (float)i;
            float xz = radius * glm::cos(latiAngle);
            float y = radius * glm::sin(latiAngle);

            for (size_t j = 0; j < longitudinalSections + 1; ++j) // +1 for wrap around
            {
                float longAngle = glm::pi<float>() / 2.0f - longStep * (float)j;
                float x = xz * glm::cos(longAngle);
                float z = xz * glm::sin(longAngle);

                positions.emplace_back(x, y, z);
                textureCoordinates.emplace_back(
                    (float)j / (float)longitudinalSections,
                    (float)(latitudinalSections - i) / (float)latitudinalSections
                );
                if (smoothNormals)
                {
                    normals.emplace_back(x / radius, y / radius, z / radius);
                }
            }
        }

        // bottom
        for (size_t i = 0; i < longitudinalSections; ++i)
        {
            positions.emplace_back(0.0f, -1.0f * radius, 0.0f);
            textureCoordinates.emplace_back(((float)i + 0.5f) / (float)longitudinalSections, 0.0f);
            if (smoothNormals)
            {
                normals.emplace_back(0.0f, -1.0f, 0.0f);
            }
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        GenerateSphereMeshVerticesAndIndices(
            meshData,
            longitudinalSections,
            latitudinalSections,
            positions,
            textureCoordinates,
            normals,
            smoothNormals,
            triangulate);

        return true;
    }

    bool MeshLoader::GenerateTriangles(
        MeshData& meshData,
        const std::vector<glm::vec3>& positions)
    {
        if (positions.size() == 0 || positions.size() % 3 != 0)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        size_t currentVertexCount = meshVertexArray.size();

        for (size_t i = 0; i < positions.size() - 2; i += 3)
        {
            const glm::vec3& position1 = positions[i];
            const glm::vec3& position2 = positions[i + 1];
            const glm::vec3& position3 = positions[i + 2];
            glm::vec3 normal = glm::cross(position2 - position1, position3 - position1);

            MeshVertex meshVertex;
            meshVertex.position = position1;
            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((unsigned int)(currentVertexCount + i));

            meshVertex.position = position2;
            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 1));

            meshVertex.position = position3;
            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((unsigned int)(currentVertexCount + i + 2));
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;
            if (maxVertexPosition.z < currentPosition.z) maxVertexPosition.z = currentPosition.z;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
            if (minVertexPosition.z > currentPosition.z) minVertexPosition.z = currentPosition.z;
        }

        return true;
    }

    bool MeshLoader::GenerateTube(
        MeshData& meshData,
        const glm::vec3& start,
        const glm::vec3& end,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f || faces < 3 || start == end)
        {
            return false;
        }

        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        size_t initialVertexCount = meshVertexArray.size();

        glm::vec3 direction = end - start;
        glm::vec3 otherDirection(-direction.z, direction.x, direction.y);
        glm::vec3 unitPerpendicular = glm::normalize(glm::cross(direction, otherDirection));

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);
        const glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), faceStep, direction);

        if (smoothNormals)
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 2);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            for (size_t i = 0; i < faces; ++i)
            {
                glm::vec3 perpendicular = unitPerpendicular * radius;

                MeshVertex topVertex;
                topVertex.position = start + perpendicular;
                topVertex.normal = unitPerpendicular;
                destMeshVertexPtrArray->push_back(topVertex);

                MeshVertex bottomVertex;
                bottomVertex.position = end + perpendicular;
                bottomVertex.normal = unitPerpendicular;
                destMeshVertexPtrArray->push_back(bottomVertex);

                float vertexRadius = glm::length(topVertex.position);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < topVertex.position.x) maxVertexPosition.x = topVertex.position.x;
                if (maxVertexPosition.y < topVertex.position.y) maxVertexPosition.y = topVertex.position.y;
                if (maxVertexPosition.z < topVertex.position.z) maxVertexPosition.z = topVertex.position.z;

                if (minVertexPosition.x > topVertex.position.x) minVertexPosition.x = topVertex.position.x;
                if (minVertexPosition.y > topVertex.position.y) minVertexPosition.y = topVertex.position.y;
                if (minVertexPosition.z > topVertex.position.z) minVertexPosition.z = topVertex.position.z;

                vertexRadius = glm::length(bottomVertex.position);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < bottomVertex.position.x) maxVertexPosition.x = bottomVertex.position.x;
                if (maxVertexPosition.y < bottomVertex.position.y) maxVertexPosition.y = bottomVertex.position.y;
                if (maxVertexPosition.z < bottomVertex.position.z) maxVertexPosition.z = bottomVertex.position.z;

                if (minVertexPosition.x > bottomVertex.position.x) minVertexPosition.x = bottomVertex.position.x;
                if (minVertexPosition.y > bottomVertex.position.y) minVertexPosition.y = bottomVertex.position.y;
                if (minVertexPosition.z > bottomVertex.position.z) minVertexPosition.z = bottomVertex.position.z;

                unitPerpendicular = rotation * unitPerpendicular;
            }

            if (triangulate)
            {
                for (size_t i = 0; i < faces - 1; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 2 + 2]);
                }
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 2]);
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 1]);
                meshVertexArray.push_back(tempMeshVertexArray[1]);
                meshVertexArray.push_back(tempMeshVertexArray[faces * 2 - 1]);
                meshVertexArray.push_back(tempMeshVertexArray[0]);

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
                for (unsigned int i = (unsigned int)initialVertexCount; i < currentVertexCount; ++i)
                {
                    meshIndexArray.push_back(i);
                }

                for (size_t i = 0; i < faces * 6; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                for (size_t i = 0; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 3);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 2) + 2);
                }
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 2);
                meshIndexArray.push_back((unsigned int)initialVertexCount);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 1);
                meshIndexArray.push_back((unsigned int)initialVertexCount + 1);
                meshIndexArray.push_back((unsigned int)(initialVertexCount + faces * 2) - 1);
                meshIndexArray.push_back((unsigned int)initialVertexCount);
            }
        }
        else
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexPtrArray;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 4);
                destMeshVertexPtrArray = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexPtrArray = &meshVertexArray;
            }

            const glm::quat halfRotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), faceStep * 0.5f, direction);

            for (size_t i = 1; i < faces + 1; ++i)
            {
                glm::vec3 perpendicular = unitPerpendicular * radius;
                glm::vec3 normal = halfRotation * unitPerpendicular;

                MeshVertex topLeftBodyVertex;
                topLeftBodyVertex.position = start + perpendicular;
                topLeftBodyVertex.normal = normal;
                destMeshVertexPtrArray->push_back(topLeftBodyVertex);

                MeshVertex bottomLeftBodyVertex;
                bottomLeftBodyVertex.position = end + perpendicular;
                bottomLeftBodyVertex.normal = normal;
                destMeshVertexPtrArray->push_back(bottomLeftBodyVertex);

                unitPerpendicular = rotation * unitPerpendicular;
                perpendicular = unitPerpendicular * radius;

                MeshVertex topRightBodyVertex;
                topRightBodyVertex.position = start + perpendicular;
                topRightBodyVertex.normal = normal;
                destMeshVertexPtrArray->push_back(topRightBodyVertex);

                MeshVertex bottomRightBodyVertex;
                bottomRightBodyVertex.position = end + perpendicular;
                bottomRightBodyVertex.normal = normal;
                destMeshVertexPtrArray->push_back(bottomRightBodyVertex);

                float vertexRadius = glm::length(topLeftBodyVertex.position);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < topLeftBodyVertex.position.x) maxVertexPosition.x = topLeftBodyVertex.position.x;
                if (maxVertexPosition.y < topLeftBodyVertex.position.y) maxVertexPosition.y = topLeftBodyVertex.position.y;
                if (maxVertexPosition.z < topLeftBodyVertex.position.z) maxVertexPosition.z = topLeftBodyVertex.position.z;

                if (minVertexPosition.x > topLeftBodyVertex.position.x) minVertexPosition.x = topLeftBodyVertex.position.x;
                if (minVertexPosition.y > topLeftBodyVertex.position.y) minVertexPosition.y = topLeftBodyVertex.position.y;
                if (minVertexPosition.z > topLeftBodyVertex.position.z) minVertexPosition.z = topLeftBodyVertex.position.z;

                vertexRadius = glm::length(bottomLeftBodyVertex.position);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < bottomLeftBodyVertex.position.x) maxVertexPosition.x = bottomLeftBodyVertex.position.x;
                if (maxVertexPosition.y < bottomLeftBodyVertex.position.y) maxVertexPosition.y = bottomLeftBodyVertex.position.y;
                if (maxVertexPosition.z < bottomLeftBodyVertex.position.z) maxVertexPosition.z = bottomLeftBodyVertex.position.z;

                if (minVertexPosition.x > bottomLeftBodyVertex.position.x) minVertexPosition.x = bottomLeftBodyVertex.position.x;
                if (minVertexPosition.y > bottomLeftBodyVertex.position.y) minVertexPosition.y = bottomLeftBodyVertex.position.y;
                if (minVertexPosition.z > bottomLeftBodyVertex.position.z) minVertexPosition.z = bottomLeftBodyVertex.position.z;

            }

            if (triangulate)
            {
                // body
                for (size_t i = 0; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4 + 2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4 + 3]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4 + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i * 4 + 2]);
                }

                for (size_t i = 0; i < faces * 6; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                for (size_t i = 0; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4) + 2);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4) + 3);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4) + 1);
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i * 4) + 2);
                }
            }
        }

        return true;
    }

    void MeshLoader::CopyMesh(
        MeshData& destinationMeshData,
        const MeshData& sourceMeshData)
    {
        if (destinationMeshData.maxBoundingRadius < sourceMeshData.maxBoundingRadius) destinationMeshData.maxBoundingRadius = sourceMeshData.maxBoundingRadius;

        if (destinationMeshData.maxVertexPosition.x < sourceMeshData.maxVertexPosition.x) destinationMeshData.maxVertexPosition.x = sourceMeshData.maxVertexPosition.x;
        if (destinationMeshData.maxVertexPosition.y < sourceMeshData.maxVertexPosition.y) destinationMeshData.maxVertexPosition.y = sourceMeshData.maxVertexPosition.y;
        if (destinationMeshData.maxVertexPosition.z < sourceMeshData.maxVertexPosition.z) destinationMeshData.maxVertexPosition.z = sourceMeshData.maxVertexPosition.z;

        if (destinationMeshData.minVertexPosition.x > sourceMeshData.minVertexPosition.x) destinationMeshData.minVertexPosition.x = sourceMeshData.minVertexPosition.x;
        if (destinationMeshData.minVertexPosition.y > sourceMeshData.minVertexPosition.y) destinationMeshData.minVertexPosition.y = sourceMeshData.minVertexPosition.y;
        if (destinationMeshData.minVertexPosition.z > sourceMeshData.minVertexPosition.z) destinationMeshData.minVertexPosition.z = sourceMeshData.minVertexPosition.z;

        unsigned int indexOffset = destinationMeshData.meshVertexArray.size();

        for (size_t i = 0; i < sourceMeshData.meshVertexArray.size(); ++i)
        {
            destinationMeshData.meshVertexArray.push_back(sourceMeshData.meshVertexArray[i]);
        }

        for (size_t i = 0; i < sourceMeshData.meshIndexArray.size(); ++i)
        {
            destinationMeshData.meshIndexArray.push_back(indexOffset + sourceMeshData.meshIndexArray[i]);
        }
    }

    void MeshLoader::NormalizeMeshSize(MeshData& meshData)
    {
        glm::vec3 size = meshData.maxVertexPosition - meshData.minVertexPosition;
        float biggestDimension = size.x;
        if (size.y > size.x && size.y > size.z)
        {
            biggestDimension = size.y;
        }
        else if (size.z > size.x && size.z > size.y)
        {
            biggestDimension = size.z;
        }

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            meshData.meshVertexArray[i].position /= biggestDimension;
        }

        meshData.maxBoundingRadius /= biggestDimension;
        meshData.maxVertexPosition /= biggestDimension;
        meshData.minVertexPosition /= biggestDimension;
    }

    void MeshLoader::RecenterMesh(MeshData& meshData)
    {
        meshData.maxBoundingRadius = 0;

        glm::vec3 centerShift = (meshData.maxVertexPosition + meshData.minVertexPosition) / 2.0f;
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec3& currentPosition = meshData.meshVertexArray[i].position;
            currentPosition -= centerShift;

            float vertexRadius = glm::length(currentPosition);
            if (meshData.maxBoundingRadius < vertexRadius) meshData.maxBoundingRadius = vertexRadius;
        }

        meshData.maxVertexPosition -= centerShift;
        meshData.minVertexPosition -= centerShift;
    }

    void MeshLoader::RecalculateMeshMinMax(MeshData& meshData)
    {
        meshData.maxBoundingRadius = 0;
        meshData.maxVertexPosition.x = -1.0f * std::numeric_limits<float>::infinity();
        meshData.maxVertexPosition.y = -1.0f * std::numeric_limits<float>::infinity();
        meshData.maxVertexPosition.z = -1.0f * std::numeric_limits<float>::infinity();
        meshData.minVertexPosition.x = std::numeric_limits<float>::infinity();
        meshData.minVertexPosition.y = std::numeric_limits<float>::infinity();
        meshData.minVertexPosition.z = std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            const glm::vec3& currentPosition = meshData.meshVertexArray[i].position;
            float vertexRadius = glm::length(currentPosition);
            if (meshData.maxBoundingRadius < vertexRadius) meshData.maxBoundingRadius = vertexRadius;

            if (meshData.maxVertexPosition.x < currentPosition.x) meshData.maxVertexPosition.x = currentPosition.x;
            if (meshData.maxVertexPosition.y < currentPosition.y) meshData.maxVertexPosition.y = currentPosition.y;
            if (meshData.maxVertexPosition.z < currentPosition.z) meshData.maxVertexPosition.z = currentPosition.z;

            if (meshData.minVertexPosition.x > currentPosition.x) meshData.minVertexPosition.x = currentPosition.x;
            if (meshData.minVertexPosition.y > currentPosition.y) meshData.minVertexPosition.y = currentPosition.y;
            if (meshData.minVertexPosition.z > currentPosition.z) meshData.minVertexPosition.z = currentPosition.z;
        }
    }

    void MeshLoader::RotateMesh(
        MeshData& meshData,
        glm::quat rotation)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec3& position = meshData.meshVertexArray[i].position;
            glm::vec3& normal = meshData.meshVertexArray[i].normal;

            position = rotation * position;
            normal = rotation * normal;
        }

        RecalculateMeshMinMax_v2(meshData);
    }

    void MeshLoader::RotateMeshX(
        MeshData& meshData,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        RotateMesh(meshData, rotation);
    }

    void MeshLoader::RotateMeshY(
        MeshData& meshData,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        RotateMesh(meshData, rotation);
    }

    void MeshLoader::RotateMeshZ(
        MeshData& meshData,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        RotateMesh(meshData, rotation);
    }

    void MeshLoader::ScaleMesh(
        MeshData& meshData,
        glm::vec3 scale)
    {
        meshData.maxBoundingRadius = 0;

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec3& currentPosition = meshData.meshVertexArray[i].position;
            currentPosition *= scale;

            float vertexRadius = glm::length(currentPosition);
            if (meshData.maxBoundingRadius < vertexRadius) meshData.maxBoundingRadius = vertexRadius;
        }

        meshData.maxVertexPosition *= scale;
        meshData.minVertexPosition *= scale;
    }

    void MeshLoader::TranslateMesh(
        MeshData& meshData,
        glm::vec3 translation)
    {
        meshData.maxBoundingRadius = 0;

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec3& currentPosition = meshData.meshVertexArray[i].position;
            currentPosition += translation;

            float vertexRadius = glm::length(currentPosition);
            if (meshData.maxBoundingRadius < vertexRadius) meshData.maxBoundingRadius = vertexRadius;
        }

        meshData.maxVertexPosition += translation;
        meshData.minVertexPosition += translation;
    }

    void MeshLoader::SizeMeshAlongNormals(
        MeshData& meshData,
        float size)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            MeshVertex& currentMeshVertex = meshData.meshVertexArray[i];
            currentMeshVertex.position += currentMeshVertex.normal * size;
        }

        RecalculateMeshMinMax_v2(meshData);
    }

    void MeshLoader::TurnInsideOut(
        MeshData& meshData,
        bool wasTriangulated)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            meshData.meshVertexArray[i].normal *= -1.0f;
        }

        if (wasTriangulated)
        {
            for (size_t i = 0; i < meshData.meshVertexArray.size() / 2; ++i)
            {
                size_t& vertexIndex0 = i;
                size_t vertexIndex1 = meshData.meshVertexArray.size() - 1 - i;

                MeshVertex& meshVertex0 = meshData.meshVertexArray[vertexIndex0];
                MeshVertex& meshVertex1 = meshData.meshVertexArray[vertexIndex1];

                MeshVertex temp = meshVertex0;
                meshVertex0 = meshVertex1;
                meshVertex1 = temp;
            }

            for (size_t i = 0; i < meshData.meshIndexArray.size(); ++i)
            {
                unsigned int& currentIndex = meshData.meshIndexArray[i];
                currentIndex = (unsigned int)meshData.meshIndexArray.size() - 1 - currentIndex;
            }
        }

        for (size_t i = 0; i < meshData.meshIndexArray.size() - 2; i += 3)
        {
            unsigned int& index0 = meshData.meshIndexArray[i];
            unsigned int& index1 = meshData.meshIndexArray[i + 2];

            unsigned int temp = index0;
            index0 = index1;
            index1 = temp;
        }
    }

    void MeshLoader::ApplyTextureCoordinates(
        MeshData& meshData,
        const std::vector<glm::vec2>& textureCoordinates)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size() && i < textureCoordinates.size(); ++i)
        {
            MeshVertex& meshVertex = meshData.meshVertexArray[i];
            meshVertex.textureCoordinate = textureCoordinates[i];
        }
    }

    void MeshLoader::ApplyPositionalTextureCoordinates(MeshData& meshData)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            MeshVertex& meshVertex = meshData.meshVertexArray[i];
            meshVertex.textureCoordinate = meshVertex.position;
        }
    }

    void MeshLoader::RotateTextureCoordinates(
        MeshData& meshData,
        float rotation)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec2& textureCoordinate = meshData.meshVertexArray[i].textureCoordinate;
            textureCoordinate = Rotate2DVector(textureCoordinate, rotation);
        }
    }

    void MeshLoader::ScaleTextureCoordinates(
        MeshData& meshData,
        glm::vec2 scale)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec2& textureCoordinate = meshData.meshVertexArray[i].textureCoordinate;
            textureCoordinate *= scale;
        }
    }

    void MeshLoader::TranslateTextureCoordinates(
        MeshData& meshData,
        glm::vec2 translation)
    {
        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            glm::vec2& textureCoordinate = meshData.meshVertexArray[i].textureCoordinate;
            textureCoordinate += translation;
        }
    }

    bool MeshLoader::s_triangulate = false;

    // protected ---------------------------------------------------------------

    bool MeshLoader::GetLineFromConstChar(
        const char*& incrementingPtr,
        std::string& currentLine)
    {
        if (incrementingPtr == nullptr || *incrementingPtr == '\0')
        {
            currentLine = "";
            return false; // null ptr or end of line
        }

        const char* startPtr = incrementingPtr;
        const char* endPtr = incrementingPtr;

        while (*endPtr != '\n' && *endPtr != '\0')
        {
            endPtr++;
        }

        size_t lineLength = (size_t)(endPtr - startPtr);

        currentLine = std::string(startPtr, lineLength);

        if (*endPtr == '\n')
        {
            incrementingPtr = endPtr + 1; // Move past newline
            return true;
        }

        incrementingPtr = endPtr; // Move to null terminator
        return false;
    }

    bool MeshLoader::ProcessMeshOBJLine(
        std::string& line,
        MeshData& meshData,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::vec3>& normals,
        bool triangulate)
    {
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (line.empty())
        {
            return true; // continue
        }

        std::string tag;
        std::string values;

        size_t tagStart = line.find_first_not_of(" \t");
        size_t tagEnd = line.find_first_of(" \t", tagStart);

        size_t valuesStart = line.find_first_not_of(" \t", tagEnd);

        if (tagStart != std::string::npos && tagEnd != std::string::npos)
        {
            tag = line.substr(tagStart, tagEnd - tagStart);
        }
        else
        {
            return true; // continue
        }

        if (valuesStart != std::string::npos)
        {
            values = line.substr(valuesStart);
        }
        else
        {
            return true; // continue
        }

        std::vector<std::string> splitValues;

        size_t previousValueEnd = 0;
        while (previousValueEnd != std::string::npos)
        {
            size_t valueStart = values.find_first_not_of(" \t", previousValueEnd);
            size_t valueEnd = values.find_first_of(" \t", valueStart);

            if (valueStart != std::string::npos)
            {
                if (valueEnd != std::string::npos)
                {
                    splitValues.push_back(values.substr(valueStart, valueEnd - valueStart));
                }
                else
                {
                    splitValues.push_back(values.substr(valueStart));
                }
            }

            previousValueEnd = valueEnd;
        }

        if (tag == "v")
        {
            if (splitValues.size() >= 3)
            {
                glm::vec3 newPosition(std::stof(splitValues[0]), std::stof(splitValues[1]), std::stof(splitValues[2]));

                float vertexRadius = glm::length(newPosition);
                if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

                if (maxVertexPosition.x < newPosition.x) maxVertexPosition.x = newPosition.x;
                if (maxVertexPosition.y < newPosition.y) maxVertexPosition.y = newPosition.y;
                if (maxVertexPosition.z < newPosition.z) maxVertexPosition.z = newPosition.z;

                if (minVertexPosition.x > newPosition.x) minVertexPosition.x = newPosition.x;
                if (minVertexPosition.y > newPosition.y) minVertexPosition.y = newPosition.y;
                if (minVertexPosition.z > newPosition.z) minVertexPosition.z = newPosition.z;

                positions.push_back(newPosition);
            }
        }
        else if (tag == "vt")
        {
            if (splitValues.size() >= 2)
            {
                glm::vec2 newTextureCoordinate(std::stof(splitValues[0]), std::stof(splitValues[1]));

                textureCoordinates.push_back(newTextureCoordinate);
            }
        }
        else if (tag == "vn")
        {
            if (splitValues.size() >= 3)
            {
                glm::vec3 newNormal(std::stof(splitValues[0]), std::stof(splitValues[1]), std::stof(splitValues[2]));

                normals.push_back(newNormal);
            }
        }
        else if (tag == "f")
        {
            if (splitValues.size() >= 3)
            {
                // x/y/z
                // positionIndex/textureCoordinateIndex/normalIndex
                std::vector<glm::ivec3> face;

                for (size_t i = 0; i < splitValues.size(); ++i)
                {
                    std::string& indexGroup = splitValues[i];

                    std::vector<size_t> slashIndicies;
                    for (size_t j = 0; j < indexGroup.size(); ++j)
                    {
                        if (indexGroup[j] == '/')
                        {
                            slashIndicies.push_back(j);
                        }
                    }

                    if (slashIndicies.size() == 0)
                    {
                        int positionIndex = std::stoi(indexGroup);

                        face.emplace_back(positionIndex, 0, 0);
                    }
                    else if (slashIndicies.size() == 1)
                    {
                        const size_t& slashIndex = slashIndicies[0];

                        int positionIndex = 0;
                        int textureCoordinateIndex = 0;

                        if (slashIndex > 0)
                        {
                            std::string positionIndexString = indexGroup.substr(0, slashIndex);
                            positionIndex = std::stoi(positionIndexString);
                        }

                        if (indexGroup.size() > slashIndex + 1)
                        {
                            std::string textureCoordinateIndexString = indexGroup.substr(slashIndex + 1);
                            textureCoordinateIndex = std::stoi(textureCoordinateIndexString);
                        }

                        face.emplace_back(positionIndex, textureCoordinateIndex, 0);
                    }
                    else if (slashIndicies.size() == 2)
                    {
                        const size_t& firstSlashIndex = slashIndicies[0];
                        const size_t& secondSlashIndex = slashIndicies[1];

                        int positionIndex = 0;
                        int textureCoordinateIndex = 0;
                        int normalIndex = 0;

                        if (firstSlashIndex > 0)
                        {
                            std::string positionIndexString = indexGroup.substr(0, firstSlashIndex);
                            positionIndex = std::stoi(positionIndexString);
                        }

                        if (secondSlashIndex > firstSlashIndex + 1)
                        {
                            std::string textureCoordinateIndexString = indexGroup.substr(firstSlashIndex + 1, secondSlashIndex - firstSlashIndex - 1);
                            textureCoordinateIndex = std::stoi(textureCoordinateIndexString);
                        }

                        if (indexGroup.size() > secondSlashIndex + 1)
                        {
                            std::string normalIndexString = indexGroup.substr(secondSlashIndex + 1);
                            normalIndex = std::stoi(normalIndexString);
                        }

                        face.emplace_back(positionIndex, textureCoordinateIndex, normalIndex);
                    }
                }

                if (face.size() >= 3)
                {
                    if (!triangulate)
                    {
                        unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                        for (size_t i = 0; i < face.size(); ++i)
                        {
                            const glm::ivec3& faceVertex = face[i];
                            MeshVertex meshVertex;
                            if (!GetMeshVertexFromFaceVertex(meshVertex, faceVertex, positions, textureCoordinates, normals))
                            {
                                return false;
                            }
                            meshVertexArray.push_back(meshVertex);
                        }

                        for (size_t i = 1; i < face.size() - 1; ++i)
                        {
                            meshIndexArray.push_back(currentVertexCount);
                            meshIndexArray.push_back(currentVertexCount + (unsigned int)i);
                            meshIndexArray.push_back(currentVertexCount + (unsigned int)i + 1);
                        }
                    }
                    else
                    {
                        for (size_t i = 1; i < face.size() - 1; ++i)
                        {
                            const glm::ivec3& face0 = face[0];
                            const glm::ivec3& face1 = face[i];
                            const glm::ivec3& face2 = face[i + 1];

                            MeshVertex meshVertex0;
                            if (!GetMeshVertexFromFaceVertex(meshVertex0, face0, positions, textureCoordinates, normals))
                            {
                                return false;
                            }
                            meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                            meshVertexArray.push_back(meshVertex0);

                            MeshVertex meshVertex1;
                            if (!GetMeshVertexFromFaceVertex(meshVertex1, face1, positions, textureCoordinates, normals))
                            {
                                return false;
                            }
                            meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                            meshVertexArray.push_back(meshVertex1);

                            MeshVertex meshVertex2;
                            if (!GetMeshVertexFromFaceVertex(meshVertex2, face2, positions, textureCoordinates, normals))
                            {
                                return false;
                            }
                            meshIndexArray.push_back((unsigned int)meshVertexArray.size());
                            meshVertexArray.push_back(meshVertex2);
                        }
                    }
                }
            }
        }

        return true; // continue
    }

    bool MeshLoader::GetMeshVertexFromFaceVertex(
        MeshVertex& meshVertex,
        const glm::ivec3& faceVertex,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        const std::vector<glm::vec3>& normals)
    {
        bool success = true;

        const int& positionIndex = faceVertex.x;
        const int& textureCoordinateIndex = faceVertex.y;
        const int& normalIndex = faceVertex.z;

        if (positionIndex > 0 && positionIndex <= positions.size())
        {
            int adjustedPositionIndex = positionIndex - 1;
            meshVertex.position = positions[adjustedPositionIndex];
        }
        else if (positionIndex < 0 && positionIndex >= positions.size() * -1)
        {
            int adjustedPositionIndex = (int)(positionIndex + positions.size());
            meshVertex.position = positions[adjustedPositionIndex];
        }
        else
        {
            success = false;
        }

        if (textureCoordinateIndex > 0 && textureCoordinateIndex <= textureCoordinates.size())
        {
            int adjustedTextureCoordinateIndex = textureCoordinateIndex - 1;
            meshVertex.textureCoordinate = textureCoordinates[adjustedTextureCoordinateIndex];
        }
        else if (textureCoordinateIndex < 0 && textureCoordinateIndex >= textureCoordinates.size() * -1)
        {
            int adjustedTextureCoordinateIndex = (int)(textureCoordinateIndex + textureCoordinates.size());
            meshVertex.textureCoordinate = textureCoordinates[adjustedTextureCoordinateIndex];
        }
        // else
        // {
        //     success = false;
        // }

        if (normalIndex > 0 && normalIndex <= normals.size())
        {
            int adjustedNormalIndex = normalIndex - 1;
            meshVertex.normal = normals[adjustedNormalIndex];
        }
        else if (normalIndex < 0 && normalIndex >= normals.size() * -1)
        {
            int adjustedNormalIndex = (int)(normalIndex + normals.size());
            meshVertex.normal = normals[adjustedNormalIndex];
        }
        // else
        // {
        //     success = false;
        // }

        return success;
    }

    glm::vec2 MeshLoader::GetLineLineIntersection2D(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2)
    {
        glm::vec2 intersectionPoint(NAN, NAN);

        float& x = intersectionPoint.x;
        float& y = intersectionPoint.y;
        const float& x1 = point1.x;
        const float& y1 = point1.y;
        const float& x2 = point2.x;
        const float& y2 = point2.y;
        const float& m1 = slope1;
        const float& m2 = slope2;

        // y - y1 = m1 * (x - x1)
        // y - y2 = m2 * (x - x2)
        // 
        // y = m1 * (x - x1) + y1
        // y = m2 * (x - x2) + y2
        // 
        // x = (1 / m1) * (y - y1) + x1
        // x = (1 / m2) * (y - y2) + x2

        if (slope1 == slope2)
        {
            // intersectionPoint.x = NAN;
            // intersectionPoint.y = NAN;
        }
        else if (slope1 == std::numeric_limits<float>::infinity() || slope1 == -std::numeric_limits<float>::infinity())
        {
            x = x1;
            y = m2 * (x1 - x2) + y2;
        }
        else if (slope2 == std::numeric_limits<float>::infinity() || slope2 == -std::numeric_limits<float>::infinity())
        {
            x = x2;
            y = m1 * (x2 - x1) + y1;
        }
        else
        {
            // y = y
            // m1 * (x - x1) + y1 = m2 * (x - x2) + y2
            // m1 * x - m1 * x1 + y1 = m2 * x - m2 * x2 + y2
            // m1 * x - m2 * x = m1 * x1 - y1 - m2 * x2 + y2
            // x * (m1 - m2) = m1 * x1 - y1 - m2 * x2 + y2
            x = (m1 * x1 - y1 - m2 * x2 + y2) / (m1 - m2);
            y = m1 * (x - x1) + y1;
        }

        return intersectionPoint;
    }

    void MeshLoader::PopulateMeshDataWithIndicesAndVerticesFor2DPolygon(
        MeshData& meshData,
        const std::vector<glm::vec2>& vertices,
        const std::vector<size_t>& indices,
        bool triangulate)
    {
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;
        float& maxBoundingRadius = meshData.maxBoundingRadius;
        glm::vec3& maxVertexPosition = meshData.maxVertexPosition;
        glm::vec3& minVertexPosition = meshData.minVertexPosition;

        if (maxVertexPosition.z < 0.0f) maxVertexPosition.z = 0.0f;
        if (minVertexPosition.z > 0.0f) minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        size_t currentVertexCount = meshVertexArray.size();

        if (!triangulate)
        {
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                const glm::vec2& currentCorner = vertices[i];

                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(currentCorner, 0.0f);
                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < indices.size(); ++i)
            {
                const size_t& currentIndex = indices[i];

                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)currentIndex);
            }
        }
        else
        {
            for (size_t i = 0; i < indices.size(); ++i)
            {
                const size_t& currentIndex = indices[i];
                const glm::vec2& currentCorner = vertices[currentIndex];

                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(currentCorner, 0.0f);
                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);

                meshIndexArray.push_back((unsigned int)currentVertexCount + (unsigned int)i);
            }
        }

        for (size_t i = 0; i < vertices.size(); ++i)
        {
            const glm::vec2& currentPosition = vertices[i];
            float vertexRadius = glm::length(currentPosition);
            if (maxBoundingRadius < vertexRadius) maxBoundingRadius = vertexRadius;

            if (maxVertexPosition.x < currentPosition.x) maxVertexPosition.x = currentPosition.x;
            if (maxVertexPosition.y < currentPosition.y) maxVertexPosition.y = currentPosition.y;

            if (minVertexPosition.x > currentPosition.x) minVertexPosition.x = currentPosition.x;
            if (minVertexPosition.y > currentPosition.y) minVertexPosition.y = currentPosition.y;
        }
    }

    void MeshLoader::SubdivideIcosphereTriangleFaces(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions)
    {
        for (size_t i = 0; i < subdivisions; ++i)
        {
            std::vector<glm::uvec3> newTriangleFaces;

            for (size_t j = 0; j < triangleFaces.size(); ++j)
            {
                //       0         ->         0         ->         0
                //     /   \       ->       /   \       ->       /   \
                //    /     \      ->      A     C      ->      A --- C
                //   /       \     ->     /       \     ->     /  \ /  \
                //  1 ------- 2    ->    1 -- B -- 2    ->    1 -- B -- 2
                // 
                // or
                // 
                //  0 ------- 2    ->    0 -- C -- 2    ->    0 -- C -- 2
                //   \       /     ->     \       /     ->     \  / \  /
                //    \     /      ->      A     B      ->      A --- B
                //     \   /       ->       \   /       ->       \   /
                //       1         ->         1         ->         1

                const glm::uvec3& currentTriangleFace = triangleFaces[j];
                const unsigned int& index0 = currentTriangleFace.x;
                const unsigned int& index1 = currentTriangleFace.y;
                const unsigned int& index2 = currentTriangleFace.z;

                const glm::vec3& position0 = positions[index0];
                const glm::vec2& textureCoordinate0 = textureCoordinates[index0];
                const glm::vec3& position1 = positions[index1];
                const glm::vec2& textureCoordinate1 = textureCoordinates[index1];
                const glm::vec3& position2 = positions[index2];
                const glm::vec2& textureCoordinate2 = textureCoordinates[index2];

                glm::vec3 positionA(position0.x + position1.x, position0.y + position1.y, position0.z + position1.z);
                float scaleA = radius / glm::sqrt(positionA.x * positionA.x + positionA.y * positionA.y + positionA.z * positionA.z);
                positionA.x *= scaleA;
                positionA.y *= scaleA;
                positionA.z *= scaleA;
                glm::vec2 textureCoordinateA((textureCoordinate0.x + textureCoordinate1.x) / 2.0f, (textureCoordinate0.y + textureCoordinate1.y) / 2.0f);

                glm::vec3 positionB(position1.x + position2.x, position1.y + position2.y, position1.z + position2.z);
                float scaleB = radius / glm::sqrt(positionB.x * positionB.x + positionB.y * positionB.y + positionB.z * positionB.z);
                positionB.x *= scaleB;
                positionB.y *= scaleB;
                positionB.z *= scaleB;
                glm::vec2 textureCoordinateB((textureCoordinate1.x + textureCoordinate2.x) / 2.0f, (textureCoordinate1.y + textureCoordinate2.y) / 2.0f);

                glm::vec3 positionC(position2.x + position0.x, position2.y + position0.y, position2.z + position0.z);
                float scaleC = radius / glm::sqrt(positionC.x * positionC.x + positionC.y * positionC.y + positionC.z * positionC.z);
                positionC.x *= scaleC;
                positionC.y *= scaleC;
                positionC.z *= scaleC;
                glm::vec2 textureCoordinateC((textureCoordinate2.x + textureCoordinate0.x) / 2.0f, (textureCoordinate2.y + textureCoordinate0.y) / 2.0f);

                const unsigned int indexA = (unsigned int)positions.size();
                const unsigned int indexB = indexA + 1;
                const unsigned int indexC = indexB + 1;

                positions.push_back(positionA);
                textureCoordinates.push_back(textureCoordinateA);
                positions.push_back(positionB);
                textureCoordinates.push_back(textureCoordinateB);
                positions.push_back(positionC);
                textureCoordinates.push_back(textureCoordinateC);

                newTriangleFaces.emplace_back(index0, indexA, indexC);
                newTriangleFaces.emplace_back(indexA, indexB, indexC);
                newTriangleFaces.emplace_back(indexA, index1, indexB);
                newTriangleFaces.emplace_back(indexC, indexB, index2);
            }

            triangleFaces = std::move(newTriangleFaces);
        }
    }

    void MeshLoader::SubdivideIcosphereTriangleFaces_v2(
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<uint32_t>& offsetSeeds,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions)
    {
        for (size_t i = 0; i < subdivisions; ++i)
        {
            std::vector<glm::uvec3> newTriangleFaces;

            for (size_t j = 0; j < triangleFaces.size(); ++j)
            {
                //       0         ->         0         ->         0
                //     /   \       ->       /   \       ->       /   \
                //    /     \      ->      A     C      ->      A --- C
                //   /       \     ->     /       \     ->     /  \ /  \
                //  1 ------- 2    ->    1 -- B -- 2    ->    1 -- B -- 2
                // 
                // or
                // 
                //  0 ------- 2    ->    0 -- C -- 2    ->    0 -- C -- 2
                //   \       /     ->     \       /     ->     \  / \  /
                //    \     /      ->      A     B      ->      A --- B
                //     \   /       ->       \   /       ->       \   /
                //       1         ->         1         ->         1

                const glm::uvec3& currentTriangleFace = triangleFaces[j];
                const unsigned int& index0 = currentTriangleFace.x;
                const unsigned int& index1 = currentTriangleFace.y;
                const unsigned int& index2 = currentTriangleFace.z;

                const glm::vec3& position0 = positions[index0];
                const glm::vec2& textureCoordinate0 = textureCoordinates[index0];
                const glm::vec3& position1 = positions[index1];
                const glm::vec2& textureCoordinate1 = textureCoordinates[index1];
                const glm::vec3& position2 = positions[index2];
                const glm::vec2& textureCoordinate2 = textureCoordinates[index2];

                glm::vec3 positionA(position0.x + position1.x, position0.y + position1.y, position0.z + position1.z);
                float scaleA = radius / glm::sqrt(positionA.x * positionA.x + positionA.y * positionA.y + positionA.z * positionA.z);
                positionA.x *= scaleA;
                positionA.y *= scaleA;
                positionA.z *= scaleA;
                glm::vec2 textureCoordinateA((textureCoordinate0.x + textureCoordinate1.x) / 2.0f, (textureCoordinate0.y + textureCoordinate1.y) / 2.0f);

                glm::vec3 positionB(position1.x + position2.x, position1.y + position2.y, position1.z + position2.z);
                float scaleB = radius / glm::sqrt(positionB.x * positionB.x + positionB.y * positionB.y + positionB.z * positionB.z);
                positionB.x *= scaleB;
                positionB.y *= scaleB;
                positionB.z *= scaleB;
                glm::vec2 textureCoordinateB((textureCoordinate1.x + textureCoordinate2.x) / 2.0f, (textureCoordinate1.y + textureCoordinate2.y) / 2.0f);

                glm::vec3 positionC(position2.x + position0.x, position2.y + position0.y, position2.z + position0.z);
                float scaleC = radius / glm::sqrt(positionC.x * positionC.x + positionC.y * positionC.y + positionC.z * positionC.z);
                positionC.x *= scaleC;
                positionC.y *= scaleC;
                positionC.z *= scaleC;
                glm::vec2 textureCoordinateC((textureCoordinate2.x + textureCoordinate0.x) / 2.0f, (textureCoordinate2.y + textureCoordinate0.y) / 2.0f);

                // Positions that are the same as another positions will get
                // the same offset seed

                bool positionA_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionA.x, currentPosition.x) &&
                        FloatEqualToFloat(positionA.y, currentPosition.y) &&
                        FloatEqualToFloat(positionA.z, currentPosition.z))
                    {
                        positionA_is_unique = false;
                        offsetSeeds.push_back(offsetSeeds[i]);
                        break;
                    }
                }
                if (positionA_is_unique)
                {
                    offsetSeeds.push_back(offsetSeeds.size());
                }

                bool positionB_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionB.x, currentPosition.x) &&
                        FloatEqualToFloat(positionB.y, currentPosition.y) &&
                        FloatEqualToFloat(positionB.z, currentPosition.z))
                    {
                        positionB_is_unique = false;
                        offsetSeeds.push_back(offsetSeeds[i]);
                        break;
                    }
                }
                if (positionB_is_unique)
                {
                    offsetSeeds.push_back(offsetSeeds.size());
                }

                bool positionC_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionC.x, currentPosition.x) &&
                        FloatEqualToFloat(positionC.y, currentPosition.y) &&
                        FloatEqualToFloat(positionC.z, currentPosition.z))
                    {
                        positionC_is_unique = false;
                        offsetSeeds.push_back(offsetSeeds[i]);
                        break;
                    }
                }
                if (positionC_is_unique)
                {
                    offsetSeeds.push_back(offsetSeeds.size());
                }

                const unsigned int indexA = (unsigned int)positions.size();
                const unsigned int indexB = indexA + 1;
                const unsigned int indexC = indexB + 1;

                positions.push_back(positionA);
                textureCoordinates.push_back(textureCoordinateA);
                positions.push_back(positionB);
                textureCoordinates.push_back(textureCoordinateB);
                positions.push_back(positionC);
                textureCoordinates.push_back(textureCoordinateC);

                newTriangleFaces.emplace_back(index0, indexA, indexC);
                newTriangleFaces.emplace_back(indexA, indexB, indexC);
                newTriangleFaces.emplace_back(indexA, index1, indexB);
                newTriangleFaces.emplace_back(indexC, indexB, index2);
            }

            triangleFaces = std::move(newTriangleFaces);
        }
    }

    void MeshLoader::SubdivideIcosphereTriangleFaces_v3(
        std::vector<glm::vec3>& positions,
        std::vector<glm::uvec3>& triangleFaces,
        float radius,
        size_t subdivisions)
    {
        for (size_t i = 0; i < subdivisions; ++i)
        {
            std::vector<glm::uvec3> newTriangleFaces;

            for (size_t j = 0; j < triangleFaces.size(); ++j)
            {
                //       0         ->         0         ->         0
                //     /   \       ->       /   \       ->       /   \
                //    /     \      ->      A     C      ->      A --- C
                //   /       \     ->     /       \     ->     /  \ /  \
                //  1 ------- 2    ->    1 -- B -- 2    ->    1 -- B -- 2
                // 
                // or
                // 
                //  0 ------- 2    ->    0 -- C -- 2    ->    0 -- C -- 2
                //   \       /     ->     \       /     ->     \  / \  /
                //    \     /      ->      A     B      ->      A --- B
                //     \   /       ->       \   /       ->       \   /
                //       1         ->         1         ->         1

                const glm::uvec3& currentTriangleFace = triangleFaces[j];
                const unsigned int& index0 = currentTriangleFace.x;
                const unsigned int& index1 = currentTriangleFace.y;
                const unsigned int& index2 = currentTriangleFace.z;

                const glm::vec3& position0 = positions[index0];
                const glm::vec3& position1 = positions[index1];
                const glm::vec3& position2 = positions[index2];

                glm::vec3 positionA(position0.x + position1.x, position0.y + position1.y, position0.z + position1.z);
                float scaleA = radius / glm::sqrt(positionA.x * positionA.x + positionA.y * positionA.y + positionA.z * positionA.z);
                positionA.x *= scaleA;
                positionA.y *= scaleA;
                positionA.z *= scaleA;

                glm::vec3 positionB(position1.x + position2.x, position1.y + position2.y, position1.z + position2.z);
                float scaleB = radius / glm::sqrt(positionB.x * positionB.x + positionB.y * positionB.y + positionB.z * positionB.z);
                positionB.x *= scaleB;
                positionB.y *= scaleB;
                positionB.z *= scaleB;

                glm::vec3 positionC(position2.x + position0.x, position2.y + position0.y, position2.z + position0.z);
                float scaleC = radius / glm::sqrt(positionC.x * positionC.x + positionC.y * positionC.y + positionC.z * positionC.z);
                positionC.x *= scaleC;
                positionC.y *= scaleC;
                positionC.z *= scaleC;

                unsigned int indexA;
                bool positionA_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionA.x, currentPosition.x) &&
                        FloatEqualToFloat(positionA.y, currentPosition.y) &&
                        FloatEqualToFloat(positionA.z, currentPosition.z))
                    {
                        indexA = i;
                        positionA_is_unique = false;
                        break;
                    }
                }

                if (positionA_is_unique)
                {
                    indexA = (unsigned int)positions.size();
                    positions.push_back(positionA);
                }

                unsigned int indexB;
                bool positionB_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionB.x, currentPosition.x) &&
                        FloatEqualToFloat(positionB.y, currentPosition.y) &&
                        FloatEqualToFloat(positionB.z, currentPosition.z))
                    {
                        indexB = i;
                        positionB_is_unique = false;
                        break;
                    }
                }

                if (positionB_is_unique)
                {
                    indexB = (unsigned int)positions.size();
                    positions.push_back(positionB);
                }

                unsigned int indexC;
                bool positionC_is_unique = true;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    const glm::vec3 currentPosition = positions[i];
                    if (FloatEqualToFloat(positionC.x, currentPosition.x) &&
                        FloatEqualToFloat(positionC.y, currentPosition.y) &&
                        FloatEqualToFloat(positionC.z, currentPosition.z))
                    {
                        indexC = i;
                        positionC_is_unique = false;
                        break;
                    }
                }

                if (positionC_is_unique)
                {
                    indexC = (unsigned int)positions.size();
                    positions.push_back(positionC);
                }

                newTriangleFaces.emplace_back(index0, indexA, indexC);
                newTriangleFaces.emplace_back(indexA, indexB, indexC);
                newTriangleFaces.emplace_back(indexA, index1, indexB);
                newTriangleFaces.emplace_back(indexC, indexB, index2);
            }

            triangleFaces = std::move(newTriangleFaces);
        }
    }

    void MeshLoader::GenerateIcosphereMeshVerticesAndIndices(
        MeshData& meshData,
        float radius,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::uvec3>& triangleFaces,
        bool smoothNormals,
        bool triangulate)
    {
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;

        size_t initialVertexCount = meshVertexArray.size();

        if (smoothNormals)
        {
            if (triangulate)
            {
                std::vector<MeshVertex> tempMeshVertexArray;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.textureCoordinate = textureCoordinates[i];
                    meshVertex.normal = positions[i] / radius;
                    tempMeshVertexArray.push_back(meshVertex);
                }

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                for (size_t i = 0; i < triangleFaces.size(); ++i)
                {
                    const glm::uvec3& currentTriangleFace = triangleFaces[i];
                    const unsigned int& index0 = currentTriangleFace.x;
                    const unsigned int& index1 = currentTriangleFace.y;
                    const unsigned int& index2 = currentTriangleFace.z;

                    meshVertexArray.push_back(tempMeshVertexArray[index0]);
                    meshVertexArray.push_back(tempMeshVertexArray[index1]);
                    meshVertexArray.push_back(tempMeshVertexArray[index2]);
                }

                for (size_t i = 0; i < triangleFaces.size() * 3; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.textureCoordinate = textureCoordinates[i];
                    meshVertex.normal = positions[i] / radius;
                    meshVertexArray.push_back(meshVertex);
                }

                for (size_t i = 0; i < triangleFaces.size(); ++i)
                {
                    const glm::uvec3& currentTriangleFace = triangleFaces[i];
                    meshIndexArray.push_back((unsigned int)initialVertexCount + currentTriangleFace.x);
                    meshIndexArray.push_back((unsigned int)initialVertexCount + currentTriangleFace.y);
                    meshIndexArray.push_back((unsigned int)initialVertexCount + currentTriangleFace.z);
                }
            }
        }
        else
        {
            unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

            for (size_t i = 0; i < triangleFaces.size(); ++i)
            {
                const glm::uvec3& currentTriangleFace = triangleFaces[i];
                const unsigned int& index0 = currentTriangleFace.x;
                const unsigned int& index1 = currentTriangleFace.y;
                const unsigned int& index2 = currentTriangleFace.z;
                const glm::vec3& position0 = positions[index0];
                const glm::vec3& position1 = positions[index1];
                const glm::vec3& position2 = positions[index2];

                glm::vec3 position01 = position1 - position0;
                glm::vec3 position12 = position2 - position1;
                glm::vec3 normal = glm::normalize(glm::cross(position01, position12));

                MeshVertex meshVertex0;
                meshVertex0.position = position0;
                meshVertex0.textureCoordinate = textureCoordinates[index0];
                meshVertex0.normal = normal;

                MeshVertex meshVertex1;
                meshVertex1.position = position1;
                meshVertex1.textureCoordinate = textureCoordinates[index1];
                meshVertex1.normal = normal;

                MeshVertex meshVertex2;
                meshVertex2.position = position2;
                meshVertex2.textureCoordinate = textureCoordinates[index2];
                meshVertex2.normal = normal;

                meshVertexArray.push_back(meshVertex0);
                meshVertexArray.push_back(meshVertex1);
                meshVertexArray.push_back(meshVertex2);
            }

            for (size_t i = 0; i < triangleFaces.size() * 3; ++i)
            {
                meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
            }
        }
    }

    void MeshLoader::GenerateSphereMeshVerticesAndIndices(
        MeshData& meshData,
        size_t longitudinalSections,
        size_t latitudinalSections,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec2>& textureCoordinates,
        std::vector<glm::vec3>& normals,
        bool smoothNormals,
        bool triangulate)
    {
        std::vector<MeshVertex>& meshVertexArray = meshData.meshVertexArray;
        std::vector<unsigned int>& meshIndexArray = meshData.meshIndexArray;

        size_t initialVertexCount = meshVertexArray.size();

        // ex: longitudinalSections = 3, latitudinalSections = 4
        // indicies:
        // 
        //     ( 0 )   ( 1 )   ( 2 )     --------
        //      / \     / \     / \       top
        //     /   \   /   \   /   \
        // ( 3 )---( 4 )---( 5 )---( 6 ) --------
        //   |       |       |       |    body
        //   |       |       |       |
        // ( 7 )---( 8 )---( 9 )---( 10)
        //   |       |       |       |
        //   |       |       |       |
        // ( 11)---( 12)---( 13)---( 14) --------
        //     \   /   \   /   \   /      bottom
        //      \ /     \ /     \ /
        //     ( 15)   ( 16)   ( 17)     --------
        // 
        // firstBodyPosition = 3
        // firstBottomPosition = 11

        const size_t firstBodyPosition = longitudinalSections;
        const size_t firstBottomPosition = (longitudinalSections + 1) * (latitudinalSections - 1) - 1;
        const size_t bodyWrapVertexCount = longitudinalSections + 1;

        if (smoothNormals)
        {
            if (triangulate)
            {
                std::vector<MeshVertex> tempMeshVertexArray;
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.textureCoordinate = textureCoordinates[i];
                    meshVertex.normal = normals[i];
                    tempMeshVertexArray.push_back(meshVertex);
                }

                // top
                //     ( i )
                //      / \
                //     /   \
                // ( i2)---( i3)
                for (size_t i = 0; i < firstBodyPosition; ++i)
                {
                    size_t i2 = i + longitudinalSections;
                    size_t i3 = i2 + 1;

                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i3]);
                }

                // body
                // ( i )---( i3)
                //   |    /  |
                //   |  /    |
                // ( i2)---( i4)
                if (latitudinalSections > 2)
                {
                    for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                    {
                        if ((i + 2) % bodyWrapVertexCount == 0)
                        {
                            continue; // skip wrap section
                        }

                        size_t i2 = i + bodyWrapVertexCount;
                        size_t i3 = i + 1;
                        size_t i4 = i2 + 1;

                        meshVertexArray.push_back(tempMeshVertexArray[i]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2]);
                        meshVertexArray.push_back(tempMeshVertexArray[i3]);

                        meshVertexArray.push_back(tempMeshVertexArray[i4]);
                        meshVertexArray.push_back(tempMeshVertexArray[i3]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    }
                }

                // bottom
                // ( i )---( i3)
                //     \   /
                //      \ /
                //     ( i2)
                for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
                {
                    size_t i2 = i + bodyWrapVertexCount;
                    size_t i3 = i + 1;

                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i3]);
                }

                for (size_t i = 0; i < (latitudinalSections - 1) * longitudinalSections * 6; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                }
            }
            else
            {
                for (size_t i = 0; i < positions.size(); ++i)
                {
                    MeshVertex meshVertex;
                    meshVertex.position = positions[i];
                    meshVertex.textureCoordinate = textureCoordinates[i];
                    meshVertex.normal = normals[i];
                    meshVertexArray.push_back(meshVertex);
                }

                // top
                //     ( i )
                //      / \
                //     /   \
                // ( i2)---( i3)
                for (size_t i = 0; i < firstBodyPosition; ++i)
                {
                    size_t i2 = i + longitudinalSections;
                    size_t i3 = i2 + 1;

                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                }

                // body
                // ( i )---( i3)
                //   |    /  |
                //   |  /    |
                // ( i2)---( i4)
                if (latitudinalSections > 2)
                {
                    for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                    {
                        if ((i + 2) % bodyWrapVertexCount == 0)
                        {
                            continue; // skip wrap section
                        }

                        size_t i2 = i + bodyWrapVertexCount;
                        size_t i3 = i + 1;
                        size_t i4 = i2 + 1;

                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i4));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                        meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                    }
                }

                // bottom
                // ( i )---( i3)
                //     \   /
                //      \ /
                //     ( i2)
                for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
                {
                    size_t i2 = i + bodyWrapVertexCount;
                    size_t i3 = i + 1;

                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i2));
                    meshIndexArray.push_back((unsigned int)(initialVertexCount + i3));
                }
            }
        }
        else
        {
            // top
            //     ( i )
            //      / \
            //     /   \
            // ( i2)---( i3)
            for (size_t i = 0; i < firstBodyPosition; ++i)
            {
                size_t i2 = i + longitudinalSections;
                size_t i3 = i2 + 1;

                const glm::vec3& p1 = positions[i];
                const glm::vec3& p2 = positions[i2];
                const glm::vec3& p3 = positions[i3];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.textureCoordinate = textureCoordinates[i];
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.textureCoordinate = textureCoordinates[i2];
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.textureCoordinate = textureCoordinates[i3];
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }

            // body
            // ( i )---( i3)
            //   |    /  |
            //   |  /    |
            // ( i2)---( i4)
            if (latitudinalSections > 2)
            {
                for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                {
                    if ((i + 2) % bodyWrapVertexCount == 0)
                    {
                        continue; // skip wrap section
                    }

                    size_t i2 = i + bodyWrapVertexCount;
                    size_t i3 = i + 1;
                    size_t i4 = i2 + 1;

                    const glm::vec3& p1 = positions[i];
                    const glm::vec3& p2 = positions[i2];
                    const glm::vec3& p3 = positions[i3];

                    glm::vec3 p1_to_p2 = p2 - p1;
                    glm::vec3 p2_to_p3 = p3 - p2;
                    glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                    MeshVertex v1;
                    v1.position = p1;
                    v1.textureCoordinate = textureCoordinates[i];
                    v1.normal = normal123;

                    MeshVertex v2;
                    v2.position = p2;
                    v2.textureCoordinate = textureCoordinates[i2];
                    v2.normal = normal123;

                    MeshVertex v3;
                    v3.position = p3;
                    v3.textureCoordinate = textureCoordinates[i3];
                    v3.normal = normal123;

                    MeshVertex v4;
                    v4.position = positions[i4];
                    v4.textureCoordinate = textureCoordinates[i4];
                    v4.normal = normal123;

                    unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();
                    if (triangulate)
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v2);

                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                        meshIndexArray.push_back(currentVertexCount++);
                    }
                    else
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);

                        meshIndexArray.push_back(currentVertexCount);
                        meshIndexArray.push_back(currentVertexCount + 1);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 3);
                        meshIndexArray.push_back(currentVertexCount + 2);
                        meshIndexArray.push_back(currentVertexCount + 1);
                    }
                }
            }

            // bottom
            // ( i )---( i3)
            //     \   /
            //      \ /
            //     ( i2)
            for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
            {
                size_t i2 = i + bodyWrapVertexCount;
                size_t i3 = i + 1;

                const glm::vec3& p1 = positions[i];
                const glm::vec3& p2 = positions[i2];
                const glm::vec3& p3 = positions[i3];

                glm::vec3 p1_to_p2 = p2 - p1;
                glm::vec3 p2_to_p3 = p3 - p2;
                glm::vec3 normal123 = glm::normalize(glm::cross(p1_to_p2, p2_to_p3));

                MeshVertex v1;
                v1.position = p1;
                v1.textureCoordinate = textureCoordinates[i];
                v1.normal = normal123;

                MeshVertex v2;
                v2.position = p2;
                v2.textureCoordinate = textureCoordinates[i2];
                v2.normal = normal123;

                MeshVertex v3;
                v3.position = p3;
                v3.textureCoordinate = textureCoordinates[i3];
                v3.normal = normal123;

                unsigned int currentVertexCount = (unsigned int)meshVertexArray.size();

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(currentVertexCount);
                meshIndexArray.push_back(currentVertexCount + 1);
                meshIndexArray.push_back(currentVertexCount + 2);
            }
        }
    }

    void MeshLoader::RecalculateMeshMinMax_v2(MeshData& meshData)
    {
        if (meshData.meshVertexArray.size() > 0)
        {
            const MeshVertex& currentMeshVertex = meshData.meshVertexArray[0];

            meshData.maxBoundingRadius = 0;
            meshData.maxVertexPosition.x = currentMeshVertex.position.x;
            meshData.maxVertexPosition.y = currentMeshVertex.position.y;
            meshData.maxVertexPosition.z = currentMeshVertex.position.z;
            meshData.minVertexPosition.x = currentMeshVertex.position.x;
            meshData.minVertexPosition.y = currentMeshVertex.position.y;
            meshData.minVertexPosition.z = currentMeshVertex.position.z;
        }

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            const glm::vec3& currentPosition = meshData.meshVertexArray[i].position;
            float vertexRadius = glm::length(currentPosition);
            if (meshData.maxBoundingRadius < vertexRadius) meshData.maxBoundingRadius = vertexRadius;

            if (meshData.maxVertexPosition.x < currentPosition.x) meshData.maxVertexPosition.x = currentPosition.x;
            if (meshData.maxVertexPosition.y < currentPosition.y) meshData.maxVertexPosition.y = currentPosition.y;
            if (meshData.maxVertexPosition.z < currentPosition.z) meshData.maxVertexPosition.z = currentPosition.z;

            if (meshData.minVertexPosition.x > currentPosition.x) meshData.minVertexPosition.x = currentPosition.x;
            if (meshData.minVertexPosition.y > currentPosition.y) meshData.minVertexPosition.y = currentPosition.y;
            if (meshData.minVertexPosition.z > currentPosition.z) meshData.minVertexPosition.z = currentPosition.z;
        }
    }
}