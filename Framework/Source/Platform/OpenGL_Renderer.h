// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#pragma once

// https://www.khronos.org/registry/OpenGL/specs/gl/

#include "AutoIdMap.h"
#include "UniqueBiMap.h"
#include "RenderData.h"
#include "Renderer.h"



namespace Project001
{
    class OpenGL_Mesh;
    class OpenGL_Shader;
    class OpenGL_Texture;

    class OpenGL_Renderer : public Renderer
    {
    public:
        OpenGL_Renderer(const RendererInfo& rendererInfo);
        ~OpenGL_Renderer() override;

        bool GetDepthTesting() const override;
        void SetDepthTesting(bool depthTesting) override;

        void SetMultisampleAntiAliasing(bool multisampleAntiAliasing) override;

        void SetInstanceBufferCapacity(unsigned int capacity) override;

        void SetBatchedIndexBufferCapacity(unsigned int capacity) override;
        void SetBatchedVertexBufferCapacity(unsigned int capacity) override;

        void GetFramebufferSize(
            unsigned int& width,
            unsigned int& height) const override;

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override;

        void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) const override;

        void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override;

        glm::vec2 ConvertPointFromWindowToViewport(
            glm::vec2 windowPoint,
            float windowHeight) const override;

        glm::vec2 ConvertPointFromWindowToViewportNormalized(
            glm::vec2 windowPoint,
            float windowHeight) const override;

        void GetCameraViewport(
            float& x,
            float& y,
            float& width,
            float& height) override;

        void SetCameraViewport(
            float x,
            float y,
            float width,
            float height) override;

        // Border color for the borders when the framebuffer's aspect ratio is
        // different from the window's.
        // The alpha value of the border color sets the windows overall
        // transparency.
        void SetBorderColor(const glm::vec4& color) override;

        void SetClearColor(const glm::vec4& color) override;

        void CreateTexture(
            unsigned int& textureId,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps) override;

        bool DeleteTexture(unsigned int textureId) override;

        void DeleteAllTextures() override;

        void SetViewMatrix(const glm::mat4& viewMatrix) override;
        void SetViewPosition(const glm::vec3& viewPosition) override;
        void SetProjectionMatrix(const glm::mat4& projectionMatrix) override;

        void SetDirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;
        void AddPointLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;
        void AddSpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            float cutoff,
            float outerCutoff,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;

        void ClearDirectionalLight() override;
        void ClearPointLights() override;
        void ClearSpotLights() override;

        void BeginRendering();

        void Clear() override;

        void ClearDepthOnly() override;

        void CreateMesh(
            unsigned int& meshId,
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount) override;

        bool DeleteMesh(unsigned int meshId) override;

        void DeleteAllMeshes() override;

        bool RenderMesh(
            unsigned int meshId,
            const MeshInstanceData* meshInstanceDataPtr,
            unsigned int meshInstanceCount) override;

        bool AddMeshToBatch(
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
            bool lit);

        void RenderBatch();

        void FinishRendering();

        void SwapBuffers() override;

    protected:
        void CreateGridBufferAndArray();

        void CreateScreenFramebuffers();

        void CleanUpScreenFramebuffers();

        bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit);

        // returns 2 if texture failed to bind because textureId isn't found
        // returns 1 if texture failed to bind because all texture units are
        //           occupied
        // returns 0 if texture successfuly bound to unit
        int GetTextureUnit(unsigned int textureId, float& textureUnit);

        bool GetStalestTextureUnit(unsigned int& textureUnit) const;

        void IncreaseTectureUnitStaleness();

        void RenderMeshToTexture(OpenGL_Mesh* meshPtr);

        void RenderBatchToTexture();

        void DrawGrid();

        void RenderTextureToScreen();

        void UsePrimaryShaderAndUpdateItsUniforms();

        static const bool s_cullBackface;
        static const bool s_drawWireframe;
        static const bool s_drawNormals;
        static const bool s_drawGrid;

        static const unsigned int s_numberOfTextureUnits_;

        static const unsigned int s_numberOfPointLights_;
        static const unsigned int s_numberOfSpotLights_;

        static const float s_minorGridIncement_;
        static const float s_majorGridIncement_;
        static const float s_gridHalfExtents_;

        Window* windowPtr_;

        bool redrawGrid_;

        bool depthTesting_;
        bool multisampleAntiAliasing_;

        unsigned int instanceBufferCapacity_;

        unsigned int batchedIndexBufferCapacity_;
        unsigned int batchedVertexBufferCapacity_;

        OpenGL_Shader* primaryShaderPtr_;
        OpenGL_Shader* gridShaderPtr_;
        OpenGL_Shader* wireframeShaderPtr_;
        OpenGL_Shader* normalShaderPtr_;
        OpenGL_Shader* screenShaderPtr_;

        unsigned int instanceBufferId_;

        unsigned int batchedVertexArrayId_;
        unsigned int batchedVertexBufferId_;
        unsigned int batchedIndexBufferId_;

        unsigned int gridVertexArrayId_;
        unsigned int gridVertexBufferId_;
        unsigned int gridVertexCount_;

        unsigned int screenVertexArrayId_;
        unsigned int screenVertexBufferId_;

        unsigned int frameBufferWidth_;
        unsigned int frameBufferHeight_;

        unsigned int viewportX_;
        unsigned int viewportY_;
        unsigned int viewportWidth_;
        unsigned int viewportHeight_;

        float cameraViewportX_;
        float cameraViewportY_;
        float cameraViewportWidth_;
        float cameraViewportHeight_;

        glm::vec4 borderColor_;
        glm::vec4 clearColor_;

        unsigned int msaaFrameBufferId_;
        unsigned int msaaTextureId_;

        unsigned int rttFrameBufferId_;
        unsigned int rttTextureId_;

        unsigned int renderBufferId_;

        AutoIdMap<OpenGL_Texture*> texturePtrMap_;
        UniqueBiMap<unsigned int, unsigned int> textureIdToUnitBiMap_;
        std::vector<unsigned int> textureUnitStalenessValues_;

        AutoIdMap<OpenGL_Mesh*> meshPtrMap_;

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;
        glm::mat4 projectionMatrix_;

        DirectionalLight directionalLight_;
        std::vector<PointLight> pointLights_;
        std::vector<SpotLight> spotLights_;

        std::vector<InstanceData> instanceBuffer_;

        std::vector<BatchedVertexData> batchedVertexStagingBuffer_;
        std::vector<unsigned int> batchedIndexStagingBuffer_;
    };

    inline bool OpenGL_Renderer::GetDepthTesting() const
    {
        return depthTesting_;
    }

    inline void OpenGL_Renderer::SetDepthTesting(bool depthTesting)
    {
        depthTesting_ = depthTesting;
    }

    inline void OpenGL_Renderer::GetFramebufferSize(
        unsigned int& width,
        unsigned int& height) const
    {
        width = frameBufferWidth_;
        height = frameBufferHeight_;
    }

    inline void OpenGL_Renderer::GetViewport(
        unsigned int& x,
        unsigned int& y,
        unsigned int& width,
        unsigned int& height) const
    {
        x = viewportX_;
        y = viewportY_;
        width = viewportWidth_;
        height = viewportHeight_;
    }

    inline void OpenGL_Renderer::SetViewport(
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
        viewportX_ = x;
        viewportY_ = y;
        viewportWidth_ = width;
        viewportHeight_ = height;
    }

    inline glm::vec2 OpenGL_Renderer::ConvertPointFromWindowToViewport(
        glm::vec2 windowPoint,
        float windowHeight) const
    {
        return glm::vec2(
            windowPoint.x - (float)viewportX_,
            windowHeight - windowPoint.y - (float)viewportY_);
    }

    inline glm::vec2 OpenGL_Renderer::ConvertPointFromWindowToViewportNormalized(
        glm::vec2 windowPoint,
        float windowHeight) const
    {
        return glm::vec2(
            (windowPoint.x - (float)viewportX_) / (float)viewportWidth_,
            (windowHeight - windowPoint.y - (float)viewportY_) / (float)viewportHeight_);
    }

    inline void OpenGL_Renderer::GetCameraViewport(
        float& x,
        float& y,
        float& width,
        float& height)
    {
        x = cameraViewportX_;
        y = cameraViewportY_;
        width = cameraViewportWidth_;
        height = cameraViewportHeight_;
    }

    inline void OpenGL_Renderer::SetCameraViewport(
        float x,
        float y,
        float width,
        float height)
    {
        cameraViewportX_ = x;
        cameraViewportY_ = y;
        cameraViewportWidth_ = width;
        cameraViewportHeight_ = height;
    }

    inline void OpenGL_Renderer::SetBorderColor(const glm::vec4& color)
    {
        borderColor_ = color;
    }

    inline void OpenGL_Renderer::SetClearColor(const glm::vec4& color)
    {
        clearColor_ = color;
    }

    inline void OpenGL_Renderer::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void OpenGL_Renderer::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void OpenGL_Renderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }

    inline void OpenGL_Renderer::SetDirectionalLight(
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        directionalLight_.direction = direction;
        directionalLight_.ambient = ambient;
        directionalLight_.diffuse = diffuse;
        directionalLight_.specular = specular;
    }

    inline void OpenGL_Renderer::AddPointLight(
        const glm::vec3& position,
        float constant,
        float linear,
        float quadratic,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        if (pointLights_.size() < s_numberOfPointLights_)
        {
            pointLights_.emplace_back(
                position,
                constant,
                linear,
                quadratic,
                ambient,
                diffuse,
                specular);
        }
    }

    inline void OpenGL_Renderer::AddSpotLight(
        const glm::vec3& position,
        const glm::vec3& direction,
        float cutoff,
        float outerCutoff,
        float constant,
        float linear,
        float quadratic,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        if (spotLights_.size() < s_numberOfSpotLights_)
        {
            spotLights_.emplace_back(
                position,
                direction,
                cutoff,
                outerCutoff,
                constant,
                linear,
                quadratic,
                ambient,
                diffuse,
                specular);
        }
    }

    inline void OpenGL_Renderer::ClearDirectionalLight()
    {
        directionalLight_.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionalLight_.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    inline void OpenGL_Renderer::ClearPointLights()
    {
        pointLights_.clear();
    }

    inline void OpenGL_Renderer::ClearSpotLights()
    {
        spotLights_.clear();
    }
}