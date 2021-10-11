#include "MeshStores.h"

#include <fstream>

#include "Engine/GeometryFunctions.h"



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
        MeshVertex*& firstVertexPtr,
        unsigned int& vertexCount,
        unsigned int*& firstIndexPtr,
        unsigned int& indexCount)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        MeshVertex* meshVertexArrayPtr = meshVertexArray_.data();

        firstVertexPtr = &meshVertexArrayPtr[meshData.vertexIndex];
        vertexCount = meshData.vertexCount;

        unsigned int* meshIndexArrayPtr = meshIndexArray_.data();

        firstIndexPtr = &meshIndexArrayPtr[meshData.indexIndex];
        indexCount = meshData.indexCount;

        return true;
    }

    bool MeshStores::GetMeshData(
        unsigned int index,
        MeshData& meshData)
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

    bool MeshStores::Generate2DLine(
        unsigned int& index,
        const std::vector<glm::vec2>& positions,
        float width,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DLine(newMeshData, meshVertexArray_, meshIndexArray_, positions, width, recenter, triangulate, positionalTexture))
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

    bool MeshStores::GenerateBox(
        unsigned int& index,
        float xLength,
        float yLength,
        float zLength,
        bool triangulate)
    {
        MeshData newMeshData;
        if (GenerateBox(newMeshData, meshVertexArray_, meshIndexArray_, xLength, yLength, zLength, triangulate))
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
                    std::vector<FaceVertex> face;

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
                            FaceVertex newFaceVertex;
                            newFaceVertex.positionIndex = std::stoi(indexGroup);

                            face.push_back(newFaceVertex);
                        }
                        else if (slashIndicies.size() == 1)
                        {
                            const size_t& slashIndex = slashIndicies[0];

                            FaceVertex newFaceVertex;

                            if (slashIndex > 0)
                            {
                                std::string positionIndexString = indexGroup.substr(0, slashIndex);
                                newFaceVertex.positionIndex = std::stoi(positionIndexString);
                            }

                            if (indexGroup.size() > slashIndex + 1)
                            {
                                std::string textureCoordinateIndexString = indexGroup.substr(slashIndex + 1);
                                newFaceVertex.textureCoordinateIndex = std::stoi(textureCoordinateIndexString);
                            }

                            face.push_back(newFaceVertex);
                        }
                        else if (slashIndicies.size() == 2)
                        {
                            const size_t& firstSlashIndex = slashIndicies[0];
                            const size_t& secondSlashIndex = slashIndicies[1];

                            FaceVertex newFaceVertex;

                            if (firstSlashIndex > 0)
                            {
                                std::string positionIndexString = indexGroup.substr(0, firstSlashIndex);
                                newFaceVertex.positionIndex = std::stoi(positionIndexString);
                            }

                            if (secondSlashIndex > firstSlashIndex + 1)
                            {
                                std::string textureCoordinateIndexString = indexGroup.substr(firstSlashIndex + 1, secondSlashIndex - firstSlashIndex - 1);
                                newFaceVertex.textureCoordinateIndex = std::stoi(textureCoordinateIndexString);
                            }

                            if (indexGroup.size() > secondSlashIndex + 1)
                            {
                                std::string normalIndexString = indexGroup.substr(secondSlashIndex + 1);
                                newFaceVertex.normalIndex = std::stoi(normalIndexString);
                            }

                            face.push_back(newFaceVertex);
                        }
                    }

                    if (face.size() >= 3)
                    {
                        if (!triangulate)
                        {
                            for (size_t i = 0; i < face.size(); ++i)
                            {
                                FaceVertex& faceVertex = face[i];
                                MeshVertex meshVertex;
                                GetMeshVertexFromFaceVertex(meshVertex, faceVertex, positions, textureCoordinates, normals);
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
                                FaceVertex& face0 = face[0];
                                FaceVertex& face1 = face[i];
                                FaceVertex& face2 = face[i + 1];

                                MeshVertex meshVertex0;
                                GetMeshVertexFromFaceVertex(meshVertex0, face0, positions, textureCoordinates, normals);
                                meshVertexArray.push_back(meshVertex0);
                                meshIndexArray.push_back(meshData.indexCount++);

                                MeshVertex meshVertex1;
                                GetMeshVertexFromFaceVertex(meshVertex1, face1, positions, textureCoordinates, normals);
                                meshVertexArray.push_back(meshVertex1);
                                meshIndexArray.push_back(meshData.indexCount++);

                                MeshVertex meshVertex2;
                                GetMeshVertexFromFaceVertex(meshVertex2, face2, positions, textureCoordinates, normals);
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

    bool MeshStores::Generate2DLine(
        MeshData& meshData,
        std::vector<MeshVertex>& meshVertexArray,
        std::vector<unsigned int>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        float width,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        if (positions.size() < 2)
        {
            return false;
        }

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 position1 = positions[0];
        glm::vec2 position2 = positions[1];

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

        for (size_t i = 2; i < positions.size(); ++i)
        {
            glm::vec2 position3 = positions[i];
            glm::vec2 direction2 = position3 - position2;
            float slope2 = direction2.y / direction2.x;

            if (slope1 != slope2)
            {
                bool sharpCorner = false;
                float angle = Get2DVectorAngle(direction1, direction2);
                if (angle > glm::pi<float>() / 1.9999999f || angle < glm::pi<float>() / -1.9999999f)
                {
                    sharpCorner = true;
                }

                glm::vec2 offset02 = glm::vec2(-1.0f * direction2.y, direction2.x);
                float magnitude02 = std::sqrtf(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                glm::vec2 positionE = position2 + scaled2;
                glm::vec2 positionF = position2 - scaled2;

                if (sharpCorner)
                {
                    offset01 = glm::vec2(-1.0f * direction1.y, direction1.x);
                    magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        positionC = Get2DLineIntersection(positionA, slope1, positionE, slope2);
                        positionD = position2 - scaled1;
                    }
                    else
                    {
                        positionC = position2 + scaled1;
                        positionD = Get2DLineIntersection(positionB, slope1, positionF, slope2);
                    }
                }
                else
                {
                    positionC = Get2DLineIntersection(positionA, slope1, positionE, slope2);
                    positionD = Get2DLineIntersection(positionB, slope1, positionF, slope2);
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

        if (positionalTexture)
        {
            std::vector<glm::vec2> textureCoordinates;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                textureCoordinates.emplace_back(positions[i].x + 0.5f, positions[i].y + 0.5f);
            }

            Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, textureCoordinates, recenter, triangulate);
        }
        else
        {
            Generate2DTriangleFan(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), recenter, triangulate);
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

        if (positionalTexture)
        {
            std::vector<glm::vec2> textureCoordinates;
            for (size_t i = 0; i < positions.size(); ++i)
            {
                textureCoordinates.emplace_back(positions[i].x + 0.5f, positions[i].y + 0.5f);
            }

            Generate2DTriangleStrip(meshData, meshVertexArray, meshIndexArray, positions, textureCoordinates, recenter, triangulate);
        }
        else
        {
            Generate2DTriangleStrip(meshData, meshVertexArray, meshIndexArray, positions, std::vector<glm::vec2>(), recenter, triangulate);
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

        glm::vec3 normal_posX( 1.0f,  0.0f,  0.0f);
        glm::vec3 normal_negX(-1.0f,  0.0f,  0.0f);
        glm::vec3 normal_posY( 0.0f,  1.0f,  0.0f);
        glm::vec3 normal_negY( 0.0f, -1.0f,  0.0f);
        glm::vec3 normal_posZ( 0.0f,  0.0f,  1.0f);
        glm::vec3 normal_negZ( 0.0f,  0.0f, -1.0f);

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

        meshData.vertexIndex = (unsigned int)meshVertexArray.size();
        meshData.indexIndex = (unsigned int)meshIndexArray.size();
        meshData.maxVertexPosition.x = xLength /  2.0f;
        meshData.minVertexPosition.x = xLength / -2.0f;
        meshData.maxVertexPosition.y = yLength /  2.0f;
        meshData.minVertexPosition.x = yLength / -2.0f;
        meshData.maxVertexPosition.z = zLength /  2.0f;
        meshData.minVertexPosition.x = zLength / -2.0f;

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

        return true;
    }

    // TODO:
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

        // top
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>() * 0.5f, 0.0f);
            textureCoordinates.emplace_back((float)( i * 2 + 1), 1.0f);
        }

        const float hAngle = 0.4f * glm::pi<float>();
        const float vAngle = std::atan(0.5f);

        // body
        for (size_t i = 0; i < 6; ++i)
        {
            positions.emplace_back(radius, vAngle, (float)i * hAngle);
            textureCoordinates.emplace_back((float)(i * 2), 2.0f / 3.0f);

            positions.emplace_back(radius, -1.0f * vAngle, ((float)i + 0.5) * hAngle);
            textureCoordinates.emplace_back((float)(i * 2 + 1), 1.0f / 3.0f);
        }

        // bottom
        for (size_t i = 0; i < 5; ++i)
        {
            positions.emplace_back(radius, glm::pi<float>() * -0.5f, 0.0f);
            textureCoordinates.emplace_back((float)(i * 2 + 2), 0.0f);
        }

        // subdivide

        return false;
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

            for (size_t j = 0; j < longitudinalSections + 1; j++) // +1 for wrap around
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

                    meshData.vertexCount += 3;
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

                        meshData.vertexCount += 6;
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

                    meshData.vertexCount += 3;
                }

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

                        meshIndexArray.push_back((unsigned int)i);
                        meshIndexArray.push_back((unsigned int)i2);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i4);
                        meshIndexArray.push_back((unsigned int)i);

                        meshData.indexCount += 6;
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

                    meshData.indexCount += 3;
                }
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

                meshData.vertexCount += 3;

                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
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
                    
                        meshData.vertexCount += 4;

                        meshIndexArray.push_back((unsigned int)i);
                        meshIndexArray.push_back((unsigned int)i2);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i);
                        meshIndexArray.push_back((unsigned int)i3);
                        meshIndexArray.push_back((unsigned int)i4);

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

                meshData.vertexCount += 3;

                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
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

    // protected ---------------------------------------------------------------

    bool MeshStores::GetMeshVertexFromFaceVertex(
        MeshVertex& meshVertex,
        const FaceVertex& faceVertex,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        const std::vector<glm::vec3>& normals)
    {
        bool success = true;

        if (faceVertex.positionIndex > 0 && faceVertex.positionIndex <= positions.size())
        {
            int positionIndex = faceVertex.positionIndex - 1;
            meshVertex.position = positions[positionIndex];
        }
        else if (faceVertex.positionIndex < 0 && faceVertex.positionIndex >= positions.size() * -1)
        {
            int positionIndex = (int)(faceVertex.positionIndex + positions.size());
            meshVertex.position = positions[positionIndex];
        }
        else
        {
            success = false;
        }

        if (faceVertex.textureCoordinateIndex > 0 && faceVertex.textureCoordinateIndex <= textureCoordinates.size())
        {
            int textureCoordinateIndex = faceVertex.textureCoordinateIndex - 1;
            meshVertex.textureCoordinate = textureCoordinates[textureCoordinateIndex];
        }
        else if (faceVertex.textureCoordinateIndex < 0 && faceVertex.textureCoordinateIndex >= textureCoordinates.size() * -1)
        {
            int textureCoordinateIndex = (int)(faceVertex.textureCoordinateIndex + textureCoordinates.size());
            meshVertex.textureCoordinate = textureCoordinates[textureCoordinateIndex];
        }
        else
        {
            success = false;
        }

        if (faceVertex.normalIndex > 0 && faceVertex.normalIndex <= normals.size())
        {
            int normalIndex = faceVertex.normalIndex - 1;
            meshVertex.normal = normals[normalIndex];
        }
        else if (faceVertex.normalIndex < 0 && faceVertex.normalIndex >= normals.size() * -1)
        {
            int normalIndex = (int)(faceVertex.normalIndex + normals.size());
            meshVertex.normal = normals[normalIndex];
        }
        else
        {
            success = false;
        }

        return success;
    }
}