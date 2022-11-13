#pragma once

#include "Engine/RenderData.h"



namespace Project001
{
    struct MeshVertex;

    class Window;

    class Renderer
    {
    public:
        static Renderer* Create(Window* windowPtr, unsigned int width, unsigned int height);

        virtual ~Renderer();

        virtual void SetDepthTesting(
            bool depthTesting) = 0;

        virtual void SetMultisampleAntiAliasing(
            bool multisampleAntaiAliasing) = 0;

        virtual void SetFramebufferSize(
            unsigned int width,
            unsigned int height) = 0;

        // viewport origin is the bottom left corner
        virtual void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) = 0;

        virtual void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) = 0;

        virtual bool CreateTexture(
            unsigned int& textureId,
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool mipMaps) = 0;

        virtual bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit) = 0;

        virtual bool DeleteTexture(unsigned int textureId) = 0;

        virtual void DeleteAllTextures() = 0;

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

        virtual bool AddMesh(
            const MeshVertex* meshVerticies,
            unsigned int meshVertexCount,
            const unsigned int* meshIndicies,
            unsigned int meshIndexCount,
            unsigned int textureIndex,
            unsigned int specularIndex,
            const glm::vec3& position,
            const glm::quat& orientation,
            const glm::vec3& scale,
            const glm::vec4& color,
            float shininess,
            bool translucent,
            bool lit) = 0;

        virtual void PrepareCapabilities() = 0;

        virtual void Clear() = 0;

        virtual void Render() = 0;

        virtual void SwapBuffers() = 0;

    protected:

    private:
    };
}