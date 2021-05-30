#pragma once

#include "glm/glm.hpp"

#include "Engine/RenderData.h"



namespace Project001
{
    class ModelStores;
    class TextureStores;

    class Renderer
    {
    public:
        static Renderer* Create(ModelStores* modelStoresPtr, TextureStores* textureStoresPtr);

        virtual void AddTexture(
            unsigned int textureSlot,
            unsigned char* data,
            int width,
            int height,
            int numberOfComponents) = 0;

        virtual void SetModelStoresPtr(ModelStores* modelStoresPtr) = 0;
        virtual void SetTextureStoresPtr(TextureStores* textureStoresPtr) = 0;

        virtual void SetViewMatrix(const glm::mat4& viewMatrix) = 0;
        virtual void SetViewPosition(const glm::vec3& viewPosition) = 0;
        virtual void SetProjectionMatrix(const glm::mat4& projectionMatrix) = 0;

        virtual void SetDirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) = 0;
        virtual void AddPointLight(
            const glm::vec3& position,
            const float& constant,
            const float& linear,
            const float& quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) = 0;
        virtual void AddSpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            const float& cutoff,
            const float& outerCutoff,
            const float& constant,
            const float& linear,
            const float& quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) = 0;

        virtual void ClearDirectionalLight() = 0;
        virtual void ClearPointLights() = 0;
        virtual void ClearSpotLights() = 0;

        virtual void AddModel(
            const unsigned int& modelIndex,
            const unsigned int& textureIndex,
            const unsigned int& specularIndex,
            const float& shininess,
            const glm::vec4& color,
            const glm::vec3& scale,
            const glm::vec3& position,
            const glm::quat& orientation) = 0;

        virtual void ClearBuffers() = 0;

        virtual void Render() = 0;

    protected:

    private:
    };
}