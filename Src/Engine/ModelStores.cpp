#include "ModelStores.h"

#include <fstream>

#include "Engine/MathFunctions.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    ModelStores::ModelStores()
    {
        TestGet2DVectorAngle();
    }

    ModelStores::~ModelStores()
    {}

    void ModelStores::ClearModels()
    {
        modelVertexArray_.clear();
        modelIndexArray_.clear();
        modelDataArray_.clear();
    }

    bool ModelStores::GetModel(glm::uint index, ModelVertex*& firstVertexPtr, glm::uint& vertexCount, glm::uint*& firstIndexPtr, glm::uint& indexCount)
    {
        if (index >= modelDataArray_.size())
        {
            return false;
        }

        ModelData& modelData = modelDataArray_[index];
        ModelVertex* modelVertexArrayPtr = modelVertexArray_.data();

        firstVertexPtr = &modelVertexArrayPtr[modelData.vertexIndex];
        vertexCount = modelData.vertexCount;

        glm::uint* modelIndexArrayPtr = modelIndexArray_.data();

        firstIndexPtr = &modelIndexArrayPtr[modelData.indexIndex];
        indexCount = modelData.indexCount;

        return true;
    }

    bool ModelStores::LoadModel(const std::string& path, glm::uint& index, bool triangulate)
    {
        // file path must have .obj extension
        if (path.size() < 4 || path.substr(path.size() - 4, 4) != ".obj")
        {
            return false;
        }

        std::ifstream file(path);

        if (!file.is_open())
        {
            return false;
        }

        ModelData newModelData;
        newModelData.vertexIndex = (glm::uint)modelVertexArray_.size();
        newModelData.vertexCount = 0;
        newModelData.indexIndex = (glm::uint)modelIndexArray_.size();
        newModelData.indexCount = 0;

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
            else if (tagStart != std::string::npos)
            {
                continue;
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

                if (valueStart != std::string::npos && valueEnd != std::string::npos)
                {
                    splitValues.push_back(values.substr(valueStart, valueEnd - valueStart));
                }
                else if (tagStart != std::string::npos)
                {
                    splitValues.push_back(values.substr(valueStart));
                }
                else
                {
                    continue;
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
                    struct FaceVertex
                    {
                        glm::uint positonIndex;
                        glm::uint textureCoordinateIndex;
                        glm::uint normalIndex;
                    };

                    std::vector<FaceVertex> face;

                    for (glm::uint i = 0; i < splitValues.size(); ++i)
                    {
                        std::string indexGroup = splitValues[i];

                        std::vector<std::string> indexGroupValues;

                        size_t previousindexGroupValueEnd = 0;
                        while (previousindexGroupValueEnd != std::string::npos)
                        {
                            size_t indexGroupValueStart = indexGroup.find_first_not_of("/", previousindexGroupValueEnd);
                            size_t indexGroupValueEnd = indexGroup.find_first_of("/", indexGroupValueStart);

                            if (indexGroupValueStart != std::string::npos && indexGroupValueEnd != std::string::npos)
                            {
                                indexGroupValues.push_back(indexGroup.substr(indexGroupValueStart, indexGroupValueEnd - indexGroupValueStart));
                            }
                            else if (tagStart != std::string::npos)
                            {
                                indexGroupValues.push_back(indexGroup.substr(indexGroupValueStart));
                            }
                            else
                            {
                                continue;
                            }

                            previousindexGroupValueEnd = indexGroupValueEnd;
                        }

                        FaceVertex newFaceVertex;
                        newFaceVertex.positonIndex = std::stoi(indexGroupValues[0]) - 1; // indicies start at 1
                        newFaceVertex.textureCoordinateIndex = std::stoi(indexGroupValues[1]) - 1;
                        newFaceVertex.normalIndex = std::stoi(indexGroupValues[2]) - 1;

                        face.push_back(newFaceVertex);
                    }

                    if (face.size() >= 3)
                    {
                        if (!triangulate)
                        {
                            for (glm::uint i = 0; i < face.size(); ++i)
                            {
                                FaceVertex& faceVertex = face[i];
                                ModelVertex modelVertex;
                                modelVertex.position = positions[faceVertex.positonIndex];
                                modelVertex.textureCoordinte = textureCoordinates[faceVertex.textureCoordinateIndex];
                                modelVertex.normal = normals[faceVertex.normalIndex];
                                modelVertexArray_.push_back(modelVertex);
                            }

                            for (glm::uint i = 1; i < face.size() - 1; ++i)
                            {
                                modelIndexArray_.push_back(newModelData.vertexCount);
                                modelIndexArray_.push_back(newModelData.vertexCount + i);
                                modelIndexArray_.push_back(newModelData.vertexCount + i + 1);
                            }

                            newModelData.vertexCount += (glm::uint)face.size();
                            newModelData.indexCount += (glm::uint)((face.size() - 2) * 3);
                        }
                        else
                        {
                            for (glm::uint i = 1; i < face.size() - 1; ++i)
                            {
                                FaceVertex& face0 = face[0];
                                FaceVertex& face1 = face[i];
                                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                                FaceVertex& face2 = face[i_plus_1];

                                ModelVertex modelVertex;
                                modelVertex.position = positions[face0.positonIndex];
                                modelVertex.textureCoordinte = textureCoordinates[face0.textureCoordinateIndex];
                                modelVertex.normal = normals[face0.normalIndex];
                                modelVertexArray_.push_back(modelVertex);
                                modelIndexArray_.push_back(newModelData.indexCount++);

                                modelVertex.position = positions[face1.positonIndex];
                                modelVertex.textureCoordinte = textureCoordinates[face1.textureCoordinateIndex];
                                modelVertex.normal = normals[face1.normalIndex];
                                modelVertexArray_.push_back(modelVertex);
                                modelIndexArray_.push_back(newModelData.indexCount++);

                                modelVertex.position = positions[face2.positonIndex];
                                modelVertex.textureCoordinte = textureCoordinates[face2.textureCoordinateIndex];
                                modelVertex.normal = normals[face2.normalIndex];
                                modelVertexArray_.push_back(modelVertex);
                                modelIndexArray_.push_back(newModelData.indexCount++);
                            }

                            newModelData.vertexCount += (glm::uint)((face.size() - 2) * 3);
                        }
                    }
                }
            }
        }

        modelDataArray_.push_back(newModelData);
        index = (glm::uint)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, glm::uint& index, bool triangulate)
    {
        if (vertices.size() < 3)
        {
            return false;
        }

        ModelData newModelData;
        newModelData.vertexIndex = (glm::uint)modelVertexArray_.size();
        newModelData.vertexCount = 0;
        newModelData.indexIndex = (glm::uint)modelIndexArray_.size();
        newModelData.indexCount = 0;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (glm::uint i = 0; i < vertices.size(); ++i)
            {
                ModelVertex modelVertex;
                modelVertex.position = glm::vec3(vertices[i], 0.0f);
                modelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertex.normal = normal;
                modelVertexArray_.push_back(modelVertex);
            }

            for (glm::uint i = 1; i < vertices.size() - 1; ++i)
            {
                modelIndexArray_.push_back(0);

                modelIndexArray_.push_back(i);

                glm::uint i_plus_1 = i + 1;
                modelIndexArray_.push_back(i_plus_1);
            }

            newModelData.vertexCount = (glm::uint)vertices.size();
            newModelData.indexCount += (glm::uint)((vertices.size() - 2) * 3);
        }
        else
        {
            for (glm::uint i = 1; i < vertices.size() - 1; ++i)
            {
                ModelVertex centerModelVertex;
                centerModelVertex.position = glm::vec3(vertices[0], 0.0f);
                centerModelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                centerModelVertex.normal = normal;
                modelVertexArray_.push_back(centerModelVertex);
                modelIndexArray_.push_back(newModelData.indexCount++);

                ModelVertex modelVertexA;
                modelVertexA.position = glm::vec3(vertices[i], 0.0f);
                modelVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexA.normal = normal;
                modelVertexArray_.push_back(modelVertexA);
                modelIndexArray_.push_back(newModelData.indexCount++);

                ModelVertex modelVertexB;
                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                modelVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                modelVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexB.normal = normal;
                modelVertexArray_.push_back(modelVertexB);
                modelIndexArray_.push_back(newModelData.indexCount++);
            }

            newModelData.vertexCount += (glm::uint)((vertices.size() - 2) * 3);
        }

        modelDataArray_.push_back(newModelData);
        index = (glm::uint)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::GenerateTriangles(const std::vector<glm::vec2>& vertices, glm::uint& index)
    {
        if (vertices.size() == 0 || vertices.size() % 3 != 0)
        {
            return false;
        }

        ModelData newModelData;
        newModelData.vertexIndex = (glm::uint)modelVertexArray_.size();
        newModelData.vertexCount = (glm::uint)vertices.size();
        newModelData.indexIndex = (glm::uint)modelIndexArray_.size();
        newModelData.indexCount = (glm::uint)vertices.size();

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        for (glm::uint i = 0; i < vertices.size(); ++i)
        {
            ModelVertex modelVertex;
            modelVertex.position = glm::vec3(vertices[i], 0.0f);
            modelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
            modelVertex.normal = normal;

            modelVertexArray_.push_back(modelVertex);
            modelIndexArray_.push_back(i);
        }

        modelDataArray_.push_back(newModelData);
        index = (glm::uint)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, glm::uint& index, bool triangulate)
    {
        if (vertices.size() < 3)
        {
            return false;
        }

        ModelData newModelData;
        newModelData.vertexIndex = (glm::uint)modelVertexArray_.size();
        newModelData.vertexCount = 0;
        newModelData.indexIndex = (glm::uint)modelIndexArray_.size();
        newModelData.indexCount = 0;

        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        if (!triangulate)
        {
            for (glm::uint i = 0; i < vertices.size(); ++i)
            {
                ModelVertex modelVertex;
                modelVertex.position = glm::vec3(vertices[i], 0.0f);
                modelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertex.normal = normal;
                modelVertexArray_.push_back(modelVertex);
            }

            for (glm::uint i = 0; i < vertices.size() - 2; ++i)
            {
                glm::uint i_plus_1 = i + 1;
                glm::uint i_plus_2 = i + 2;

                if (i % 2 == 0)
                {
                    modelIndexArray_.push_back(i);
                    modelIndexArray_.push_back(i_plus_1);
                    modelIndexArray_.push_back(i_plus_2);
                }
                else
                {
                    modelIndexArray_.push_back(i_plus_2);
                    modelIndexArray_.push_back(i_plus_1);
                    modelIndexArray_.push_back(i);
                }
            }

            newModelData.vertexCount = (glm::uint)vertices.size();
            newModelData.indexCount += (glm::uint)((vertices.size() - 2) * 3);
        }
        else
        {
            for (glm::uint i = 0; i < vertices.size() - 2; ++i)
            {
                ModelVertex modelVertexA;
                modelVertexA.position = glm::vec3(vertices[i], 0.0f);
                modelVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexA.normal = normal;

                ModelVertex modelVertexB;
                glm::uint i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                modelVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                modelVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexB.normal = normal;

                ModelVertex modelVertexC;
                glm::uint i_plus_2 = i + 2; // Doing this suppresses the C26451 warning in visual studio
                modelVertexC.position = glm::vec3(vertices[i_plus_2], 0.0f);
                modelVertexC.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexC.normal = normal;

                if (i % 2 == 0)
                {
                    modelVertexArray_.push_back(modelVertexA);
                    modelVertexArray_.push_back(modelVertexB);
                    modelVertexArray_.push_back(modelVertexC);
                }
                else
                {
                    modelVertexArray_.push_back(modelVertexC);
                    modelVertexArray_.push_back(modelVertexB);
                    modelVertexArray_.push_back(modelVertexA);
                }

                modelIndexArray_.push_back(newModelData.indexCount++);
                modelIndexArray_.push_back(newModelData.indexCount++);
                modelIndexArray_.push_back(newModelData.indexCount++);
            }

            newModelData.vertexCount += (glm::uint)((vertices.size() - 2) * 3);
        }

        modelDataArray_.push_back(newModelData);
        index = (glm::uint)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DLine(const std::vector<glm::vec2>& vertices, float width, glm::uint& index, bool triangulate)
    {
        if (vertices.size() < 2)
        {
            return false;
        }

        ModelData newModelData;
        newModelData.vertexIndex = (glm::uint)modelVertexArray_.size();
        newModelData.vertexCount = 0;
        newModelData.indexIndex = (glm::uint)modelIndexArray_.size();
        newModelData.indexCount = 0;

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

        ModelVertex modelVertexA;
        modelVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
        modelVertexA.normal = normal;
        ModelVertex modelVertexB;
        modelVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
        modelVertexB.normal = normal;
        ModelVertex modelVertexC;
        modelVertexC.textureCoordinte = glm::vec2(0.0f, 0.0f);
        modelVertexC.normal = normal;
        ModelVertex modelVertexD;
        modelVertexD.textureCoordinte = glm::vec2(0.0f, 0.0f);
        modelVertexD.normal = normal;

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

                modelVertexA.position = glm::vec3(vertexA, 0.0f);
                modelVertexB.position = glm::vec3(vertexB, 0.0f);
                modelVertexC.position = glm::vec3(vertexC, 0.0f);
                modelVertexD.position = glm::vec3(vertexD, 0.0f);

                glm::vec3 normalABC = glm::cross(modelVertexB.position - modelVertexA.position, modelVertexC.position - modelVertexA.position);
                glm::vec3 normalDCB = glm::cross(modelVertexC.position - modelVertexD.position, modelVertexB.position - modelVertexD.position);

                float dotABC = glm::dot(normal, normalABC);
                float dotDCB = glm::dot(normal, normalDCB);

                if (!triangulate)
                {
                    modelVertexArray_.push_back(modelVertexA);
                    modelVertexArray_.push_back(modelVertexB);
                    modelVertexArray_.push_back(modelVertexC);
                    modelVertexArray_.push_back(modelVertexD);

                    if (dotABC > 0.0f)
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount);
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                    }
                    else
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                        modelIndexArray_.push_back(newModelData.vertexCount);
                    }

                    if (dotDCB > 0.0f)
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount + 3);
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                    }
                    else
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                        modelIndexArray_.push_back(newModelData.vertexCount + 3);
                    }

                    newModelData.vertexCount += 4;
                    newModelData.indexCount += 6;
                }
                else
                {
                    if (dotABC > 0.0f)
                    {
                        modelVertexArray_.push_back(modelVertexA);
                        modelVertexArray_.push_back(modelVertexB);
                        modelVertexArray_.push_back(modelVertexC);
                    }
                    // When the width of the line is too large, triangles can 
                    // flip.
                    else
                    {
                        modelVertexArray_.push_back(modelVertexC);
                        modelVertexArray_.push_back(modelVertexB);
                        modelVertexArray_.push_back(modelVertexA);
                    }

                    modelIndexArray_.push_back(newModelData.indexCount++);
                    modelIndexArray_.push_back(newModelData.indexCount++);
                    modelIndexArray_.push_back(newModelData.indexCount++);

                    if (dotDCB > 0.0f)
                    {
                        modelVertexArray_.push_back(modelVertexD);
                        modelVertexArray_.push_back(modelVertexC);
                        modelVertexArray_.push_back(modelVertexB);
                    }
                    else
                    {
                        modelVertexArray_.push_back(modelVertexB);
                        modelVertexArray_.push_back(modelVertexC);
                        modelVertexArray_.push_back(modelVertexD);
                    }

                    modelIndexArray_.push_back(newModelData.indexCount++);
                    modelIndexArray_.push_back(newModelData.indexCount++);
                    modelIndexArray_.push_back(newModelData.indexCount++);

                    newModelData.vertexCount += 6;
                }

                if (sharpCorner)
                {
                    modelVertexA.position = glm::vec3(vertexC, 0.0f);
                    modelVertexB.position = glm::vec3(vertexD, 0.0f);

                    if (angle > 0.0f) // turn to the left
                    {
                        modelVertexC.position = glm::vec3(vertexF, 0.0f);
                    }
                    else
                    {
                        modelVertexC.position = glm::vec3(vertexE, 0.0f);
                    }

                    glm::vec3 normalABC = glm::cross(modelVertexB.position - modelVertexA.position, modelVertexC.position - modelVertexA.position);

                    float dotABC = glm::dot(normal, normalABC);

                    modelVertexArray_.push_back(modelVertexA);
                    modelVertexArray_.push_back(modelVertexB);
                    modelVertexArray_.push_back(modelVertexC);

                    if (dotABC > 0.0f)
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount);
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                    }
                    else
                    {
                        modelIndexArray_.push_back(newModelData.vertexCount + 2);
                        modelIndexArray_.push_back(newModelData.vertexCount + 1);
                        modelIndexArray_.push_back(newModelData.vertexCount);
                    }

                    newModelData.vertexCount += 3;
                    newModelData.indexCount += 3;

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

        modelVertexA.position = glm::vec3(vertexA, 0.0f);
        modelVertexB.position = glm::vec3(vertexB, 0.0f);
        modelVertexC.position = glm::vec3(vertexC, 0.0f);
        modelVertexD.position = glm::vec3(vertexD, 0.0f);

        glm::vec3 normalABC = glm::cross(modelVertexB.position - modelVertexA.position, modelVertexC.position - modelVertexA.position);
        glm::vec3 normalDCB = glm::cross(modelVertexC.position - modelVertexD.position, modelVertexB.position - modelVertexD.position);

        float dotABC = glm::dot(normal, normalABC);
        float dotDCB = glm::dot(normal, normalDCB);

        if (!triangulate)
        {
            modelVertexArray_.push_back(modelVertexA);
            modelVertexArray_.push_back(modelVertexB);
            modelVertexArray_.push_back(modelVertexC);
            modelVertexArray_.push_back(modelVertexD);

            if (dotABC > 0.0f)
            {
                modelIndexArray_.push_back(newModelData.vertexCount);
                modelIndexArray_.push_back(newModelData.vertexCount + 1);
                modelIndexArray_.push_back(newModelData.vertexCount + 2);
            }
            else
            {
                modelIndexArray_.push_back(newModelData.vertexCount + 2);
                modelIndexArray_.push_back(newModelData.vertexCount + 1);
                modelIndexArray_.push_back(newModelData.vertexCount);
            }

            if (dotDCB > 0.0f)
            {
                modelIndexArray_.push_back(newModelData.vertexCount + 3);
                modelIndexArray_.push_back(newModelData.vertexCount + 2);
                modelIndexArray_.push_back(newModelData.vertexCount + 1);
            }
            else
            {
                modelIndexArray_.push_back(newModelData.vertexCount + 1);
                modelIndexArray_.push_back(newModelData.vertexCount + 2);
                modelIndexArray_.push_back(newModelData.vertexCount + 3);
            }

            newModelData.vertexCount += 4;
            newModelData.indexCount += 6;
        }
        else
        {
            if (dotABC > 0.0f)
            {
                modelVertexArray_.push_back(modelVertexA);
                modelVertexArray_.push_back(modelVertexB);
                modelVertexArray_.push_back(modelVertexC);
            }
            else
            {
                modelVertexArray_.push_back(modelVertexC);
                modelVertexArray_.push_back(modelVertexB);
                modelVertexArray_.push_back(modelVertexA);
            }

            modelIndexArray_.push_back(newModelData.indexCount++);
            modelIndexArray_.push_back(newModelData.indexCount++);
            modelIndexArray_.push_back(newModelData.indexCount++);

            if (dotDCB > 0.0f)
            {
                modelVertexArray_.push_back(modelVertexD);
                modelVertexArray_.push_back(modelVertexC);
                modelVertexArray_.push_back(modelVertexB);
            }
            else
            {
                modelVertexArray_.push_back(modelVertexB);
                modelVertexArray_.push_back(modelVertexC);
                modelVertexArray_.push_back(modelVertexD);
            }

            modelIndexArray_.push_back(newModelData.indexCount++);
            modelIndexArray_.push_back(newModelData.indexCount++);
            modelIndexArray_.push_back(newModelData.indexCount++);

            newModelData.vertexCount += 6;
        }

        modelDataArray_.push_back(newModelData);
        index = (glm::uint)(modelDataArray_.size() - 1);

        return true;
    }
}