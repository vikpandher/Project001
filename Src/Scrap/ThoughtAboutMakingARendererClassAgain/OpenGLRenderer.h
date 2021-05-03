#pragma once

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
        ~OpenGLRenderer();
        
        void Render() const override;

        bool AddTexture(unsigned int textureSlot, unsigned char* data, int width, int height, int numberOfComponents) override;

        void AddVerticies(const VertexData* verticies, unsigned int count) override;
        
        void ClearVerticies() override;

        void SetProjectionMatrix(const glm::mat4& projectionMatrix) const override;

        void SetViewMatrix(const glm::mat4& viewMatrix) const override;

        void SetViewPosition(const glm::vec3& viewPosition) const override;

        void SetDirectionalLight(const DirectionalLight& directionalLight) const override;

        bool SetPointLight(const PointLight& pointLight, unsigned int index) const override;

        bool SetSpotLight(const SpotLight& spotLight, unsigned int index) const override;

    protected:
        // determines the size of the index and vertex buffers
        static const unsigned int s_bufferCapacity_ = 36 * 5;
        static const unsigned int s_numberOfTextureSlots_ = 16;

        static const unsigned int s_numberOfPointLights_ = 8;
        static const unsigned int s_numberOfSpotLights_ = 4;

        GLFWwindow* glfwWindowPtr_;

        OpenGLShader* shaderPtr_;

        // this holds the buffer's id
        // the buffer holds the blob of data that will be displayed
        unsigned int vertexBufferId_;

        // this holds the index buffer's id
        unsigned int indexBufferId_;

        // this holds the vertex array's id
        // the vertex array holds information about the size, shape, and type of array
        unsigned int vertexArrayId_;

        OpenGLTexture* texturePtrs_[s_numberOfTextureSlots_];

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;

        glm::mat4 projectionMatrix_;

        DirectionalLight directionalLight;
        PointLight pointLights_[s_numberOfPointLights_];
        SpotLight spotLights_[s_numberOfSpotLights_];

        std::vector<VertexData> vertexBuffer;
        std::vector<glm::uint> indexBuffer;

    private:
    };
}