#pragma once

#include "Engine/RenderData.h"



namespace Project001
{
    class MeshStores;

    class Renderer
    {
    public:
        static Renderer* Create(unsigned int width, unsigned int height);

        virtual ~Renderer();

        virtual void SetFramebufferSize(
            unsigned int width,
            unsigned int height) = 0;

        virtual void SetViewportSize(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) = 0;

        virtual bool AddTexture(
            unsigned int textureIndex,
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int numberOfComponents) = 0;

        virtual bool BindTexture(
            unsigned int textureIndex,
            unsigned int textureSlot) = 0;

        virtual void ClearTextures() = 0;

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
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) = 0;
        virtual void AddSpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            float cutoff,
            float outerCutoff,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) = 0;

        virtual void ClearDirectionalLight() = 0;
        virtual void ClearPointLights() = 0;
        virtual void ClearSpotLights() = 0;

        virtual void AddModel(
            MeshStores* meshStoresPtr,
            unsigned int meshIndex,
            unsigned int textureIndex,
            unsigned int specularIndex,
            float shininess,
            const glm::vec4& color,
            bool translucent,
            const glm::vec3& scale,
            const glm::vec3& position,
            const glm::quat& orientation,
            bool lit) = 0;

        virtual void ClearBuffers() = 0;

        virtual void Render() = 0;

    protected:

    private:
    };
}