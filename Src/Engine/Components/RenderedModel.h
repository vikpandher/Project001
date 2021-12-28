#pragma once

#include "Engine/Components/Placement.h"



namespace Project001
{
    struct RenderedModel : Placement
    {
        RenderedModel();

        // Inherited:
        // glm::vec3 position;
        // glm::quat orientation;

        bool visible;
        unsigned int meshIndex;
        unsigned int textureIndex;
        unsigned int specularIndex;
        glm::vec3 scale;
        union
        {
            glm::vec4 color;
            glm::vec3 colorRGB;
        };
        float shininess; // 32.0f looks good
        bool translucent;
        bool lit;
    };

    inline RenderedModel::RenderedModel()
        : visible(true)
        , meshIndex((unsigned int)-1)
        , textureIndex((unsigned int)-1)
        , specularIndex((unsigned int)-1)
        , scale(1.0f, 1.0f, 1.0f)
        , color(1.0f, 1.0f, 1.0f, 1.0f)
        , shininess(0.0f)
        , translucent(false)
        , lit(true)
    {}
}