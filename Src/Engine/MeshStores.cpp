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
        glm::uint index,
        MeshVertex*& firstVertexPtr,
        glm::uint& vertexCount,
        glm::uint*& firstIndexPtr,
        glm::uint& indexCount)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        MeshVertex* meshVertexArrayPtr = meshVertexArray_.data();

        firstVertexPtr = &meshVertexArrayPtr[meshData.vertexIndex];
        vertexCount = meshData.vertexCount;

        glm::uint* meshIndexArrayPtr = meshIndexArray_.data();

        firstIndexPtr = &meshIndexArrayPtr[meshData.indexIndex];
        indexCount = meshData.indexCount;

        return true;
    }

    bool MeshStores::GetMeshData(
        glm::uint index,
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
        glm::uint& index,
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
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangleFan(
        glm::uint& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DTriangleFan(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangles(
        glm::uint& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter)
    {
        MeshData newMeshData;
        if (Generate2DTriangles(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter))
        {
            meshDataArray_.push_back(newMeshData);
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DTriangleStrip(
        glm::uint& index,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        MeshData newMeshData;
        if (Generate2DTriangleStrip(newMeshData, meshVertexArray_, meshIndexArray_, positions, textureCoordinates, recenter, triangulate))
        {
            meshDataArray_.push_back(newMeshData);
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DLine(
        glm::uint& index,
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
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DRegularPolygon(
        glm::uint& index,
        float radius,
        glm::uint sides,
        bool recenter,
        bool triangulate,
        bool positionalTexture)
    {
        MeshData newMeshData;
        if (Generate2DRegularPolygon(newMeshData, meshVertexArray_, meshIndexArray_, radius, sides, recenter, triangulate, positionalTexture))
        {
            meshDataArray_.push_back(newMeshData);
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::Generate2DArc(
        glm::uint& index,
        float innerRadius,
        float outerRadius,
        glm::uint subdivisions,
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
            index = (glm::uint)(meshDataArray_.size() - 1);

            return true;
        }

        return false;
    }

    bool MeshStores::NormalizeMeshSize(glm::uint& index)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        NormalizeMeshSize(meshData, meshVertexArray_);

        return true;
    }

    bool MeshStores::RecenterMesh(glm::uint& index)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RecenterMesh(meshData, meshVertexArray_);

        return true;
    }

    bool MeshStores::TranslateMesh(glm::uint& index, glm::vec3 translation)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        TranslateMesh(meshData, meshVertexArray_, translation);

        return true;
    }

    bool MeshStores::RotateMesh(glm::uint& index, glm::quat rotation)
    {
        if (index >= meshDataArray_.size())
        {
            return false;
        }

        MeshData& meshData = meshDataArray_[index];
        RotateMesh(meshData, meshVertexArray_, rotation);

        return true;
    }

    bool MeshStores::ScaleMesh(glm::uint& index, glm::vec3 scale)
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
        std::vector<glm::uint>& meshIndexArray,
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

        meshData.vertexIndex = (glm::uint)meshVertexArray.size();
        meshData.indexIndex = (glm::uint)meshIndexArray.size();

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
                                meshIndexArray.push_back(meshData.vertexCount + (glm::uint)i);
                                meshIndexArray.push_back(meshData.vertexCount + (glm::uint)i + 1);
                            }

                            meshData.vertexCount += (glm::uint)face.size();
                            meshData.indexCount += (glm::uint)((face.size() - 2) * 3);
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

                            meshData.vertexCount += (glm::uint)((face.size() - 2) * 3);
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
        std::vector<glm::uint>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        meshData.vertexIndex = (glm::uint)meshVertexArray.size();
        meshData.indexIndex = (glm::uint)meshIndexArray.size();
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
                    meshVertex.textureCoordinte = textureCoordinates[i];
                }

                meshVertex.normal = normal;
                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 1; i < positions.size() - 1; ++i)
            {
                meshIndexArray.push_back(0);
                meshIndexArray.push_back((glm::uint)i);
                meshIndexArray.push_back((glm::uint)i + 1);
            }

            meshData.vertexCount = (glm::uint)positions.size();
            meshData.indexCount += (glm::uint)((positions.size() - 2) * 3);
        }
        else
        {
            for (size_t i = 1; i < positions.size() - 1; ++i)
            {
                MeshVertex centerMeshVertex;
                centerMeshVertex.position = glm::vec3(positions[0], 0.0f);

                if (0 < textureCoordinates.size())
                {
                    centerMeshVertex.textureCoordinte = textureCoordinates[0];
                }

                centerMeshVertex.normal = normal;

                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertexA.textureCoordinte = textureCoordinates[i];
                }

                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);

                if (i + 1 < textureCoordinates.size())
                {
                    meshVertexB.textureCoordinte = textureCoordinates[i + 1];
                }

                meshVertexB.normal = normal;

                meshVertexArray.push_back(centerMeshVertex);
                meshVertexArray.push_back(meshVertexA);
                meshVertexArray.push_back(meshVertexB);

                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
                meshIndexArray.push_back(meshData.indexCount++);
            }

            meshData.vertexCount += (glm::uint)((positions.size() - 2) * 3);
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
        std::vector<glm::uint>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter)
    {
        if (positions.size() == 0 || positions.size() % 3 != 0)
        {
            return false;
        }

        meshData.vertexIndex = (glm::uint)meshVertexArray.size();
        meshData.vertexCount = (glm::uint)positions.size();
        meshData.indexIndex = (glm::uint)meshIndexArray.size();
        meshData.indexCount = (glm::uint)positions.size();
        meshData.maxVertexPosition.z = 0.0f;
        meshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        for (size_t i = 0; i < positions.size(); ++i)
        {
            MeshVertex meshVertex;
            meshVertex.position = glm::vec3(positions[i], 0.0f);

            if (i < textureCoordinates.size())
            {
                meshVertex.textureCoordinte = textureCoordinates[i];
            }

            meshVertex.normal = normal;

            meshVertexArray.push_back(meshVertex);
            meshIndexArray.push_back((glm::uint)i);
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
        std::vector<glm::uint>& meshIndexArray,
        const std::vector<glm::vec2>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        bool recenter,
        bool triangulate)
    {
        if (positions.size() < 3)
        {
            return false;
        }

        meshData.vertexIndex = (glm::uint)meshVertexArray.size();
        meshData.indexIndex = (glm::uint)meshIndexArray.size();
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
                    meshVertex.textureCoordinte = textureCoordinates[i];
                }

                meshVertex.normal = normal;

                meshVertexArray.push_back(meshVertex);
            }

            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                if (i % 2 == 0)
                {
                    meshIndexArray.push_back((glm::uint)i);
                    meshIndexArray.push_back((glm::uint)i + 1);
                    meshIndexArray.push_back((glm::uint)i + 2);
                }
                else
                {
                    meshIndexArray.push_back((glm::uint)i + 2);
                    meshIndexArray.push_back((glm::uint)i + 1);
                    meshIndexArray.push_back((glm::uint)i);
                }
            }

            meshData.vertexCount = (glm::uint)positions.size();
            meshData.indexCount += (glm::uint)((positions.size() - 2) * 3);
        }
        else
        {
            for (size_t i = 0; i < positions.size() - 2; ++i)
            {
                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(positions[i], 0.0f);

                if (i < textureCoordinates.size())
                {
                    meshVertexA.textureCoordinte = textureCoordinates[i];
                }

                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                meshVertexB.position = glm::vec3(positions[i + 1], 0.0f);

                if (i + 1 < textureCoordinates.size())
                {
                    meshVertexB.textureCoordinte = textureCoordinates[i + 1];
                }

                meshVertexB.normal = normal;

                MeshVertex meshVertexC;
                meshVertexC.position = glm::vec3(positions[i + 2], 0.0f);

                if (i + 2 < textureCoordinates.size())
                {
                    meshVertexC.textureCoordinte = textureCoordinates[i + 2];
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

            meshData.vertexCount += (glm::uint)((positions.size() - 2) * 3);
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
        std::vector<glm::uint>& meshIndexArray,
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

        meshData.vertexIndex = (glm::uint)meshVertexArray.size();
        meshData.indexIndex = (glm::uint)meshIndexArray.size();
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
                glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
                MeshVertex& currentMeshVertex = meshVertexArray[meshVeretxArrayIndex];

                currentMeshVertex.textureCoordinte.x = currentMeshVertex.position.x;
                currentMeshVertex.textureCoordinte.y = currentMeshVertex.position.y;
            }
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
        std::vector<glm::uint>& meshIndexArray,
        float radius,
        glm::uint sides,
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

        for (size_t i = 0; i < (size_t)sides; ++i)
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
        std::vector<glm::uint>& meshIndexArray,
        float innerRadius,
        float outerRadius,
        glm::uint subdivisions,
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

        for (size_t i = 0; i < (size_t)subdivisions; ++i)
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
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
            glm::vec3& position = meshVertexArray[meshVeretxArrayIndex].position;
            glm::vec3& normal = meshVertexArray[meshVeretxArrayIndex].normal;

            position = rotation * position;
            normal = rotation * normal;
        }

        for (size_t i = 0; i < meshData.vertexCount; ++i)
        {
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
            glm::uint meshVeretxArrayIndex = meshData.vertexIndex + (glm::uint)i;
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
            meshVertex.textureCoordinte = textureCoordinates[textureCoordinateIndex];
        }
        else if (faceVertex.textureCoordinateIndex < 0 && faceVertex.textureCoordinateIndex >= textureCoordinates.size() * -1)
        {
            int textureCoordinateIndex = (int)(faceVertex.textureCoordinateIndex + textureCoordinates.size());
            meshVertex.textureCoordinte = textureCoordinates[textureCoordinateIndex];
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