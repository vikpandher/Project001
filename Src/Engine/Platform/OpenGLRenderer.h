#pragma once

#include "Engine/BiMap.h"
#include "Engine/Renderer.h"



namespace Project001
{
    class OpenGLShader;
    class OpenGLTexture;

    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer(unsigned int width, unsigned int height);
        ~OpenGLRenderer() override;

        void SetDepthTesting(
            bool depthTesting) override;

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override;

        void SetViewportSize(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override;

        bool AddTexture(
            unsigned int textureIndex,
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int numberOfComponents) override;

        bool BindTexture(
            unsigned int textureIndex,
            unsigned int textureUnit) override;

        void ClearTextures() override;

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

        bool AddMesh(
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
            bool lit) override;

        void PrepareCapabilities() override;

        void Clear() override;

        void Render() override;

    protected:
        void CreateFramebuffer();

        void RenderTriangles(const std::vector<VertexData>& vertexBuffer);

        bool GetTextureUnit(unsigned int textureIndex, float& textureUnit);

        bool GetStalestTextureUnit(unsigned int& textureUnit) const;
        void IncreaseTectureUnitStaleness();

        static const unsigned int s_bufferCapacity_ = 36 * 10;

        static const unsigned int s_numberOfTextureUnits_ = 16;

        static const unsigned int s_numberOfPointLights_ = 8;
        static const unsigned int s_numberOfSpotLights_ = 4;

        bool depthTesting_;

        OpenGLShader* primaryShaderPtr_;
        OpenGLShader* screenShaderPtr_;

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

        std::map<unsigned int, OpenGLTexture*> texturePtrMap_;
        BiMap<unsigned int, unsigned int> textureIndexToUnitBiMap_;
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

    inline void OpenGLRenderer::SetDepthTesting(bool depthTesting)
    {
        depthTesting_ = depthTesting;
    }

    inline void OpenGLRenderer::SetViewportSize(
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

    inline void OpenGLRenderer::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void OpenGLRenderer::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void OpenGLRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }

    inline void OpenGLRenderer::SetDirectionalLight(
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

    inline void OpenGLRenderer::AddPointLight(
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

    inline void OpenGLRenderer::AddSpotLight(
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

    inline void OpenGLRenderer::ClearDirectionalLight()
    {
        directionalLight_.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionalLight_.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    inline void OpenGLRenderer::ClearPointLights()
    {
        pointLights_.clear();
    }

    inline void OpenGLRenderer::ClearSpotLights()
    {
        spotLights_.clear();
    }
}