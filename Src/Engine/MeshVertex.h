#pragma once

#include "glm/glm.hpp"



namespace Project001
{
    struct MeshVertex
    {
        MeshVertex()
            : position(0.0f)
            , textureCoordinate(0.0f)
            , normal(0.0f)
        {}

        glm::vec3 position;
        glm::vec2 textureCoordinate;
        glm::vec3 normal;
    };
}