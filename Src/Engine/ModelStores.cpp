#include "ModelStores.h"

#include <fstream>



namespace Project001
{
    ModelStores::ModelStores()
    {}

    ModelStores::~ModelStores()
    {}

    void ModelStores::ClearModels()
    {
        modelVertexArray_.clear();
        modelIndexArray_.clear();
        modelDataArray_.clear();
    }

    bool ModelStores::GetModel(unsigned int index, ModelVertex*& firstVertexPtr, glm::uint& vertexCount, glm::uint*& firstIndexPtr, glm::uint& indexCount)
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

    bool ModelStores::LoadModel(const std::string& path, unsigned int& index, bool triangulate)
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

                    for (unsigned int i = 0; i < splitValues.size(); ++i)
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
                            for (unsigned int i = 0; i < face.size(); ++i)
                            {
                                FaceVertex& faceVertex = face[i];
                                ModelVertex modelVertex;
                                modelVertex.position = positions[faceVertex.positonIndex];
                                modelVertex.textureCoordinte = textureCoordinates[faceVertex.textureCoordinateIndex];
                                modelVertex.normal = normals[faceVertex.normalIndex];
                                modelVertexArray_.push_back(modelVertex);
                            }

                            for (unsigned int i = 1; i < face.size() - 1; ++i)
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
                            for (unsigned int i = 1; i < face.size() - 1; ++i)
                            {
                                FaceVertex& face0 = face[0];
                                FaceVertex& face1 = face[i];
                                unsigned int i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
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
        index = (unsigned int)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DTriangleFan(const std::vector<glm::vec2>& vertices, unsigned int& index, bool triangulate)
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
            for (unsigned int i = 0; i < vertices.size(); ++i)
            {
                ModelVertex modelVertex;
                modelVertex.position = glm::vec3(vertices[i], 0.0f);
                modelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertex.normal = normal;
                modelVertexArray_.push_back(modelVertex);
            }

            for (unsigned int i = 1; i < vertices.size() - 1; ++i)
            {
                modelIndexArray_.push_back(0);

                modelIndexArray_.push_back(i);

                unsigned int i_plus_1 = i + 1;
                modelIndexArray_.push_back(i_plus_1);
            }

            newModelData.vertexCount = (glm::uint)vertices.size();
            newModelData.indexCount += (glm::uint)((vertices.size() - 2) * 3);
        }
        else
        {
            for (unsigned int i = 1; i < vertices.size() - 1; ++i)
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
                unsigned int i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                modelVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                modelVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexB.normal = normal;
                modelVertexArray_.push_back(modelVertexB);
                modelIndexArray_.push_back(newModelData.indexCount++);
            }

            newModelData.vertexCount += (glm::uint)((vertices.size() - 2) * 3);
        }

        modelDataArray_.push_back(newModelData);
        index = (unsigned int)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DTriangleStrip(const std::vector<glm::vec2>& vertices, unsigned int& index, bool triangulate)
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
            for (unsigned int i = 0; i < vertices.size(); ++i)
            {
                ModelVertex modelVertex;
                modelVertex.position = glm::vec3(vertices[i], 0.0f);
                modelVertex.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertex.normal = normal;
                modelVertexArray_.push_back(modelVertex);
            }

            for (unsigned int i = 0; i < vertices.size() - 2; ++i)
            {
                unsigned int i_plus_1 = i + 1;
                unsigned int i_plus_2 = i + 2;

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
            for (unsigned int i = 0; i < vertices.size() - 2; ++i)
            {
                ModelVertex modelVertexA;
                modelVertexA.position = glm::vec3(vertices[i], 0.0f);
                modelVertexA.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexA.normal = normal;

                ModelVertex modelVertexB;
                unsigned int i_plus_1 = i + 1; // Doing this suppresses the C26451 warning in visual studio
                modelVertexB.position = glm::vec3(vertices[i_plus_1], 0.0f);
                modelVertexB.textureCoordinte = glm::vec2(0.0f, 0.0f);
                modelVertexB.normal = normal;

                ModelVertex modelVertexC;
                unsigned int i_plus_2 = i + 2; // Doing this suppresses the C26451 warning in visual studio
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
        index = (unsigned int)(modelDataArray_.size() - 1);

        return true;
    }

    bool ModelStores::Generate2DLine(const std::vector<glm::vec2>& vertices, float width, unsigned int& index)
    {
        if (vertices.size() < 2)
        {
            return false;
        }

        // TODO:

        return false;
    }
}