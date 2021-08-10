#include "MeshStores.h"

#include <fstream>

#include "Engine/MathFunctions.h"
#include "Engine/MathConstants.h"



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

    bool MeshStores::GetMesh(glm::uint index, MeshVertex*& firstVertexPtr, glm::uint& vertexCount, glm::uint*& firstIndexPtr, glm::uint& indexCount)
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

    bool MeshStores::LoadMesh(const std::string& path, glm::uint& index, bool normalizeSize, bool recenter, bool triangulate)
    {
        // Should the file path be required to have the .obj extension?
        // if (path.size() < 4 || path.substr(path.size() - 4, 4) != ".obj")
        // {
        //     return false;
        // }

        std::ifstream file(path);

        if (!file.is_open())
        {
            return false;
        }

        MeshData newMeshData;
        newMeshData.vertexIndex = (glm::uint)meshVertexArray_.size();
        newMeshData.indexIndex = (glm::uint)meshIndexArray_.size();

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

                    newMeshData.maxVertexPosition.x = std::max(newMeshData.maxVertexPosition.x, newPosition.x);
                    newMeshData.maxVertexPosition.y = std::max(newMeshData.maxVertexPosition.y, newPosition.y);
                    newMeshData.maxVertexPosition.z = std::max(newMeshData.maxVertexPosition.z, newPosition.z);

                    newMeshData.minVertexPosition.x = std::min(newMeshData.minVertexPosition.x, newPosition.x);
                    newMeshData.minVertexPosition.y = std::min(newMeshData.minVertexPosition.y, newPosition.y);
                    newMeshData.minVertexPosition.z = std::min(newMeshData.minVertexPosition.z, newPosition.z);

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

                    for (glm::uint i = 0; i < splitValues.size(); ++i)
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

                        if (slashIndicies.size() == 2)
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
                            for (glm::uint i = 0; i < face.size(); ++i)
                            {
                                FaceVertex& faceVertex = face[i];
                                MeshVertex meshVertex = GetMeshVertexFromFaceVertex(faceVertex, positions, textureCoordinates, normals);
                                meshVertexArray_.push_back(meshVertex);
                            }

                            for (glm::uint i = 1; i < face.size() - 1; ++i)
                            {
                                meshIndexArray_.push_back(newMeshData.vertexCount);
                                meshIndexArray_.push_back(newMeshData.vertexCount + i);
                                meshIndexArray_.push_back(newMeshData.vertexCount + i + 1);
                            }

                            newMeshData.vertexCount += (glm::uint)face.size();
                            newMeshData.indexCount += (glm::uint)((face.size() - 2) * 3);
                        }
                        else
                        {
                            for (glm::uint i = 1; i < face.size() - 1; ++i)
                            {
                                FaceVertex& face0 = face[0];
                                FaceVertex& face1 = face[i];
                                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                                FaceVertex& face2 = face[i_plus_1];

                                MeshVertex meshVertex = GetMeshVertexFromFaceVertex(face0, positions, textureCoordinates, normals);
                                meshVertexArray_.push_back(meshVertex);
                                meshIndexArray_.push_back(newMeshData.indexCount++);

                                meshVertex = GetMeshVertexFromFaceVertex(face1, positions, textureCoordinates, normals);
                                meshVertexArray_.push_back(meshVertex);
                                meshIndexArray_.push_back(newMeshData.indexCount++);

                                meshVertex = GetMeshVertexFromFaceVertex(face2, positions, textureCoordinates, normals);
                                meshVertexArray_.push_back(meshVertex);
                                meshIndexArray_.push_back(newMeshData.indexCount++);
                            }

                            newMeshData.vertexCount += (glm::uint)((face.size() - 2) * 3);
                        }
                    }
                }
            }
        }

        if (recenter)
        {
            glm::vec3 center = (newMeshData.maxVertexPosition + newMeshData.minVertexPosition) / 2.0f;
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position -= center;
            }
            newMeshData.maxVertexPosition -= center;
            newMeshData.minVertexPosition -= center;
        }

        if (normalizeSize)
        {
            glm::vec3 size = newMeshData.maxVertexPosition - newMeshData.minVertexPosition;
            float biggestDimension = size.x;
            if (size.y > size.x && size.y > size.z)
            {
                biggestDimension = size.y;
            }
            else if (size.z > size.x && size.z > size.y)
            {
                biggestDimension = size.z;
            }
            
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position /= biggestDimension;
            }
        }

        if (newMeshData.vertexCount == 0 && newMeshData.indexCount == 0)
        {
            return false;
        }

        meshDataArray_.push_back(newMeshData);
        index = (glm::uint)(meshDataArray_.size() - 1);

        return true;
    }

    bool MeshStores::Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter, bool triangulate)
    {
        if (vertices.size() < 3)
        {
            return false;
        }

        MeshData newMeshData;
        newMeshData.vertexIndex = (glm::uint)meshVertexArray_.size();
        newMeshData.indexIndex = (glm::uint)meshIndexArray_.size();
        newMeshData.maxVertexPosition.z = 0.0f;
        newMeshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (glm::uint i = 0; i < vertices.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(vertices[i], 0.0f);
                meshVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertex.normal = normal;
                meshVertexArray_.push_back(meshVertex);
            }

            for (glm::uint i = 1; i < vertices.size() - 1; ++i)
            {
                meshIndexArray_.push_back(0);

                meshIndexArray_.push_back(i);

                glm::uint i_plus_1 = i + 1;
                meshIndexArray_.push_back(i_plus_1);
            }

            newMeshData.vertexCount = (glm::uint)vertices.size();
            newMeshData.indexCount += (glm::uint)((vertices.size() - 2) * 3);
        }
        else
        {
            for (glm::uint i = 1; i < vertices.size() - 1; ++i)
            {
                MeshVertex centerMeshVertex;
                centerMeshVertex.position = glm::vec3(vertices[0], 0.0f);
                centerMeshVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                centerMeshVertex.normal = normal;
                meshVertexArray_.push_back(centerMeshVertex);
                meshIndexArray_.push_back(newMeshData.indexCount++);

                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(vertices[i], 0.0f);
                meshVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertexA.normal = normal;
                meshVertexArray_.push_back(meshVertexA);
                meshIndexArray_.push_back(newMeshData.indexCount++);

                MeshVertex meshVertexB;
                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                meshVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                meshVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertexB.normal = normal;
                meshVertexArray_.push_back(meshVertexB);
                meshIndexArray_.push_back(newMeshData.indexCount++);
            }

            newMeshData.vertexCount += (glm::uint)((vertices.size() - 2) * 3);
        }

        for (glm::uint i = 0; i < vertices.size(); ++i)
        {
            newMeshData.maxVertexPosition.x = std::max(newMeshData.maxVertexPosition.x, vertices[i].x);
            newMeshData.maxVertexPosition.y = std::max(newMeshData.maxVertexPosition.y, vertices[i].y);

            newMeshData.minVertexPosition.x = std::min(newMeshData.minVertexPosition.x, vertices[i].x);
            newMeshData.minVertexPosition.y = std::min(newMeshData.minVertexPosition.y, vertices[i].y);
        }

        if (recenter)
        {
            glm::vec3 center = (newMeshData.maxVertexPosition + newMeshData.minVertexPosition) / 2.0f;
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position -= center;
            }
            newMeshData.maxVertexPosition -= center;
            newMeshData.minVertexPosition -= center;
        }

        meshDataArray_.push_back(newMeshData);
        index = (glm::uint)(meshDataArray_.size() - 1);

        return true;
    }

    bool MeshStores::Generate2DTriangles(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter)
    {
        if (vertices.size() == 0 || vertices.size() % 3 != 0)
        {
            return false;
        }

        MeshData newMeshData;
        newMeshData.vertexIndex = (glm::uint)meshVertexArray_.size();
        newMeshData.vertexCount = (glm::uint)vertices.size();
        newMeshData.indexIndex = (glm::uint)meshIndexArray_.size();
        newMeshData.indexCount = (glm::uint)vertices.size();
        newMeshData.maxVertexPosition.z = 0.0f;
        newMeshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        for (glm::uint i = 0; i < vertices.size(); ++i)
        {
            MeshVertex meshVertex;
            meshVertex.position = glm::vec3(vertices[i], 0.0f);
            meshVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
            meshVertex.normal = normal;

            meshVertexArray_.push_back(meshVertex);
            meshIndexArray_.push_back(i);
        }

        for (glm::uint i = 0; i < vertices.size(); ++i)
        {
            newMeshData.maxVertexPosition.x = std::max(newMeshData.maxVertexPosition.x, vertices[i].x);
            newMeshData.maxVertexPosition.y = std::max(newMeshData.maxVertexPosition.y, vertices[i].y);

            newMeshData.minVertexPosition.x = std::min(newMeshData.minVertexPosition.x, vertices[i].x);
            newMeshData.minVertexPosition.y = std::min(newMeshData.minVertexPosition.y, vertices[i].y);
        }

        if (recenter)
        {
            glm::vec3 center = (newMeshData.maxVertexPosition + newMeshData.minVertexPosition) / 2.0f;
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position -= center;
            }
            newMeshData.maxVertexPosition -= center;
            newMeshData.minVertexPosition -= center;
        }

        meshDataArray_.push_back(newMeshData);
        index = (glm::uint)(meshDataArray_.size() - 1);

        return true;
    }

    bool MeshStores::Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, glm::uint& index, bool recenter, bool triangulate)
    {
        if (vertices.size() < 3)
        {
            return false;
        }

        MeshData newMeshData;
        newMeshData.vertexIndex = (glm::uint)meshVertexArray_.size();
        newMeshData.indexIndex = (glm::uint)meshIndexArray_.size();
        newMeshData.maxVertexPosition.z = 0.0f;
        newMeshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (glm::uint i = 0; i < vertices.size(); ++i)
            {
                MeshVertex meshVertex;
                meshVertex.position = glm::vec3(vertices[i], 0.0f);
                meshVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertex.normal = normal;
                meshVertexArray_.push_back(meshVertex);
            }

            for (glm::uint i = 0; i < vertices.size() - 2; ++i)
            {
                glm::uint i_plus_1 = i + 1;
                glm::uint i_plus_2 = i + 2;

                if (i % 2 == 0)
                {
                    meshIndexArray_.push_back(i);
                    meshIndexArray_.push_back(i_plus_1);
                    meshIndexArray_.push_back(i_plus_2);
                }
                else
                {
                    meshIndexArray_.push_back(i_plus_2);
                    meshIndexArray_.push_back(i_plus_1);
                    meshIndexArray_.push_back(i);
                }
            }

            newMeshData.vertexCount = (glm::uint)vertices.size();
            newMeshData.indexCount += (glm::uint)((vertices.size() - 2) * 3);
        }
        else
        {
            for (glm::uint i = 0; i < vertices.size() - 2; ++i)
            {
                MeshVertex meshVertexA;
                meshVertexA.position = glm::vec3(vertices[i], 0.0f);
                meshVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertexA.normal = normal;

                MeshVertex meshVertexB;
                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                meshVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                meshVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertexB.normal = normal;

                MeshVertex meshVertexC;
                glm::uint i_plus_2 = i + 2; // Doing this suppresses the C26451 warning in visual studio
                meshVertexC.position = glm::vec3(vertices[i_plus_2], 0.0f);
                meshVertexC.textureCoordinte = glm::vec2(0.0f, 0.0f);
                meshVertexC.normal = normal;

                if (i % 2 == 0)
                {
                    meshVertexArray_.push_back(meshVertexA);
                    meshVertexArray_.push_back(meshVertexB);
                    meshVertexArray_.push_back(meshVertexC);
                }
                else
                {
                    meshVertexArray_.push_back(meshVertexC);
                    meshVertexArray_.push_back(meshVertexB);
                    meshVertexArray_.push_back(meshVertexA);
                }

                meshIndexArray_.push_back(newMeshData.indexCount++);
                meshIndexArray_.push_back(newMeshData.indexCount++);
                meshIndexArray_.push_back(newMeshData.indexCount++);
            }

            newMeshData.vertexCount += (glm::uint)((vertices.size() - 2) * 3);
        }

        for (glm::uint i = 0; i < vertices.size(); ++i)
        {
            newMeshData.maxVertexPosition.x = std::max(newMeshData.maxVertexPosition.x, vertices[i].x);
            newMeshData.maxVertexPosition.y = std::max(newMeshData.maxVertexPosition.y, vertices[i].y);

            newMeshData.minVertexPosition.x = std::min(newMeshData.minVertexPosition.x, vertices[i].x);
            newMeshData.minVertexPosition.y = std::min(newMeshData.minVertexPosition.y, vertices[i].y);
        }

        if (recenter)
        {
            glm::vec3 center = (newMeshData.maxVertexPosition + newMeshData.minVertexPosition) / 2.0f;
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position -= center;
            }
            newMeshData.maxVertexPosition -= center;
            newMeshData.minVertexPosition -= center;
        }

        meshDataArray_.push_back(newMeshData);
        index = (glm::uint)(meshDataArray_.size() - 1);

        return true;
    }

    bool MeshStores::Generate2DLine(const std::vector<glm::vec2>& vertices, float width, glm::uint& index, bool recenter, bool triangulate)
    {
        if (vertices.size() < 2)
        {
            return false;
        }

        MeshData newMeshData;
        newMeshData.vertexIndex = (glm::uint)meshVertexArray_.size();
        newMeshData.indexIndex = (glm::uint)meshIndexArray_.size();
        newMeshData.maxVertexPosition.z = 0.0f;
        newMeshData.minVertexPosition.z = 0.0f;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        glm::vec2 vertex1 = vertices[0];
        glm::vec2 vertex2 = vertices[1];

        glm::vec2 direction1 = vertex2 - vertex1;

        float sin90 = std::sinf((float)M_PI_2);
        float cos90 = std::cosf((float)M_PI_2);

        glm::vec2 offset01(direction1.x * cos90 - direction1.y * sin90, direction1.x * sin90 + direction1.y * cos90);
        float magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
        glm::vec2 scaled1 = offset01 / magnitude01 * width / 2.0f;

        glm::vec2 scaled2 = scaled1;

        glm::vec2 vertexA = vertex1 + scaled1;
        glm::vec2 vertexB = vertex1 - scaled1;
        glm::vec2 vertexC;
        glm::vec2 vertexD;

        float slope1 = direction1.y / direction1.x;

        MeshVertex meshVertexA;
        meshVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
        meshVertexA.normal = normal;
        MeshVertex meshVertexB;
        meshVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
        meshVertexB.normal = normal;
        MeshVertex meshVertexC;
        meshVertexC.textureCoordinte = glm::vec2(0.0f, 0.0f);
        meshVertexC.normal = normal;
        MeshVertex meshVertexD;
        meshVertexD.textureCoordinte = glm::vec2(0.0f, 0.0f);
        meshVertexD.normal = normal;

        for (int i = 2; i < vertices.size(); ++i)
        {
            glm::vec2 vertex3 = vertices[i];
            glm::vec2 direction2 = vertex3 - vertex2;
            float slope2 = direction2.y / direction2.x;

            if (slope1 != slope2)
            {
                bool sharpCorner = false;
                float angle = Get2DVectorAngle(direction1, direction2);
                if (angle > M_PI / 1.9999999f || angle < M_PI / -1.9999999f)
                {
                    sharpCorner = true;
                }

                glm::vec2 offset02 = glm::vec2(direction2.x * cos90 - direction2.y * sin90, direction2.x * sin90 + direction2.y * cos90);
                float magnitude02 = std::sqrtf(offset02.x * offset02.x + offset02.y * offset02.y);
                scaled2 = offset02 / magnitude02 * width / 2.0f;

                glm::vec2 vertexE = vertex2 + scaled2;
                glm::vec2 vertexF = vertex2 - scaled2;

                if (sharpCorner)
                {
                    offset01 = glm::vec2(direction1.x * cos90 - direction1.y * sin90, direction1.x * sin90 + direction1.y * cos90);
                    magnitude01 = std::sqrtf(offset01.x * offset01.x + offset01.y * offset01.y);
                    scaled1 = offset01 / magnitude01 * width / 2.0f;

                    if (angle > 0.0f) // turn to the left
                    {
                        vertexC = Get2DLineIntersection(vertexA, slope1, vertexE, slope2);
                        vertexD = vertex2 - scaled1;
                    }
                    else
                    {
                        vertexC = vertex2 + scaled1;
                        vertexD = Get2DLineIntersection(vertexB, slope1, vertexF, slope2);
                    }
                }
                else
                {
                    vertexC = Get2DLineIntersection(vertexA, slope1, vertexE, slope2);
                    vertexD = Get2DLineIntersection(vertexB, slope1, vertexF, slope2);
                }

                meshVertexA.position = glm::vec3(vertexA, 0.0f);
                meshVertexB.position = glm::vec3(vertexB, 0.0f);
                meshVertexC.position = glm::vec3(vertexC, 0.0f);
                meshVertexD.position = glm::vec3(vertexD, 0.0f);

                glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);
                glm::vec3 normalDCB = glm::cross(meshVertexC.position - meshVertexD.position, meshVertexB.position - meshVertexD.position);

                float dotABC = glm::dot(normal, normalABC);
                float dotDCB = glm::dot(normal, normalDCB);

                if (!triangulate)
                {
                    meshVertexArray_.push_back(meshVertexA);
                    meshVertexArray_.push_back(meshVertexB);
                    meshVertexArray_.push_back(meshVertexC);
                    meshVertexArray_.push_back(meshVertexD);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                        meshIndexArray_.push_back(newMeshData.vertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount + 3);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                    }
                    else
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 3);
                    }

                    newMeshData.vertexCount += 4;
                    newMeshData.indexCount += 6;
                }
                else
                {
                    if (dotABC > 0.0f)
                    {
                        meshVertexArray_.push_back(meshVertexA);
                        meshVertexArray_.push_back(meshVertexB);
                        meshVertexArray_.push_back(meshVertexC);
                    }
                    // When the width of the line is too large, triangles can 
                    // flip.
                    else
                    {
                        meshVertexArray_.push_back(meshVertexC);
                        meshVertexArray_.push_back(meshVertexB);
                        meshVertexArray_.push_back(meshVertexA);
                    }

                    meshIndexArray_.push_back(newMeshData.indexCount++);
                    meshIndexArray_.push_back(newMeshData.indexCount++);
                    meshIndexArray_.push_back(newMeshData.indexCount++);

                    if (dotDCB > 0.0f)
                    {
                        meshVertexArray_.push_back(meshVertexD);
                        meshVertexArray_.push_back(meshVertexC);
                        meshVertexArray_.push_back(meshVertexB);
                    }
                    else
                    {
                        meshVertexArray_.push_back(meshVertexB);
                        meshVertexArray_.push_back(meshVertexC);
                        meshVertexArray_.push_back(meshVertexD);
                    }

                    meshIndexArray_.push_back(newMeshData.indexCount++);
                    meshIndexArray_.push_back(newMeshData.indexCount++);
                    meshIndexArray_.push_back(newMeshData.indexCount++);

                    newMeshData.vertexCount += 6;
                }

                if (sharpCorner)
                {
                    meshVertexA.position = glm::vec3(vertexC, 0.0f);
                    meshVertexB.position = glm::vec3(vertexD, 0.0f);

                    if (angle > 0.0f) // turn to the left
                    {
                        meshVertexC.position = glm::vec3(vertexF, 0.0f);
                    }
                    else
                    {
                        meshVertexC.position = glm::vec3(vertexE, 0.0f);
                    }

                    glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);

                    float dotABC = glm::dot(normal, normalABC);

                    meshVertexArray_.push_back(meshVertexA);
                    meshVertexArray_.push_back(meshVertexB);
                    meshVertexArray_.push_back(meshVertexC);

                    if (dotABC > 0.0f)
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                    }
                    else
                    {
                        meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                        meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                        meshIndexArray_.push_back(newMeshData.vertexCount);
                    }

                    newMeshData.vertexCount += 3;
                    newMeshData.indexCount += 3;

                    if (angle > 0.0f) // turn to the left
                    {
                        vertexA = vertexC;
                        vertexB = vertexF;
                    }
                    else
                    {
                        vertexA = vertexE;
                        vertexB = vertexD;
                    }
                }
                else
                {
                    vertexA = vertexC;
                    vertexB = vertexD;
                }

                vertex1 = vertex2;
                direction1 = direction2;
                slope1 = slope2;
            }

            vertex2 = vertex3;
        }

        vertexC = vertex2 + scaled2;
        vertexD = vertex2 - scaled2;

        meshVertexA.position = glm::vec3(vertexA, 0.0f);
        meshVertexB.position = glm::vec3(vertexB, 0.0f);
        meshVertexC.position = glm::vec3(vertexC, 0.0f);
        meshVertexD.position = glm::vec3(vertexD, 0.0f);

        glm::vec3 normalABC = glm::cross(meshVertexB.position - meshVertexA.position, meshVertexC.position - meshVertexA.position);
        glm::vec3 normalDCB = glm::cross(meshVertexC.position - meshVertexD.position, meshVertexB.position - meshVertexD.position);

        float dotABC = glm::dot(normal, normalABC);
        float dotDCB = glm::dot(normal, normalDCB);

        if (!triangulate)
        {
            meshVertexArray_.push_back(meshVertexA);
            meshVertexArray_.push_back(meshVertexB);
            meshVertexArray_.push_back(meshVertexC);
            meshVertexArray_.push_back(meshVertexD);

            if (dotABC > 0.0f)
            {
                meshIndexArray_.push_back(newMeshData.vertexCount);
                meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                meshIndexArray_.push_back(newMeshData.vertexCount + 2);
            }
            else
            {
                meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                meshIndexArray_.push_back(newMeshData.vertexCount);
            }

            if (dotDCB > 0.0f)
            {
                meshIndexArray_.push_back(newMeshData.vertexCount + 3);
                meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                meshIndexArray_.push_back(newMeshData.vertexCount + 1);
            }
            else
            {
                meshIndexArray_.push_back(newMeshData.vertexCount + 1);
                meshIndexArray_.push_back(newMeshData.vertexCount + 2);
                meshIndexArray_.push_back(newMeshData.vertexCount + 3);
            }

            newMeshData.vertexCount += 4;
            newMeshData.indexCount += 6;
        }
        else
        {
            if (dotABC > 0.0f)
            {
                meshVertexArray_.push_back(meshVertexA);
                meshVertexArray_.push_back(meshVertexB);
                meshVertexArray_.push_back(meshVertexC);
            }
            else
            {
                meshVertexArray_.push_back(meshVertexC);
                meshVertexArray_.push_back(meshVertexB);
                meshVertexArray_.push_back(meshVertexA);
            }

            meshIndexArray_.push_back(newMeshData.indexCount++);
            meshIndexArray_.push_back(newMeshData.indexCount++);
            meshIndexArray_.push_back(newMeshData.indexCount++);

            if (dotDCB > 0.0f)
            {
                meshVertexArray_.push_back(meshVertexD);
                meshVertexArray_.push_back(meshVertexC);
                meshVertexArray_.push_back(meshVertexB);
            }
            else
            {
                meshVertexArray_.push_back(meshVertexB);
                meshVertexArray_.push_back(meshVertexC);
                meshVertexArray_.push_back(meshVertexD);
            }

            meshIndexArray_.push_back(newMeshData.indexCount++);
            meshIndexArray_.push_back(newMeshData.indexCount++);
            meshIndexArray_.push_back(newMeshData.indexCount++);

            newMeshData.vertexCount += 6;
        }

        for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
        {
            glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
            const MeshVertex& currentMeshVertex = meshVertexArray_[meshVeretxArrayIndex];

            newMeshData.maxVertexPosition.x = std::max(newMeshData.maxVertexPosition.x, currentMeshVertex.position.x);
            newMeshData.maxVertexPosition.y = std::max(newMeshData.maxVertexPosition.y, currentMeshVertex.position.y);

            newMeshData.minVertexPosition.x = std::min(newMeshData.minVertexPosition.x, currentMeshVertex.position.x);
            newMeshData.minVertexPosition.y = std::min(newMeshData.minVertexPosition.y, currentMeshVertex.position.y);
        }

        if (recenter)
        {
            glm::vec3 center = (newMeshData.maxVertexPosition + newMeshData.minVertexPosition) / 2.0f;
            for (glm::uint i = 0; i < newMeshData.vertexCount; ++i)
            {
                glm::uint meshVeretxArrayIndex = newMeshData.vertexIndex + i;
                meshVertexArray_[meshVeretxArrayIndex].position -= center;
            }
            newMeshData.maxVertexPosition -= center;
            newMeshData.minVertexPosition -= center;
        }

        meshDataArray_.push_back(newMeshData);
        index = (glm::uint)(meshDataArray_.size() - 1);

        return true;
    }

    // protected ---------------------------------------------------------------

    MeshVertex MeshStores::GetMeshVertexFromFaceVertex(
        const FaceVertex& faceVertex,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& textureCoordinates,
        const std::vector<glm::vec3>& normals)
    {
        MeshVertex meshVertex;

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

        return meshVertex;
    }
}