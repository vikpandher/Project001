#pragma once

#include "Engine/RenderData.h"



namespace Project001
{
    struct MeshVertex;

    class Window;

    struct RendererInfo
    {
        Window* windowPtr;
        unsigned int frameBufferWidth;
        unsigned int frameBufferHeight;
        unsigned int indexBufferCapacity;
        unsigned int vertexBufferCapacity;
        bool multisampleAntiAliasing;
        bool depthTesting;
    };

    class Renderer
    {
    public:
        static Renderer* Create(const RendererInfo& rendererInfo);

        virtual ~Renderer();

        virtual void SetDepthTesting(bool depthTesting) = 0;

        virtual void SetMultisampleAntiAliasing(bool multisampleAntiAliasing) = 0;

        virtual void SetIndexBufferCapacity(unsigned int capacity) = 0;

        virtual void SetVertexBufferCapacity(unsigned int capacity) = 0;

        virtual void GetFramebufferSize(
            unsigned int& width,
            unsigned int& height) const = 0;

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

        virtual void SetBorderColor(const glm::vec4& color) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual bool CreateTexture(
            unsigned int& textureId,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps) = 0;

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

        virtual void BeginRendering() = 0;

        virtual void Clear() = 0;

        virtual bool AddMesh(
            const MeshVertex* meshVerticies,
            unsigned int meshVertexCount,
            const unsigned int* meshIndicies,
            unsigned int meshIndexCount,
            unsigned int textureId,
            unsigned int specularId,
            const glm::vec3& position,
            const glm::quat& orientation,
            const glm::vec3& scale,
            const glm::vec4& color,
            float shininess,
            bool translucent,
            bool lit) = 0;

        virtual void Render() = 0;

        virtual void FinishRendering() = 0;

        virtual void SwapBuffers() = 0;

    protected:

    private:
    };
}