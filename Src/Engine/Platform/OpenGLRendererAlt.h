#pragma once

#include "Engine/BiMap.h"
#include "Engine/Renderer.h"



struct GLFWwindow;

namespace Project001
{
    class OpenGLShader;
    class OpenGLTexture;

    class OpenGLRendererAlt : public Renderer
    {
    public:
        OpenGLRendererAlt(unsigned int width, unsigned int height);
        virtual ~OpenGLRendererAlt() override;

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

        void ClearBuffers() override;

        void Render() override;

        void SwapBuffers() override;

    protected:
        void CheckAndMakeContextCurrent();

        void CreateFramebuffer();

        static const bool s_drawWireframe = true;
        static const bool s_drawNormals = true;

        static const unsigned int s_indexBufferCapacity_ = 4194304; // 8192;
        static const unsigned int s_vertexBufferCapacity_ = 4194304; // 6144;

        static const unsigned int s_numberOfTextureSlots_ = 16;

        static const unsigned int s_numberOfPointLights_ = 8;
        static const unsigned int s_numberOfSpotLights_ = 4;

        GLFWwindow* glfwWindowPtr_;

        bool isCurrentContext_;

        OpenGLShader* primaryShaderPtr_;
        OpenGLShader* wireframeShaderPtr_;
        OpenGLShader* normalShaderPtr_;
        OpenGLShader* screenShaderPtr_;

        unsigned int vertexBufferId_;
        unsigned int indexBufferId_;
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

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;
        glm::mat4 projectionMatrix_;

        DirectionalLight directionalLight_;
        std::vector<PointLight> pointLights_;
        std::vector<SpotLight> spotLights_;

        std::vector<VertexData> vertexBuffer_;
        std::vector<unsigned int> indexBuffer_;

    private:
    };

    inline void OpenGLRendererAlt::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void OpenGLRendererAlt::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void OpenGLRendererAlt::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }

    inline void OpenGLRendererAlt::SetDirectionalLight(
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

    inline void OpenGLRendererAlt::AddPointLight(
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

    inline void OpenGLRendererAlt::AddSpotLight(
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

    inline void OpenGLRendererAlt::ClearDirectionalLight()
    {
        directionalLight_.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionalLight_.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    inline void OpenGLRendererAlt::ClearPointLights()
    {
        pointLights_.clear();
    }

    inline void OpenGLRendererAlt::ClearSpotLights()
    {
        spotLights_.clear();
    }

    inline void OpenGLRendererAlt::ClearBuffers()
    {
        vertexBuffer_.clear();
        indexBuffer_.clear();
    }
}