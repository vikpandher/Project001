#include "MeshStores.h"

#include <algorithm>
#include <fstream>

#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorAngles.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    MeshStores::MeshStores()
    {}

    MeshStores::~MeshStores()
    {}

    void MeshStores::ClearMeshes()
    {
        meshVertexArray_.clear();
        meshIndexArray_.clear();
        meshDataArray_.clear();
    }

    bool MeshStores::GetMesh(
        unsigned int index,
        const MeshVertex*& firstVertexPtr,
        unsigned int& vertexCount,
        const unsigned int*& firstIndexPtr,
        unsigned int& indexCount) const
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        const MeshData& meshData = meshDataArray_[index];

        firstVertexPtr = &meshVertexArray_[meshData.vertexIndex];
        vertexCount = meshData.vertexCount;

        firstIndexPtr = &meshIndexArray_[meshData.indexIndex];
        indexCount = meshData.indexCount;

        return true;
    }

    bool MeshStores::GetMeshData(
        unsigned int index,
        MeshData& meshData) const
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        meshData = meshDataArray_[index];

        return true;
    }

    bool MeshStores::LoadMeshOBJ(
        unsigned int& index,
        const std::string& path,
        bool normalizeSize,
        bool recenter,
        bool triangulate)
    {
        // Should the file path be required to have the .obj extension?
        // if (path.size() < 4 || path.substr(path.size() - 4, 4) != ".obj")
        // {
        //     return false;
        // }

        MeshData newMeshData;
        if (LoadMeshOBJ(newMeshData, meshVertexArray_, meshIndexArray_, path, normalizeSize, recenter, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::LoadTriangleMesh(
        unsigned int& index,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::ivec2>& faces,
        bool normalizeSize,
        bool recenter)
    {
        MeshData newMeshData;
        if (LoadTriangleMesh(newMeshData, meshVertexArray_, meshIndexArray_, positions, normals, faces, normalizeSize, recenter))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangleFan(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DTriangleFan(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangles(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter)
    {
        MeshData newMeshData;
        if (Generate2DTriangles(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangleStrip(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DTriangleStrip(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DArc(
        unsigned int& index,
        float innerRadius,
        float outerRadius,
        size_t subdivisions,
        float startAngle,
        float endAngle,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DArc(newMeshData, meshVertexArray_, meshIndexArray_, innerRadius, outerRadius, subdivisions, startAngle, endAngle, recenter, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DCapsule(
        unsigned int& index,
        float rectangleHeight,
        float capsuleWidth,
        size_t radialSections,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DCapsule(newMeshData, meshVertexArray_, meshIndexArray_, rectangleHeight, capsuleWidth, radialSections, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DLine(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DLine(newMeshData, meshVertexArray_, meshIndexArray_, positions, width, beveledCorners, recenter, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DLineLoop(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DLineLoop(newMeshData, meshVertexArray_, meshIndexArray_, positions, width, beveledCorners, recenter, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DRegularPolygon(
        unsigned int& index,
        float radius,
        size_t sides,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DRegularPolygon(newMeshData, meshVertexArray_, meshIndexArray_, radius, sides, recenter, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DBezeledRectangle(
        unsigned int& index,
        float width,
        float height,
        float bezelSize,
        float bezelSections,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DBezeledRectangle(newMeshData, meshVertexArray_, meshIndexArray_, width, height, bezelSize, bezelSections, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DSprite(
        unsigned int& index,
        float width,
        float height,
        float textureBottom,
        float textureTop,
        float textureLeft,
        float textureRight,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DSprite(newMeshData, meshVertexArray_, meshIndexArray_, width, height, textureBottom, textureTop, textureLeft, textureRight, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateBox(
        unsigned int& index,
        float xLength,
        float yLength,
        float zLength,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateBox(newMeshData, meshVertexArray_, meshIndexArray_, xLength, yLength, zLength, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateCapsule(
        unsigned int& index,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateCapsule(newMeshData, meshVertexArray_, meshIndexArray_, cylindricalHeight, radius, faces, stacks, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateCone(
        unsigned int& index,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateCone(newMeshData, meshVertexArray_, meshIndexArray_, height, radius, faces, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateCylinder(
        unsigned int& index,
        float height,
        float radius,
        size_t faces,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateCylinder(newMeshData, meshVertexArray_, meshIndexArray_, height, radius, faces, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateIcosphere(
        unsigned int& index,
        float radius,
        size_t subdivisions,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateIcosphere(newMeshData, meshVertexArray_, meshIndexArray_, radius, subdivisions, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::GenerateSphere(
        unsigned int& index,
        float radius,
        size_t longitudinalSections,
        size_t latitudinalSections,
        bool smoothNormals,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateSphere(newMeshData, meshVertexArray_, meshIndexArray_, radius, longitudinalSections, latitudinalSections, smoothNormals, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (unsigned int)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::NormalizeMeshSize(unsigned int& index)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        NormalizeMeshSize(meshData, meshVertexArray_);

        return true;
    }

    bool MeshStores::RecenterMesh(unsigned int& index)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RecenterMesh(meshData, meshVertexArray_);

        return true;
    }

    bool MeshStores::TranslateMesh(unsigned int& index, glm::vec3 translation)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        TranslateMesh(meshData, meshVertexArray_, translation);

        return true;
    }

    bool MeshStores::RotateMesh(unsigned int& index, glm::quat rotation)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RotateMesh(meshData, meshVertexArray_, rotation);

        return true;
    }

    bool MeshStores::RotateMeshX(unsigned int& index, float rotationInRadians)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RotateMeshX(meshData, meshVertexArray_, rotationInRadians);

        return true;
    }

    bool MeshStores::RotateMeshY(unsigned int& index, float rotationInRadians)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RotateMeshY(meshData, meshVertexArray_, rotationInRadians);

        return true;
    }

    bool MeshStores::RotateMeshZ(unsigned int& index, float rotationInRadians)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RotateMeshZ(meshData, meshVertexArray_, rotationInRadians);

        return true;
    }

    bool MeshStores::ScaleMesh(unsigned int& index, glm::vec3 scale)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        ScaleMesh(meshData, meshVertexArray_, scale);

        return true;
    }

    bool MeshStores::SizeMeshAlongNormals(unsigned int& index, float size)
    {
        if (index >= meshIndexArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        SizeMeshAlongNormals(meshData, meshVertexArray_, size);

        return true;
    }

    bool MeshStores::TurnInsideOut(unsigned int& index, bool wasTriangulated)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        TurnInsideOut(meshData, meshVertexArray_, meshIndexArray_, wasTriangulated);

        return true;
    }

    bool MeshStores::LoadMeshOBJ(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::string& path,
        bool normalizeSize,
        bool recenter,
        bool triangulate)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;

        std::string currentLine;
        while (std::getline(file, currentLine))
        {
            if (currentLine.empty())
            {
                continue;
            }

            std::string tag;
            std::string values;

            size_t tagStart = currentLine.find_first_not_of(" \t");
            size_t tagEnd = currentLine.find_first_of(" \t", tagStart);

            size_t valuesStart = currentLine.find_first_not_of(" \t", tagEnd);

            if (tagStart != std::string::npos && tagEnd != std::string::npos)
            {
                tag = currentLine.substr(tagStart, tagEnd - tagStart);
            }
            else
            {
                continue;
            }

            if (valuesStart != std::string::npos)
            {
                values = currentLine.substr(valuesStart);
            }
            else
            {
                continue;
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
                    glm::vec3 newPosition;
                    newPosition.x = std::stof(splitValues[0]);
                    newPosition.y = std::stof(splitValues[1]);
                    newPosition.z = std::stof(splitValues[2]);

                    meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, newPosition.x);
                    meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, newPosition.y);
                    meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, newPosition.z);

                    meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, newPosition.x);
                    meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, newPosition.y);
                    meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, newPosition.z);

                    positions.push_back(newPosition);
                }
            }
            else if (tag == "vt")
            {
                if (splitValues.size() >= 2)
                {
                    glm::vec2 newTextureCoordinate;
                    newTextureCoordinate.x = std::stof(splitValues[0]);
                    newTextureCoordinate.y = std::stof(splitValues[1]);

                    textureCoordinates.push_back(newTextureCoordinate);
                }
            }
            else if (tag == "vn")
            {
                if (splitValues.size() >= 3)
                {
                    glm::vec3 newNormal;
                    newNormal.x = std::stof(splitValues[0]);
                    newNormal.y = std::stof(splitValues[1]);
                    newNormal.z = std::stof(splitValues[2]);

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
                                meshIndexArray.push_back(meshData.vertexCount);
                                meshIndexArray.push_back(meshData.vertexCount + (unsigned int)i);
                                meshIndexArray.push_back(meshData.vertexCount + (unsigned int)i + 1);
                            }

                            meshData.vertexCount += (unsigned int)face.size();
                            meshData.indexCount += (unsigned int)((face.size() - 2) * 3);
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
                                meshVertexArray.push_back(meshVertex0);
                                meshIndexArray.push_back(meshData.indexCount++);

                                MeshVertex meshVertex1;
                                if (!GetMeshVertexFromFaceVertex(meshVertex1, face1, positions, textureCoordinates, normals))
                                {
                                    return false;
                                }
                                meshVertexArray.push_back(meshVertex1);
                                meshIndexArray.push_back(meshData.indexCount++);

                                MeshVertex meshVertex2;
                                if (!GetMeshVertexFromFaceVertex(meshVertex2, face2, positions, textureCoordinates, normals))
                                {
                                    return false;
                                }
                                meshVertexArray.push_back(meshVertex2);
                                meshIndexArray.push_back(meshData.indexCount++);
                            }

                            meshData.vertexCount += (unsigned int)((face.size() - 2) * 3);
                        }
                    }
                }
            }
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        if (normalizeSize)
        {
            NormalizeMeshSize(meshData, meshVertexArray);
        }

        if (meshData.vertexCount == 0 && meshData.indexCount == 0)
        {
            return false;
        }

        return true;
    }

    bool MeshStores::LoadTriangleMesh(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        std::vector<glm::vec3>& positions,
        std::vector<glm::vec3>& normals,
        std::vector<glm::ivec2>& faces,
        bool normalizeSize,
        bool recenter)
    {
        if (faces.empty())
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();

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

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back(meshData.indexCount++);
        }

        meshData.vertexCount = (unsigned int)faces.size();

        for (size_t i = 0; i < positions.size(); ++i)
        {
            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, positions[i].x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, positions[i].y);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, positions[i].z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, positions[i].x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, positions[i].y);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, positions[i].z);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        if (normalizeSize)
        {
            NormalizeMeshSize(meshData, meshVertexArray);
        }

        if (meshData.vertexCount == 0 && meshData.indexCount == 0)
        {
            return false;
        }

        return true;
    }

    bool MeshStores::Generate2DTriangleFan(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (size_t i = 0; i < positions.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertex.textureCoordinate = textureCoordinates[i];
                }

                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 1; i < positions.size() - 1; ++i)
            {
                meshIndexArray.push_back(0);
                meshIndexArray.push_back((unsigned int)i);
                meshIndexArray.push_back((unsigned int)i + 1);
            }

            meshData.vertexCount = (unsigned int)positions.size();
            meshData.indexCount += (unsigned int)((positions.size() - 2) * 3);
        }
        else
        {
            for (size_t i = 1; i < positions.size() - 1; ++i)
            {
                MeshVertex centerMeshVertex;
                centerMeshVertex.position = glm::vec3(positions[0], 0.0f);

                if (0 < textureCoordinates.size())
                {
                    centerMeshVertex.textureCoordinate = textureCoordinates[0];
                }

                centerMeshVertex.normal = normal;

                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertexA.textureCoordinate = textureCoordinates[i];
                }

                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);

                if (i + 1 < textureCoordinates.size())
                {
                    meshVertexB.textureCoordinate = textureCoordinates[i + 1];
                }

                meshVertexB.normal = normal;

                meshVertexArray.push_back(centerMeshVertex);
                meshVertexArray.push_back(meshVertexA);
                meshVertexArray.push_back(meshVertexB);

                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
            }

            meshData.vertexCount += (unsigned int)((positions.size() - 2) * 3);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, positions[i].x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, positions[i].y);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, positions[i].x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, positions[i].y);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        return true;
    }

    bool MeshStores::Generate2DTriangles(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter)
    {
        if (positions.size() == 0 || positions.size() % 3 != 0)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = (unsigned int)positions.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = (unsigned int)positions.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        for (size_t i = 0; i < positions.size(); ++i)
        {
            MeshVertex meshVertex;
            meshVertex.position = glm::vec3(positions[i], 0.0f);

            if (i < textureCoordinates.size())
            {
                meshVertex.textureCoordinate = textureCoordinates[i];
            }

            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((unsigned int)i);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, positions[i].x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, positions[i].y);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, positions[i].x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, positions[i].y);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        return true;
    }

    bool MeshStores::Generate2DTriangleStrip(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (size_t i = 0; i < positions.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertex.textureCoordinate = textureCoordinates[i];
                }

                meshVertex.normal = normal;

                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                if (i % 2 == 0)
                {
                    meshIndexArray.push_back((unsigned int)i);
                    meshIndexArray.push_back((unsigned int)i + 1);
                    meshIndexArray.push_back((unsigned int)i + 2);
                }
                else
                {
                    meshIndexArray.push_back((unsigned int)i + 2);
                    meshIndexArray.push_back((unsigned int)i + 1);
                    meshIndexArray.push_back((unsigned int)i);
                }
            }

            meshData.vertexCount = (unsigned int)positions.size();
            meshData.indexCount += (unsigned int)((positions.size() - 2) * 3);
        }
        else
        {
            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertexA.textureCoordinate = textureCoordinates[i];
                }

                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);

                if (i + 1 < textureCoordinates.size())
                {
                    meshVertexB.textureCoordinate = textureCoordinates[i + 1];
                }

                meshVertexB.normal = normal;

                MeshVertex meshVertexC;
                meshVertexC.position = glm::vec3(positions[i + 2], 0.0f);

                if (i + 2 < textureCoordinates.size())
                {
                    meshVertexC.textureCoordinate = textureCoordinates[i + 2];
                }

                meshVertexC.normal = normal;

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

                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
            }

            meshData.vertexCount += (unsigned int)((positions.size() - 2) * 3);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, positions[i].x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, positions[i].y);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, positions[i].x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, positions[i].y);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        return true;
    }

    bool MeshStores::Generate2DArc(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float innerRadius,
        float outerRadius,
        size_t subdivisions,
        float startAngle,
        float endAngle,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
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

        bool success = false;
        if (positionalTexture)
        {
            std::vector<glm::vec2> textureCoordinates;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                textureCoordinates.emplace_back(positions[i].x + 0.5f, positions[i].y + 0.5f);
            }

            success = Generate2DTriangleStrip(meshData, meshVertexArray, meshIndexArray, positions, textureCoordinates, recenter, triangulate);
        }
        else
        {
            success = Generate2DTriangleStrip(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), recenter, triangulate);
        }

        return success;
    }

    bool MeshStores::Generate2DCapsule(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
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
            float x = radius * std::cosf(stackAngle);
            float y = radius * std::sinf(stackAngle);

            positions.emplace_back(x, y + halfHeight);
        }

        for (size_t i = 0; i < radialSections + 1; ++i)
        {
            float stackAngle = glm::pi<float>() + radialStep * (float)i;
            float x = radius * std::cosf(stackAngle);
            float y = radius * std::sinf(stackAngle);

            positions.emplace_back(x, y - halfHeight);
        }

        return Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), false, triangulate);
    }

    bool MeshStores::Generate2DLine(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 position1 = positions[0];
        glm::vec2 position2 = positions[startIndex];

        glm::vec2 direction1 = position2 - position1;

        glm::vec2 offset01(-1.0f * direction1.y, direction1.x);
        float magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
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

            if (slope1 != slope2)
            {
                glm::vec2 offset02 = glm::vec2(-1.0f * direction2.y, direction2.x);
                float magnitude02 = std::sqrtf(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                glm::vec2 positionE = position2 + scaled2;
                glm::vec2 positionF = position2 - scaled2;

                bool sharpCorner = false;
                float angle = Get2DVectorAngle(direction1, direction2);
                if (beveledCorners)
                {
                    if (angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                    {
                        sharpCorner = true;
                    }
                }

                if (sharpCorner)
                {
                    offset01 = glm::vec2(-1.0f * direction1.y, direction1.x);
                    magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        positionC = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
                        positionD = position2 - scaled1;
                    }
                    else
                    {
                        positionC = position2 + scaled1;
                        positionD = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
                    }
                }
                else
                {
                    positionC = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
                    positionD = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
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
                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);
                    meshVertexArray.push_back(meshVertexD);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 3);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 3);
                    }

                    meshData.vertexCount += 4;
                    meshData.indexCount += 6;
                }
                else
                {
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

                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);

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

                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);

                    meshData.vertexCount += 6;
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

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount);
                    }

                    meshData.vertexCount += 3;
                    meshData.indexCount += 3;

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
            meshVertexArray.push_back(meshVertexA);
            meshVertexArray.push_back(meshVertexB);
            meshVertexArray.push_back(meshVertexC);
            meshVertexArray.push_back(meshVertexD);

            if (dotABC > 0.0f)
            {
                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);
            }
            else
            {
                meshIndexArray.push_back(meshData.vertexCount + 2);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount);
            }

            if (dotDCB > 0.0f)
            {
                meshIndexArray.push_back(meshData.vertexCount + 3);
                meshIndexArray.push_back(meshData.vertexCount + 2);
                meshIndexArray.push_back(meshData.vertexCount + 1);
            }
            else
            {
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);
                meshIndexArray.push_back(meshData.vertexCount + 3);
            }

            meshData.vertexCount += 4;
            meshData.indexCount += 6;
        }
        else
        {
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

            meshIndexArray.push_back(meshData.indexCount++);
            meshIndexArray.push_back(meshData.indexCount++);
            meshIndexArray.push_back(meshData.indexCount++);

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

            meshIndexArray.push_back(meshData.indexCount++);
            meshIndexArray.push_back(meshData.indexCount++);
            meshIndexArray.push_back(meshData.indexCount++);

            meshData.vertexCount += 6;
        }

        if (positionalTexture)
        {
            for (size_t i = 0; i < meshData.vertexCount; ++i)
            {
                unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
                MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

                currentMeshVertex.textureCoordinate.x = currentMeshVertex.position.x;
                currentMeshVertex.textureCoordinate.y = currentMeshVertex.position.y;
            }
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            const MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentMeshVertex.position.x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, currentMeshVertex.position.y);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentMeshVertex.position.x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, currentMeshVertex.position.y);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        return true;
    }

    bool MeshStores::Generate2DLineLoop(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        float width,
        bool beveledCorners,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 position1 = positions[endIndex];
        glm::vec2 position2 = positions[0];

        glm::vec2 direction1 = position2 - position1;
        float slope1 = direction1.y / direction1.x;

        glm::vec2 offset01(-1.0f * direction1.y, direction1.x);
        float magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
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
        float magnitude02 = std::sqrtf(offset02.x * offset02.x + offset02.y * offset02.y);
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
            if (angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
            {
                sharpCorner = true;
            }
        }

        if (sharpCorner)
        {
            if (angle > 0.0f) // turn to the left
            {
                positionA = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
                positionB = positionF;
            }
            else
            {
                positionA = positionE;
                positionB = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
            }
        }
        else
        {
            positionA = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
            positionB = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
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

            if (slope1 != slope2)
            {
                offset02 = glm::vec2(-1.0f * direction2.y, direction2.x);
                magnitude02 = std::sqrtf(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                positionE = position2 + scaled2;
                positionF = position2 - scaled2;

                sharpCorner = false;
                angle = Get2DVectorAngle(direction1, direction2);
                if (beveledCorners)
                {
                    if (angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                    {
                        sharpCorner = true;
                    }
                }

                if (sharpCorner)
                {
                    offset01 = glm::vec2(-1.0f * direction1.y, direction1.x);
                    magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        positionC = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
                        positionD = position2 - scaled1;
                    }
                    else
                    {
                        positionC = position2 + scaled1;
                        positionD = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
                    }
                }
                else
                {
                    positionC = GetLineLineIntersection2d(positionA, slope1, positionE, slope2);
                    positionD = GetLineLineIntersection2d(positionB, slope1, positionF, slope2);
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
                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);
                    meshVertexArray.push_back(meshVertexD);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 3);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 3);
                    }

                    meshData.vertexCount += 4;
                    meshData.indexCount += 6;
                }
                else
                {
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

                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);

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

                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);

                    meshData.vertexCount += 6;
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

                    meshVertexArray.push_back(meshVertexA);
                    meshVertexArray.push_back(meshVertexB);
                    meshVertexArray.push_back(meshVertexC);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount);
                    }

                    meshData.vertexCount += 3;
                    meshData.indexCount += 3;

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

        if (positionalTexture)
        {
            for (size_t i = 0; i < meshData.vertexCount; ++i)
            {
                unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
                MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

                currentMeshVertex.textureCoordinate.x = currentMeshVertex.position.x;
                currentMeshVertex.textureCoordinate.y = currentMeshVertex.position.y;
            }
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            const MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentMeshVertex.position.x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, currentMeshVertex.position.y);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentMeshVertex.position.x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, currentMeshVertex.position.y);
        }

        if (recenter)
        {
            RecenterMesh(meshData, meshVertexArray);
        }

        return true;
    }

    bool MeshStores::Generate2DRegularPolygon(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float radius,
        size_t sides,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        if (sides < 3 || radius == 0.0f)
        {
            return false;
        }

        std::vector<glm::vec2> positions;

        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        positions.push_back(radialVector);

        for (size_t i = 0; i < sides; ++i)
        {
            radialVector = Rotate2DVector(radialVector, sectionAngle);
            positions.push_back(radialVector);
        }

        bool success = false;
        if (positionalTexture)
        {
            std::vector<glm::vec2> textureCoordinates;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                textureCoordinates.emplace_back(positions[i].x + 0.5f, positions[i].y + 0.5f);
            }

            success = Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, textureCoordinates, recenter, triangulate);
        }
        else
        {
            success = Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), recenter, triangulate);
        }

        return success;
    }

    bool MeshStores::Generate2DBezeledRectangle(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float width,
        float height,
        float bezelSize,
        float bezelSections,
        bool triangulate,
        bool positionalTexture)
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
            float x = bezelSize * std::cosf(stackAngle);
            float y = bezelSize * std::sinf(stackAngle);

            positions.emplace_back(x + halfWidth, y + halfHeight);
        }

        // top left
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = 0.5f * glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * std::cosf(stackAngle);
            float y = bezelSize * std::sinf(stackAngle);

            positions.emplace_back(x - halfWidth, y + halfHeight);
        }

        // bottom left
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * std::cosf(stackAngle);
            float y = bezelSize * std::sinf(stackAngle);

            positions.emplace_back(x - halfWidth, y - halfHeight);
        }

        // bottom right
        for (size_t i = 0; i < bezelSections + 1; ++i)
        {
            float stackAngle = 1.5f * glm::pi<float>() + radialStep * (float)i;
            float x = bezelSize * std::cosf(stackAngle);
            float y = bezelSize * std::sinf(stackAngle);

            positions.emplace_back(x + halfWidth, y - halfHeight);
        }

        bool success = false;
        if (positionalTexture)
        {
            std::vector<glm::vec2> textureCoordinates;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                textureCoordinates.emplace_back(positions[i].x + 0.5f, positions[i].y + 0.5f);
            }

            success = Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, textureCoordinates, false, triangulate);
        }
        else
        {
            success = Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), false, triangulate);
        }

        return success;
    }

    bool MeshStores::Generate2DSprite(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float width,
        float height,
        float textureBottom,
        float textureTop,
        float textureLeft,
        float textureRight,
        bool triangulate)
    {
        if (width <= 0.0f || height <= 0.0f)
        {
            return false;
        }

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.x = halfWidth;
        meshData.maxVertexPosition.y = halfHeight;
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.x = -halfWidth;
        meshData.minVertexPosition.y = -halfHeight;
        meshData.minVertexPosition.z = 0.0f;

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

        if (triangulate)
        {
            meshVertexArray.push_back(topLeftVertex);
            meshVertexArray.push_back(bottomLeftVertex);
            meshVertexArray.push_back(topRightVertex);
            meshVertexArray.push_back(bottomRightVertex);
            meshVertexArray.push_back(topRightVertex);
            meshVertexArray.push_back(bottomLeftVertex);

            meshIndexArray.push_back(0);
            meshIndexArray.push_back(1);
            meshIndexArray.push_back(2);
            meshIndexArray.push_back(3);
            meshIndexArray.push_back(4);
            meshIndexArray.push_back(5);

            meshData.vertexCount = 6;
            meshData.indexCount = 6;
        }
        else
        {
            meshVertexArray.push_back(topLeftVertex);
            meshVertexArray.push_back(bottomLeftVertex);
            meshVertexArray.push_back(bottomRightVertex);
            meshVertexArray.push_back(topRightVertex);

            meshIndexArray.push_back(0);
            meshIndexArray.push_back(1);
            meshIndexArray.push_back(3);
            meshIndexArray.push_back(2);
            meshIndexArray.push_back(3);
            meshIndexArray.push_back(1);

            meshData.vertexCount = 4;
            meshData.indexCount = 6;
        }

        return true;
    }

    bool MeshStores::GenerateBox(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float xLength,
        float yLength,
        float zLength,
        bool smoothNormals,
        bool triangulate)
    {
        if (xLength <= 0.0f || yLength <= 0.0f || zLength <= 0.0f)
        {
            return false;
        }

        glm::vec3 octant_posX_posY_posZ(xLength /  2.0f, yLength /  2.0f, zLength /  2.0f);
        glm::vec3 octant_posX_posY_negZ(xLength /  2.0f, yLength /  2.0f, zLength / -2.0f);
        glm::vec3 octant_posX_negY_posZ(xLength /  2.0f, yLength / -2.0f, zLength /  2.0f);
        glm::vec3 octant_posX_negY_negZ(xLength /  2.0f, yLength / -2.0f, zLength / -2.0f);
        glm::vec3 octant_negX_posY_posZ(xLength / -2.0f, yLength /  2.0f, zLength /  2.0f);
        glm::vec3 octant_negX_posY_negZ(xLength / -2.0f, yLength /  2.0f, zLength / -2.0f);
        glm::vec3 octant_negX_negY_posZ(xLength / -2.0f, yLength / -2.0f, zLength /  2.0f);
        glm::vec3 octant_negX_negY_negZ(xLength / -2.0f, yLength / -2.0f, zLength / -2.0f);

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.x = xLength / 2.0f;
        meshData.minVertexPosition.x = xLength / -2.0f;
        meshData.maxVertexPosition.y = yLength / 2.0f;
        meshData.minVertexPosition.x = yLength / -2.0f;
        meshData.maxVertexPosition.z = zLength / 2.0f;
        meshData.minVertexPosition.x = zLength / -2.0f;

        if (smoothNormals)
        {
            float dimensionLength = 1.0f / std::sqrtf(3.0f);

            glm::vec3 normal_posX_posY_posZ( dimensionLength,  dimensionLength,  dimensionLength);
            glm::vec3 normal_posX_posY_negZ( dimensionLength,  dimensionLength, -dimensionLength);
            glm::vec3 normal_posX_negY_posZ( dimensionLength, -dimensionLength,  dimensionLength);
            glm::vec3 normal_posX_negY_negZ( dimensionLength, -dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_posY_posZ(-dimensionLength,  dimensionLength,  dimensionLength);
            glm::vec3 normal_negX_posY_negZ(-dimensionLength,  dimensionLength, -dimensionLength);
            glm::vec3 normal_negX_negY_posZ(-dimensionLength, -dimensionLength,  dimensionLength);
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
                    meshIndexArray.push_back((unsigned int)i);
                }

                meshData.vertexCount = 36;
                meshData.indexCount = 36;
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
                meshIndexArray.push_back(4);
                meshIndexArray.push_back(6);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(2);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(6);

                // right
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(2);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(3);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(2);

                // left
                meshIndexArray.push_back(5);
                meshIndexArray.push_back(7);
                meshIndexArray.push_back(4);
                meshIndexArray.push_back(6);
                meshIndexArray.push_back(4);
                meshIndexArray.push_back(7);

                // top
                meshIndexArray.push_back(5);
                meshIndexArray.push_back(4);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(4);

                // bottom
                meshIndexArray.push_back(6);
                meshIndexArray.push_back(7);
                meshIndexArray.push_back(2);
                meshIndexArray.push_back(3);
                meshIndexArray.push_back(2);
                meshIndexArray.push_back(7);

                // back
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(3);
                meshIndexArray.push_back(5);
                meshIndexArray.push_back(7);
                meshIndexArray.push_back(5);
                meshIndexArray.push_back(3);

                meshData.vertexCount = 8;
                meshData.indexCount = 36;
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
                    meshIndexArray.push_back((unsigned int)i);
                }

                meshData.vertexCount = 36;
                meshData.indexCount = 36;
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

                meshIndexArray.push_back(0);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(3);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(3);
                meshIndexArray.push_back(2);

                meshIndexArray.push_back(4);
                meshIndexArray.push_back(5);
                meshIndexArray.push_back(7);
                meshIndexArray.push_back(4);
                meshIndexArray.push_back(7);
                meshIndexArray.push_back(6);

                meshIndexArray.push_back(8);
                meshIndexArray.push_back(9);
                meshIndexArray.push_back(11);
                meshIndexArray.push_back(8);
                meshIndexArray.push_back(11);
                meshIndexArray.push_back(10);

                meshIndexArray.push_back(12);
                meshIndexArray.push_back(13);
                meshIndexArray.push_back(15);
                meshIndexArray.push_back(12);
                meshIndexArray.push_back(15);
                meshIndexArray.push_back(14);

                meshIndexArray.push_back(16);
                meshIndexArray.push_back(17);
                meshIndexArray.push_back(19);
                meshIndexArray.push_back(16);
                meshIndexArray.push_back(19);
                meshIndexArray.push_back(18);

                meshIndexArray.push_back(20);
                meshIndexArray.push_back(21);
                meshIndexArray.push_back(23);
                meshIndexArray.push_back(20);
                meshIndexArray.push_back(23);
                meshIndexArray.push_back(22);

                meshData.vertexCount = 24;
                meshData.indexCount = 36;
            }
        }

        return true;
    }

    bool MeshStores::GenerateCapsule(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float cylindricalHeight,
        float radius,
        size_t faces,
        size_t stacks,
        bool smoothNormals,
        bool triangulate)
    {
        if (cylindricalHeight < 0.0f || radius <= 0.0f || faces < 3 || stacks < 1)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = 0;
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = 0;
        meshData.maxVertexPosition.y = cylindricalHeight / 2.0f + radius;
        meshData.minVertexPosition.y = -1.0f * (cylindricalHeight  / 2.0f + radius);

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
            float xz = radius * std::cosf(stackAngle);
            float y = radius * std::sinf(stackAngle);

            for (size_t j = 0; j < faces; ++j)
            {
                float faceAngle = faceStep * (float)j;
                float x = xz * std::cosf(faceAngle);
                float z = xz * std::sinf(faceAngle);

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
            float xz = radius * std::cosf(stackAngle);
            float y = radius * std::sinf(stackAngle);

            for (size_t j = 0; j < faces; ++j)
            {
                float faceAngle = faceStep * (float)j;
                float x = xz * std::cosf(faceAngle);
                float z = xz * std::sinf(faceAngle);

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

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentPosition.x);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, currentPosition.z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentPosition.x);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, currentPosition.z);
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

                meshData.vertexCount = (unsigned int)(faces * stacks * 12);

                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
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
                    meshIndexArray.push_back(0);
                    meshIndexArray.push_back((unsigned int)i);
                    meshIndexArray.push_back((unsigned int)i + 1);
                }
                meshIndexArray.push_back(0);
                meshIndexArray.push_back((unsigned int)faces);
                meshIndexArray.push_back(1);

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

                        meshIndexArray.push_back((unsigned int)i1);
                        meshIndexArray.push_back((unsigned int)i2);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i4);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i2);
                    }
                    size_t i1 = (i + 1) * faces;
                    size_t i2 = (i + 2) * faces;
                    size_t i3 = i * faces + 1;
                    size_t i4 = (i + 1) * faces + 1;

                    meshIndexArray.push_back((unsigned int)i1);
                    meshIndexArray.push_back((unsigned int)i2);
                    meshIndexArray.push_back((unsigned int)i3);
                    meshIndexArray.push_back((unsigned int)i4);
                    meshIndexArray.push_back((unsigned int)i3);
                    meshIndexArray.push_back((unsigned int)i2);
                }

                // bottom
                size_t lastIndex = positions.size() - 1;
                for (size_t i = 1; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)lastIndex);
                    meshIndexArray.push_back((unsigned int)(lastIndex - i));
                    meshIndexArray.push_back((unsigned int)(lastIndex - i) - 1);
                }
                meshIndexArray.push_back((unsigned int)lastIndex);
                meshIndexArray.push_back((unsigned int)(lastIndex - faces));
                meshIndexArray.push_back((unsigned int)lastIndex - 1);

                meshData.vertexCount = (unsigned int)positions.size();

                meshData.indexCount = (unsigned int)(faces* stacks * 12);
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

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
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

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
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
                    
                    if (triangulate)
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v2);

                        meshData.vertexCount += 6;

                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                    }
                    else
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);

                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 3);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 1);

                        meshData.vertexCount += 4;
                        meshData.indexCount += 6;
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

                if (triangulate)
                {
                    meshVertexArray.push_back(v1);
                    meshVertexArray.push_back(v2);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v4);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v2);

                    meshData.vertexCount += 6;

                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                    meshIndexArray.push_back(meshData.indexCount++);
                }
                else
                {
                    meshVertexArray.push_back(v1);
                    meshVertexArray.push_back(v2);
                    meshVertexArray.push_back(v3);
                    meshVertexArray.push_back(v4);

                    meshIndexArray.push_back(meshData.vertexCount);
                    meshIndexArray.push_back(meshData.vertexCount + 1);
                    meshIndexArray.push_back(meshData.vertexCount + 2);
                    meshIndexArray.push_back(meshData.vertexCount + 3);
                    meshIndexArray.push_back(meshData.vertexCount + 2);
                    meshIndexArray.push_back(meshData.vertexCount + 1);

                    meshData.vertexCount += 4;
                    meshData.indexCount += 6;
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

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
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

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
            }
        }

        return true;
    }

    bool MeshStores::GenerateCone(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = 0;
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = 0;
        meshData.maxVertexPosition.y = 0.5f * height;
        meshData.minVertexPosition.y = -0.5f * height;

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        if (smoothNormals)
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexArrayPtr;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces + 1);
                destMeshVertexArrayPtr = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexArrayPtr = &meshVertexArray;
            }

            MeshVertex topVertex;
            topVertex.position = glm::vec3(0.0f, 0.5f * height, 0.0f);
            topVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            destMeshVertexArrayPtr->push_back(topVertex);

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * std::sinf(faceAngle);
                float z = radius * std::cosf(faceAngle);

                meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, x);
                meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, z);

                meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, x);
                meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, z);

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(x, -0.5f * height, z);
                bottomVertex.normal = glm::normalize(bottomVertex.position);
                destMeshVertexArrayPtr->push_back(bottomVertex);
            }

            if (triangulate)
            {
                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[0]);
                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                    meshData.vertexCount += 3;
                }
                meshVertexArray.push_back(tempMeshVertexArray[0]);
                meshVertexArray.push_back(tempMeshVertexArray[faces]);
                meshVertexArray.push_back(tempMeshVertexArray[1]);
                meshData.vertexCount += 3;

                // bottom
                for (size_t i = 2; i < faces; ++i)
                {
                    meshVertexArray.push_back(tempMeshVertexArray[1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshData.vertexCount += 3;
                }

                meshData.vertexCount = (unsigned int)faces * 6 - 6;
                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces; ++i)
                {
                    meshIndexArray.push_back(0);
                    meshIndexArray.push_back((unsigned int)i);
                    meshIndexArray.push_back((unsigned int)i + 1);
                    meshData.indexCount += 3;
                }
                meshIndexArray.push_back(0);
                meshIndexArray.push_back((unsigned int)faces);
                meshIndexArray.push_back(1);
                meshData.indexCount += 3;

                // bottom
                for (size_t i = 2; i < faces; ++i)
                {
                    meshIndexArray.push_back(1);
                    meshIndexArray.push_back((unsigned int)i + 1);
                    meshIndexArray.push_back((unsigned int)i);
                    meshData.indexCount += 3;
                }

                meshData.vertexCount = (unsigned int)faces + 1;
                meshData.indexCount = (unsigned int)faces * 6 - 6;
            }
        }
        else
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexArrayPtr;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 6 - 6);
                destMeshVertexArrayPtr = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexArrayPtr = &meshVertexArray;
            }

            float y = 1.0f / std::sqrtf(2.0f);

            float previousX = 0.0f;
            float previousZ = radius;
            for (size_t i = 1; i < faces + 1; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float newX = radius * std::sinf(faceAngle);
                float newZ = radius * std::cosf(faceAngle);

                meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, previousX);
                meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, previousZ);

                meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, previousX);
                meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, previousZ);

                glm::vec3 vectorToTop(0.0f - newX, 0.5f * height, 0.0f - newZ);
                glm::vec3 vectorToPrevious(previousX - newX, 0.0f, previousZ - newZ);
                glm::vec3 normal = glm::normalize(glm::cross(vectorToTop, vectorToPrevious));

                MeshVertex topVertex;
                topVertex.position = glm::vec3(0.0f, 0.5f * height, 0.0f);
                topVertex.normal = normal;
                destMeshVertexArrayPtr->push_back(topVertex);

                MeshVertex previousVertex;
                previousVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                previousVertex.normal = normal;
                destMeshVertexArrayPtr->push_back(previousVertex);

                MeshVertex newVertex;
                newVertex.position = glm::vec3(newX, -0.5f * height, newZ);
                newVertex.normal = normal;
                destMeshVertexArrayPtr->push_back(newVertex);

                previousX = newX;
                previousZ = newZ;
            }

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * std::sinf(faceAngle);
                float z = radius * std::cosf(faceAngle);

                MeshVertex vertex;
                vertex.position = glm::vec3(x, -0.5f * height, z);
                vertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                destMeshVertexArrayPtr->push_back(vertex);
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

                meshData.vertexCount = (unsigned int)faces * 6 - 6;
                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
                }
            }
            else
            {
                // top
                for (size_t i = 0; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)i * 3);
                    meshIndexArray.push_back((unsigned int)i * 3 + 1);
                    meshIndexArray.push_back((unsigned int)i * 3 + 2);
                }

                // bottom
                size_t firstBottomIndex = faces * 3;
                size_t pastLastBottomIndex = faces * 4;
                for (size_t i = firstBottomIndex; i < pastLastBottomIndex - 2; ++i)
                {
                    meshIndexArray.push_back((unsigned int)firstBottomIndex);
                    meshIndexArray.push_back((unsigned int)i + 2);
                    meshIndexArray.push_back((unsigned int)i + 1);
                }

                meshData.vertexCount = (unsigned int)faces * 4;
                meshData.indexCount = (unsigned int)faces * 6 - 6;
            }
        }

        return true;
    }

    bool MeshStores::GenerateCylinder(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = 0;
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = 0;
        meshData.maxVertexPosition.y = 0.5f * height;
        meshData.minVertexPosition.y = -0.5f * height;

        const float faceStep = 2.0f * glm::pi<float>() / (float)(faces);

        if (smoothNormals)
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexArrayPtr;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 2);
                destMeshVertexArrayPtr = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexArrayPtr = &meshVertexArray;
            }

            float y = 1.0f / std::sqrtf(2.0f);

            for (size_t i = 0; i < faces; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float x = radius * std::sinf(faceAngle);
                float z = radius * std::cosf(faceAngle);

                meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, x);
                meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, z);

                meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, x);
                meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, z);

                MeshVertex topVertex;
                topVertex.position = glm::vec3(x, 0.5f * height, z);
                topVertex.normal = glm::vec3(x * y / radius, y, z * y / radius);
                destMeshVertexArrayPtr->push_back(topVertex);

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(x, -0.5f * height, z);
                bottomVertex.normal = glm::vec3(x * y / radius, -1.0f * y, z * y / radius);
                destMeshVertexArrayPtr->push_back(bottomVertex);
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

                meshData.vertexCount = (unsigned int)(faces - 1) * 12;
                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back(0);
                    meshIndexArray.push_back((unsigned int)i * 2);
                    meshIndexArray.push_back((unsigned int)(i + 1) * 2);
                }

                // body
                for (size_t i = 0; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)i * 2);
                    meshIndexArray.push_back((unsigned int)i * 2 + 1);
                    meshIndexArray.push_back((unsigned int)i * 2 + 2);
                    meshIndexArray.push_back((unsigned int)i * 2 + 3);
                    meshIndexArray.push_back((unsigned int)i * 2 + 2);
                    meshIndexArray.push_back((unsigned int)i * 2 + 1);
                }
                meshIndexArray.push_back((unsigned int)faces * 2 - 2);
                meshIndexArray.push_back((unsigned int)faces * 2 - 1);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back(1);
                meshIndexArray.push_back(0);
                meshIndexArray.push_back((unsigned int)faces * 2 - 1);

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(i + 1) * 2 + 1);
                    meshIndexArray.push_back((unsigned int)i * 2 + 1);
                    meshIndexArray.push_back(1);
                }

                meshData.vertexCount = (unsigned int)faces * 2;
                meshData.indexCount = (unsigned int)(faces - 1) * 12;
            }
        }
        else
        {
            std::vector<MeshVertex> tempMeshVertexArray;

            std::vector<MeshVertex>* destMeshVertexArrayPtr;
            if (triangulate)
            {
                tempMeshVertexArray.reserve(faces * 6);
                destMeshVertexArrayPtr = &tempMeshVertexArray;
            }
            else
            {
                destMeshVertexArrayPtr = &meshVertexArray;
            }

            float y = 1.0f / std::sqrtf(2.0f);

            float previousX = 0.0f;
            float previousZ = radius;
            for (size_t i = 1; i < faces + 1; ++i)
            {
                float faceAngle = faceStep * (float)i;
                float newX = radius * std::sinf(faceAngle);
                float newZ = radius * std::cosf(faceAngle);

                meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, previousX);
                meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, previousZ);

                meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, previousX);
                meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, previousZ);

                glm::vec3 vectorToPrevious(previousX - newX, 0.0f, previousZ - newZ);
                glm::vec3 bodyNormal = glm::normalize(glm::cross(vectorToPrevious, glm::vec3(0.0f, -1.0f, 0.0f)));

                MeshVertex topVertex;
                topVertex.position = glm::vec3(previousX, 0.5f * height, previousZ);
                topVertex.normal = glm::vec3(0.0f, y, 0.0f);
                destMeshVertexArrayPtr->push_back(topVertex);

                MeshVertex bottomVertex;
                bottomVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                bottomVertex.normal = glm::vec3(0.0f, -1.0f * y, 0.0f);
                destMeshVertexArrayPtr->push_back(bottomVertex);

                MeshVertex topLeftBodyVertex;
                topLeftBodyVertex.position = glm::vec3(previousX, 0.5f * height, previousZ);
                topLeftBodyVertex.normal = bodyNormal;
                destMeshVertexArrayPtr->push_back(topLeftBodyVertex);

                MeshVertex bottomLeftBodyVertex;
                bottomLeftBodyVertex.position = glm::vec3(previousX, -0.5f * height, previousZ);
                bottomLeftBodyVertex.normal = bodyNormal;
                destMeshVertexArrayPtr->push_back(bottomLeftBodyVertex);

                MeshVertex topRightBodyVertex;
                topRightBodyVertex.position = glm::vec3(newX, 0.5f * height, newZ);
                topRightBodyVertex.normal = bodyNormal;
                destMeshVertexArrayPtr->push_back(topRightBodyVertex);

                MeshVertex bottomRightBodyVertex;
                bottomRightBodyVertex.position = glm::vec3(newX, -0.5f * height, newZ);
                bottomRightBodyVertex.normal = bodyNormal;
                destMeshVertexArrayPtr->push_back(bottomRightBodyVertex);

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

                meshData.vertexCount = (unsigned int)(faces - 1) * 12;

                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
                }
            }
            else
            {
                // top
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back(0);
                    meshIndexArray.push_back((unsigned int)i * 6);
                    meshIndexArray.push_back((unsigned int)(i + 1) * 6);
                }

                // bottom
                for (size_t i = 1; i < faces - 1; ++i)
                {
                    meshIndexArray.push_back((unsigned int)(i + 1) * 6 + 1);
                    meshIndexArray.push_back((unsigned int)i * 6 + 1);
                    meshIndexArray.push_back(1);
                }

                // body
                for (size_t i = 0; i < faces; ++i)
                {
                    meshIndexArray.push_back((unsigned int)i * 6 + 2);
                    meshIndexArray.push_back((unsigned int)i * 6 + 3);
                    meshIndexArray.push_back((unsigned int)i * 6 + 4);
                    meshIndexArray.push_back((unsigned int)i * 6 + 5);
                    meshIndexArray.push_back((unsigned int)i * 6 + 4);
                    meshIndexArray.push_back((unsigned int)i * 6 + 3);
                }

                meshData.vertexCount = (unsigned int)faces * 6;
                meshData.indexCount = (unsigned int)(faces - 1) * 12;
            }
        }

        return true;
    }

    bool MeshStores::GenerateIcosphere(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        float radius,
        size_t subdivisions,
        bool smoothNormals,
        bool triangulate)
    {
        if (radius <= 0.0f)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = 0;
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = 0;
        meshData.maxVertexPosition.y = radius;
        meshData.minVertexPosition.y = -1.0f * radius;

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

        const float hAngle = 0.4f * glm::pi<float>();
        const float vAngle = 0.5f * glm::pi<float>() - std::atan(0.5f);

        // top
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, 0.0f, ((float)i + 0.5f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 12.0f, 1.0f);
        }

        // body
        for (size_t i = 0; i < 6; ++i)
        {
            positions.emplace_back(radius, vAngle, (float)i * hAngle);
            textureCoordinates.emplace_back((float)(i * 2) / 12.0f, 2.0f / 3.0f);
        }

        for (size_t i = 0; i < 6; ++i)
        {
            positions.emplace_back(radius, 2.0f * vAngle, ((float)i + 0.5f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 1) / 12.0f, 1.0f / 3.0f);
        }

        // bottom
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>(), ((float)i + 1.0f) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 2) / 12.0f, 0.0f);
        }

        // Convert positions for Polar Coordinates to Cartesian Coordinates
        // ---------------------------------------------------------------------
        for (size_t i = 0; i < positions.size(); ++i)
        {
            float x = positions[i].x;
            float y = positions[i].y;
            float z = positions[i].z;
            positions[i].z = x * std::sinf(y) * std::cosf(z);
            positions[i].x = x * std::sinf(y) * std::sinf(z);
            positions[i].y = x * std::cosf(y);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            const glm::vec3& currentPosition = positions[i];

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentPosition.x);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, currentPosition.z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentPosition.x);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, currentPosition.z);
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

                glm::vec3 positionA;
                positionA.x = position0.x + position1.x;
                positionA.y = position0.y + position1.y;
                positionA.z = position0.z + position1.z;
                float scaleA = radius / sqrtf(positionA.x * positionA.x + positionA.y * positionA.y + positionA.z * positionA.z);
                positionA.x *= scaleA;
                positionA.y *= scaleA;
                positionA.z *= scaleA;
                glm::vec2 textureCoordinateA;
                textureCoordinateA.x = (textureCoordinate0.x + textureCoordinate1.x) / 2.0f;
                textureCoordinateA.y = (textureCoordinate0.y + textureCoordinate1.y) / 2.0f;

                glm::vec3 positionB;
                positionB.x = position1.x + position2.x;
                positionB.y = position1.y + position2.y;
                positionB.z = position1.z + position2.z;
                float scaleB = radius / sqrtf(positionB.x * positionB.x + positionB.y * positionB.y + positionB.z * positionB.z);
                positionB.x *= scaleB;
                positionB.y *= scaleB;
                positionB.z *= scaleB;
                glm::vec2 textureCoordinateB;
                textureCoordinateB.x = (textureCoordinate1.x + textureCoordinate2.x) / 2.0f;
                textureCoordinateB.y = (textureCoordinate1.y + textureCoordinate2.y) / 2.0f;

                glm::vec3 positionC;
                positionC.x = position2.x + position0.x;
                positionC.y = position2.y + position0.y;
                positionC.z = position2.z + position0.z;
                float scaleC = radius / sqrtf(positionC.x * positionC.x + positionC.y * positionC.y + positionC.z * positionC.z);
                positionC.x *= scaleC;
                positionC.y *= scaleC;
                positionC.z *= scaleC;
                glm::vec2 textureCoordinateC;
                textureCoordinateC.x = (textureCoordinate2.x + textureCoordinate0.x) / 2.0f;
                textureCoordinateC.y = (textureCoordinate2.y + textureCoordinate0.y) / 2.0f;

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

            triangleFaces = newTriangleFaces;
        }

        // Generate MeshVerticies
        // ---------------------------------------------------------------------
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

                meshData.vertexCount = (unsigned int)triangleFaces.size() * 3;

                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
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

                meshData.vertexCount = (unsigned int)positions.size();

                for (size_t i = 0; i < triangleFaces.size(); ++i)
                {
                    const glm::uvec3& currentTriangleFace = triangleFaces[i];
                    meshIndexArray.push_back(currentTriangleFace.x);
                    meshIndexArray.push_back(currentTriangleFace.y);
                    meshIndexArray.push_back(currentTriangleFace.z);
                }

                meshData.indexCount = (unsigned int)triangleFaces.size() * 3;
            }
        }
        else
        {
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

            meshData.vertexCount = (unsigned int)triangleFaces.size() * 3;

            while (meshData.indexCount < meshData.vertexCount)
            {
                meshIndexArray.push_back(meshData.indexCount++);
            }
        }

        return true;
    }

    bool MeshStores::GenerateSphere(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.vertexCount = 0;
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.indexCount = 0;
        meshData.maxVertexPosition.y = radius;
        meshData.minVertexPosition.y = -1.0f * radius;

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

        const float longStep = -2.0f * glm::pi<float>() / (float)(longitudinalSections);
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
            float xz = radius * std::cosf(latiAngle);
            float y = radius * std::sinf(latiAngle);

            for (size_t j = 0; j < longitudinalSections + 1; ++j) // +1 for wrap around
            {
                float longAngle = longStep * (float)j;
                float x = xz * std::cosf(longAngle);
                float z = xz * std::sinf(longAngle);

                positions.emplace_back(x, y, z);
                textureCoordinates.emplace_back((float)j / (float)longitudinalSections, (float)(latitudinalSections - i) / (float)latitudinalSections);
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

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentPosition.x);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, currentPosition.z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentPosition.x);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, currentPosition.z);
        }

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
                // p1
                // |  \
                // |   \
                // p2 -- p2+1 (p3)
                for (size_t i = 0; i < firstBodyPosition; ++i)
                {
                    size_t i2 = i + longitudinalSections;

                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2 + 1]);
                }

                // body
                // p1 -- p1+1
                // |  \  |
                // |   \ |
                // p2 -- p2+1 (p3)
                if (latitudinalSections > 2)
                {
                    for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                    {
                        if ((i + 2) % bodyWrapVertexCount == 0)
                        {
                            continue;
                        }

                        size_t i2 = i + bodyWrapVertexCount;

                        meshVertexArray.push_back(tempMeshVertexArray[i]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2]);
                        meshVertexArray.push_back(tempMeshVertexArray[i2 + 1]);

                        meshVertexArray.push_back(tempMeshVertexArray[i2 + 1]);
                        meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                        meshVertexArray.push_back(tempMeshVertexArray[i]);
                    }
                }

                // bottom
                // p1 -- p1+1 (p3)
                // |   /
                // |  /
                // p2
                for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
                {
                    size_t i2 = i + bodyWrapVertexCount;

                    meshVertexArray.push_back(tempMeshVertexArray[i]);
                    meshVertexArray.push_back(tempMeshVertexArray[i2]);
                    meshVertexArray.push_back(tempMeshVertexArray[i + 1]);
                }

                meshData.vertexCount = (unsigned int)((latitudinalSections - 1) * longitudinalSections * 6);

                while (meshData.indexCount < meshData.vertexCount)
                {
                    meshIndexArray.push_back(meshData.indexCount++);
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

                meshData.vertexCount = (unsigned int)positions.size();

                // top
                // p1
                // |  \
                // |   \
                // p2 -- p2+1 (p3)
                for (size_t i = 0; i < firstBodyPosition; ++i)
                {
                    size_t i2 = i + longitudinalSections;
                    size_t i3 = i2 + 1;

                    meshIndexArray.push_back((unsigned int)i);
                    meshIndexArray.push_back((unsigned int)i2);
                    meshIndexArray.push_back((unsigned int)i3);
                }

                // body
                // p1 -- p1+1 (p4)
                // |  \  |
                // |   \ |
                // p2 -- p2+1 (p3)
                if (latitudinalSections > 2)
                {
                    for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                    {
                        if ((i + 2) % bodyWrapVertexCount == 0)
                        {
                            continue;
                        }

                        size_t i2 = i + bodyWrapVertexCount;
                        size_t i3 = i2 + 1;
                        size_t i4 = i + 1;

                        meshIndexArray.push_back((unsigned int)i);
                        meshIndexArray.push_back((unsigned int)i2);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i4);
                        meshIndexArray.push_back((unsigned int)i);
                    }
                }

                // bottom
                // p1 -- p1+1 (p3)
                // |   /
                // |  /
                // p2
                for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
                {
                    size_t i2 = i + bodyWrapVertexCount;
                    size_t i3 = i + 1;

                    meshIndexArray.push_back((unsigned int)i);
                    meshIndexArray.push_back((unsigned int)i2);
                    meshIndexArray.push_back((unsigned int)i3);
                }

                meshData.indexCount = (unsigned int)((latitudinalSections - 1) * longitudinalSections * 6);
            }
        }
        else
        {
            // top
            // p1
            // |  \
            // |   \
            // p2 -- p2+1 (p3)
            for (size_t i = 0; i < firstBodyPosition; ++i)
            {
                size_t i2 = i + longitudinalSections;

                const glm::vec3& p1 = positions[i];
                const glm::vec3& p2 = positions[i2];
                const glm::vec3& p3 = positions[i2 + 1];

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
                v3.textureCoordinate = textureCoordinates[i2 + 1];
                v3.normal = normal123;

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
            }

            // body
            // p1 -- p1+1 (p4)
            // |  \  |
            // |   \ |
            // p2 -- p2+1 (p3)
            if (latitudinalSections > 2)
            {
                for (size_t i = firstBodyPosition; i < firstBottomPosition - 1; ++i)
                {
                    if ((i + 2) % bodyWrapVertexCount == 0)
                    {
                        continue;
                    }

                    size_t i2 = i + bodyWrapVertexCount;
                    size_t i3 = i2 + 1;
                    size_t i4 = i + 1;

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

                    if (triangulate)
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);

                        meshData.vertexCount += 6;

                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                        meshIndexArray.push_back(meshData.indexCount++);
                    }
                    else
                    {
                        meshVertexArray.push_back(v1);
                        meshVertexArray.push_back(v2);
                        meshVertexArray.push_back(v3);
                        meshVertexArray.push_back(v4);

                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 1);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount);
                        meshIndexArray.push_back(meshData.vertexCount + 2);
                        meshIndexArray.push_back(meshData.vertexCount + 3);

                        meshData.vertexCount += 4;
                        meshData.indexCount += 6;
                    }
                }
            }

            // bottom
            // p1 -- p1+1 (p3)
            // |   /
            // |  /
            // p2
            for (size_t i = firstBottomPosition; i < positions.size() - bodyWrapVertexCount; ++i)
            {
                size_t i2 = i + bodyWrapVertexCount;

                const glm::vec3& p1 = positions[i];
                const glm::vec3& p2 = positions[i2];
                const glm::vec3& p3 = positions[i + 1];

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
                v3.textureCoordinate = textureCoordinates[i + 1];
                v3.normal = normal123;

                meshVertexArray.push_back(v1);
                meshVertexArray.push_back(v2);
                meshVertexArray.push_back(v3);

                meshIndexArray.push_back(meshData.vertexCount);
                meshIndexArray.push_back(meshData.vertexCount + 1);
                meshIndexArray.push_back(meshData.vertexCount + 2);

                meshData.vertexCount += 3;
                meshData.indexCount += 3;
            }
        }

        return true;
    }

    void MeshStores::NormalizeMeshSize(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray)
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

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            meshVertexArray[meshVeretxArrayIndex].position /= biggestDimension;
        }

        meshData.maxVertexPosition /= biggestDimension;
        meshData.minVertexPosition /= biggestDimension;
    }

    void MeshStores::RecenterMesh(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray)
    {
        glm::vec3 center = (meshData.maxVertexPosition + meshData.minVertexPosition) / 2.0f;
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            meshVertexArray[meshVeretxArrayIndex].position -= center;
        }

        meshData.maxVertexPosition -= center;
        meshData.minVertexPosition -= center;
    }

    void MeshStores::TranslateMesh(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        glm::vec3 translation)
    {
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            meshVertexArray[meshVeretxArrayIndex].position += translation;
        }

        meshData.maxVertexPosition += translation;
        meshData.minVertexPosition += translation;
    }

    void MeshStores::RotateMesh(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        glm::quat rotation)
    {
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            glm::vec3& position = meshVertexArray[meshVeretxArrayIndex].position;
            glm::vec3& normal = meshVertexArray[meshVeretxArrayIndex].normal;

            position = rotation * position;
            normal = rotation * normal;
        }

        if (meshData.vertexCount > 0)
        {
            const MeshVertex& currentMeshVertex = meshVertexArray[meshData.vertexIndex];

            meshData.maxVertexPosition.x = currentMeshVertex.position.x;
            meshData.maxVertexPosition.y = currentMeshVertex.position.y;
            meshData.maxVertexPosition.z = currentMeshVertex.position.z;
            meshData.minVertexPosition.x = currentMeshVertex.position.x;
            meshData.minVertexPosition.y = currentMeshVertex.position.y;
            meshData.minVertexPosition.z = currentMeshVertex.position.z;
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            const MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentMeshVertex.position.x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, currentMeshVertex.position.y);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, currentMeshVertex.position.z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentMeshVertex.position.x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, currentMeshVertex.position.y);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, currentMeshVertex.position.z);
        }
    }

    void MeshStores::RotateMeshX(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        RotateMesh(meshData, meshVertexArray, rotation);
    }

    void MeshStores::RotateMeshY(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        RotateMesh(meshData, meshVertexArray, rotation);
    }

    void MeshStores::RotateMeshZ(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        float rotationInRadians)
    {
        glm::quat rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        RotateMesh(meshData, meshVertexArray, rotation);
    }

    void MeshStores::ScaleMesh(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        glm::vec3 scale)
    {
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            meshVertexArray[meshVeretxArrayIndex].position *= scale;
        }

        meshData.maxVertexPosition *= scale;
        meshData.minVertexPosition *= scale;
    }

    void MeshStores::SizeMeshAlongNormals(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        float size)
    {
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];
            currentMeshVertex.position += currentMeshVertex.normal * size;
        }

        if (meshData.vertexCount > 0)
        {
            const MeshVertex& currentMeshVertex = meshVertexArray[meshData.vertexIndex];

            meshData.maxVertexPosition.x = currentMeshVertex.position.x;
            meshData.maxVertexPosition.y = currentMeshVertex.position.y;
            meshData.maxVertexPosition.z = currentMeshVertex.position.z;
            meshData.minVertexPosition.x = currentMeshVertex.position.x;
            meshData.minVertexPosition.y = currentMeshVertex.position.y;
            meshData.minVertexPosition.z = currentMeshVertex.position.z;
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            unsigned int meshVeretxArrayIndex = meshData.vertexIndex + (unsigned int)i;
            const MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

            meshData.maxVertexPosition.x = std::max(meshData.maxVertexPosition.x, currentMeshVertex.position.x);
            meshData.maxVertexPosition.y = std::max(meshData.maxVertexPosition.y, currentMeshVertex.position.y);
            meshData.maxVertexPosition.z = std::max(meshData.maxVertexPosition.z, currentMeshVertex.position.z);

            meshData.minVertexPosition.x = std::min(meshData.minVertexPosition.x, currentMeshVertex.position.x);
            meshData.minVertexPosition.y = std::min(meshData.minVertexPosition.y, currentMeshVertex.position.y);
            meshData.minVertexPosition.z = std::min(meshData.minVertexPosition.z, currentMeshVertex.position.z);
        }
    }

    void MeshStores::TurnInsideOut(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        bool wasTriangulated)
    {
        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            size_t index = i + (size_t)meshData.vertexIndex;
            meshVertexArray[index].normal *= -1.0f;
        }

        if (wasTriangulated)
        {
            for (size_t i = 0; i < meshData.vertexCount / 2; ++i)
            {
                size_t vertexIndex0 = (size_t)meshData.vertexIndex + i;
                size_t vertexIndex1 = (size_t)meshData.vertexIndex + (size_t)meshData.vertexCount - 1 - i;

                MeshVertex& meshVertex0 = meshVertexArray[vertexIndex0];
                MeshVertex& meshVertex1 = meshVertexArray[vertexIndex1];

                MeshVertex temp = meshVertex0;
                meshVertex0 = meshVertex1;
                meshVertex1 = temp;
            }

            for (size_t i = 0; i < meshData.indexCount; ++i)
            {
                size_t indexIndex = (size_t)meshData.indexIndex + i;
                unsigned int& currentIndex = meshIndexArray[indexIndex];
                currentIndex = meshData.indexCount - 1 - currentIndex;
            }
        }

        for (size_t i = 0; i < meshData.indexCount - 2; i += 3)
        {
            size_t index = i + (size_t)meshData.indexIndex;
            unsigned int& index0 = meshIndexArray[index];
            unsigned int& index1 = meshIndexArray[index + 2];

            unsigned int temp = index0;
            index0 = index1;
            index1 = temp;
        }
    }

    // protected ---------------------------------------------------------------

    bool MeshStores::GetMeshVertexFromFaceVertex(
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

    glm::vec2 MeshStores::GetLineLineIntersection2d(
        const glm::vec2& point1,
        const float& slope1,
        const glm::vec2& point2,
        const float& slope2)
    {
        glm::vec2 intersectionPoint;

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
            intersectionPoint.x = NAN;
            intersectionPoint.y = NAN;
        }
        else if (slope1 == INFINITY || slope1 == -INFINITY)
        {
            x = x1;
            y = m2 * (x1 - x2) + y2;
        }
        else if (slope2 == INFINITY || slope2 == -INFINITY)
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
}