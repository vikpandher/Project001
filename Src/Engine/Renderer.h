#pragma once

#include "Engine/MeshVertex.h"

#include "glm/gtc/quaternion.hpp"



namespace Project001
{
    class Window;

    struct RendererInfo
    {
        Window* windowPtr;
        unsigned int frameBufferWidth;
        unsigned int frameBufferHeight;
        unsigned int instanceBufferCapacity;
        unsigned int batchedIndexBufferCapacity;
        unsigned int batchedVertexBufferCapacity;
        bool multisampleAntiAliasing;
        bool depthTesting;
    };

    struct MeshInstanceData
    {
        MeshInstanceData()
            : textureId(0)
            , specularId(0)
            , position(0.0f, 0.0f, 0.0f)
            , orientation(0.0f, 0.0f, 0.0f, 1.0f)
            , scale(1.0f, 1.0f, 1.0f)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , shininess(32.0f)
            , lit(true)
        {}

        MeshInstanceData(
            unsigned int textureId,
            unsigned int specularId,
            const glm::vec3& position,
            const glm::quat& orientation,
            const glm::vec3& scale,
            const glm::vec4& color,
            float shininess,
            bool lit)
            : textureId(textureId)
            , specularId(specularId)
            , position(position)
            , orientation(orientation)
            , scale(scale)
            , color(color)
            , shininess(shininess)
            , lit(lit)
        {}

        unsigned int textureId;
        unsigned int specularId;
        glm::vec3 position;
        glm::quat orientation;
        glm::vec3 scale;
        glm::vec4 color;
        float shininess;
        bool lit;
    };

    class Renderer
    {
    public:
        static Renderer* Create(const RendererInfo& rendererInfo);

        virtual ~Renderer();

        virtual void SetDepthTesting(bool depthTesting) = 0;
        virtual bool GetDepthTesting() const = 0;

        virtual void SetMultisampleAntiAliasing(bool multisampleAntiAliasing) = 0;

        virtual void SetInstanceBufferCapacity(unsigned int capacity) = 0;

        virtual void SetBatchedIndexBufferCapacity(unsigned int capacity) = 0;
        virtual void SetBatchedVertexBufferCapacity(unsigned int capacity) = 0;

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

        // measurements are relative to the viewport
        // x = 0.0 and y = 0.0 is the bottom left
        virtual void GetCameraViewport(
            float& x,
            float& y,
            float& width,
            float& height) = 0;

        virtual void SetCameraViewport(
            float x,
            float y,
            float width,
            float height) = 0;

        virtual void SetBorderColor(const glm::vec4& color) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual void CreateTexture(
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

        virtual void ClearDepthOnly() = 0;

        virtual void CreateMesh(
            unsigned int& meshId,
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount) = 0;

        virtual bool DeleteMesh(unsigned int meshId) = 0;

        virtual void DeleteAllMeshes() = 0;

        virtual bool RenderMesh(
            unsigned int meshId,
            const MeshInstanceData* meshInstanceDataPtr,
            unsigned int meshInstanceCount) = 0;

        virtual bool AddMeshToBatch(
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount,
            unsigned int textureId,
            unsigned int specularId,
            const glm::vec3& position,
            const glm::quat& orientation,
            const glm::vec3& scale,
            const glm::vec4& color,
            float shininess,
            bool lit) = 0;

        virtual void RenderBatch() = 0;

        virtual void FinishRendering() = 0;

        virtual void SwapBuffers() = 0;

    protected:

    private:
    };
}