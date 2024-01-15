#pragma once

#include "Engine/BiMap.h"
#include "Engine/Renderer.h"

#include <deque>


namespace Project001
{
    class OpenGL_Shader;
    class OpenGL_Texture;

    class OpenGL_RendererAlt : public Renderer
    {
    public:
        OpenGL_RendererAlt(Window* windowPtr, unsigned int width, unsigned int height);
        ~OpenGL_RendererAlt() override;

        void SetDepthTesting(
            bool depthTesting) override;

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override;

        void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) override;

        void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override;

        bool CreateTexture(
            unsigned int& textureId,
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool mipMaps) override;

        bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit) override;

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

        // returns false if the textureId isn't found or all textureUnits are
        //         in use or if the mesh is too big to fit in a single buffer
        bool AddMesh(
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
            bool lit) override;

        void PrepareCapabilities() override;

        void Clear() override;

        void Render() override;

        void SwapBuffers() override;

    protected:
        void CreateFramebuffer();

        void RenderTriangles(const std::vector<VertexData>& vertexBuffer);

        // returns 0 if texture successfuly bound to unit
        // returns 1 if texture failed to bind because all texture units are
        //           occupied
        // returns 2 if texture failed to bind because textureId doesn't exist
        int GetTextureUnit(unsigned int textureId, float& textureUnit);

        bool GetStalestTextureUnit(unsigned int& textureUnit) const;
        void IncreaseTectureUnitStaleness();

        static const unsigned int s_bufferCapacity_;

        static const unsigned int s_numberOfTextureUnits_;

        static const unsigned int s_numberOfPointLights_;
        static const unsigned int s_numberOfSpotLights_;

        Window* windowPtr_;

        bool depthTesting_;

        OpenGL_Shader* primaryShaderPtr_;
        OpenGL_Shader* screenShaderPtr_;

        unsigned int vertexBufferId_;
        unsigned int vertexArrayId_;

        unsigned int screenVertexBufferId_;
        unsigned int screenVertexArrayId_;

        unsigned int frameBufferWidth_;
        unsigned int frameBufferHeight_;

        unsigned int viewportX_;
        unsigned int viewportY_;
        unsigned int viewportWidth_;
        unsigned int viewportHeight_;

        unsigned int frameBufferId_;
        unsigned int screenTextureColorBufferId_;

        unsigned int renderBufferId_;

        std::deque<unsigned int> recycledTextureIds_;
        std::map<unsigned int, OpenGL_Texture*> texturePtrMap_;
        BiMap<unsigned int, unsigned int> textureIdToUnitBiMap_;
        std::vector<unsigned int> tectureUnitStalenessValues_;

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;
        glm::mat4 projectionMatrix_;

        DirectionalLight directionalLight_;
        std::vector<PointLight> pointLights_;
        std::vector<SpotLight> spotLights_;

        std::vector<VertexData> vertexBuffer_;
        std::vector<VertexData> translucentVertexBuffer_;

    private:
    };

    inline void OpenGL_RendererAlt::SetDepthTesting(bool depthTesting)
    {
        depthTesting_ = depthTesting;
    }

    inline void OpenGL_RendererAlt::GetViewport(
        unsigned int& x,
        unsigned int& y,
        unsigned int& width,
        unsigned int& height)
    {
        x = viewportX_;
        y = viewportY_;
        width = viewportWidth_;
        height = viewportHeight_;
    }

    inline void OpenGL_RendererAlt::SetViewport(
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

    inline void OpenGL_RendererAlt::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void OpenGL_RendererAlt::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void OpenGL_RendererAlt::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }

    inline void OpenGL_RendererAlt::SetDirectionalLight(
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

    inline void OpenGL_RendererAlt::AddPointLight(
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

    inline void OpenGL_RendererAlt::AddSpotLight(
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

    inline void OpenGL_RendererAlt::ClearDirectionalLight()
    {
        directionalLight_.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionalLight_.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    inline void OpenGL_RendererAlt::ClearPointLights()
    {
        pointLights_.clear();
    }

    inline void OpenGL_RendererAlt::ClearSpotLights()
    {
        spotLights_.clear();
    }
}