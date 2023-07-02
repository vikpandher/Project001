#pragma once

#include "Engine/MeshVertex.h"

#include <vector>



namespace Project001
{
    struct MeshData
    {
        MeshData();

        void Clear();

        glm::vec3 GetSize() const;

        std::vector<MeshVertex> meshVertexArray;
        std::vector<unsigned int> meshIndexArray;
        float maxRadius;
        glm::vec3 maxVertexPosition;
        glm::vec3 minVertexPosition;
    };

    inline MeshData::MeshData()
    {
        maxRadius = 0;
        maxVertexPosition.x = -1.0f * std::numeric_limits<float>::infinity();
        maxVertexPosition.y = -1.0f * std::numeric_limits<float>::infinity();
        maxVertexPosition.z = -1.0f * std::numeric_limits<float>::infinity();
        minVertexPosition.x = std::numeric_limits<float>::infinity();
        minVertexPosition.y = std::numeric_limits<float>::infinity();
        minVertexPosition.z = std::numeric_limits<float>::infinity();
    }

    inline void MeshData::Clear()
    {
        meshVertexArray.clear();
        meshIndexArray.clear();

        maxRadius = 0;
        maxVertexPosition.x = -1.0f * std::numeric_limits<float>::infinity();
        maxVertexPosition.y = -1.0f * std::numeric_limits<float>::infinity();
        maxVertexPosition.z = -1.0f * std::numeric_limits<float>::infinity();
        minVertexPosition.x = std::numeric_limits<float>::infinity();
        minVertexPosition.y = std::numeric_limits<float>::infinity();
        minVertexPosition.z = std::numeric_limits<float>::infinity();
    }

    inline glm::vec3 MeshData::GetSize() const
    {
        return maxVertexPosition - minVertexPosition;
    }
}