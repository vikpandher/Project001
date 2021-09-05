#pragma once

#include "Engine/BiMap.h"
#include "Engine/Renderer.h"



struct GLFWwindow;

namespace Project001
{
    class OpenGLShader;
    class OpenGLTexture;

    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        virtual ~OpenGLRenderer() override;

        bool AddTexture(
            unsigned int textureIndex,
            unsigned int textureUnit,
            unsigned char* data,
            int width,
            int height,
            int numberOfComponents) override;

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

        void AddModel(
            MeshStores* meshStoresPtr,
            unsigned int meshIndex,
            unsigned int textureIndex,
            unsigned int specularIndex,
            float shininess,
            const glm::vec4& color,
            bool translucent,
            const glm::vec3& scale,
            const glm::vec3& position,
            const glm::quat& orientation) override;

        void ClearBuffers() override;

        void Render() override;

    protected:
        void CheckAndMakeContextCurrent();

        void RenderTriangles(const std::vector<VertexData>& vertexBuffer);

        // determines the size of the index and vertex buffers
        static const unsigned int s_bufferCapacity_ = 36 * 10;

        static const unsigned int s_numberOfTextureSlots_ = 16;

        static const unsigned int s_numberOfPointLights_ = 8;
        static const unsigned int s_numberOfSpotLights_ = 4;

        GLFWwindow* glfwWindowPtr_;

        bool isCurrentContext_;

        OpenGLShader* shaderPtr_;

        // this holds the buffer's id
        // the buffer holds the blob of data that will be displayed
        unsigned int vertexBufferId_;

        // this holds the vertex array's id
        // the vertex array holds information about the vertex attribute
        // locations
        unsigned int vertexArrayId_;

        std::map<unsigned int, OpenGLTexture*> texturePtrMap_;
        BiMap<unsigned int, unsigned int> textureIndexToUnitBiMap_;

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

    inline void OpenGLRenderer::ClearBuffers()
    {
        vertexBuffer_.clear();
        translucentVertexBuffer_.clear();
    }
}