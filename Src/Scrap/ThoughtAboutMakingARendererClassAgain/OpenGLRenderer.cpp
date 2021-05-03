#include "Platform/OpenGLRenderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Logger.h"

#include "Platform/OpenGLShader.h"
#include "Platform/OpenGLTexture.h"
#include "Platform/Shaders.h"



namespace Project001
{
    OpenGLRenderer::OpenGLRenderer()
    {
        glfwGetWindowUserPointer(glfwWindowPtr_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Error("Failed to initialize Glad!");
        }

        Logger::Message("OpenGL Info:");
        Logger::Message("    Vendor: %s", glGetString(GL_VENDOR));
        Logger::Message("    Renderer: %s", glGetString(GL_RENDERER));
        Logger::Message("    Version: %s", glGetString(GL_VERSION));

        // This is enabled so when glViewport is used to set the viewportt size,
        // glScissor can be used to limit drawing to within the viewport.
        glEnable(GL_SCISSOR_TEST);

        // enable using the z buffer
        glEnable(GL_DEPTH_TEST);

        // enable writing to the depth buffer
        glDepthMask(GL_TRUE);

        // draw as wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // cull backfaces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        shaderPtr_ = new OpenGLShader(g_vertexShaderSource01_, g_fragmentShaderSource01_);

        // NOTE:
        // glBindVertex Array doesn't ALWAYS need to come before glBindBuffer,
        // but there are situations when it does.
        //
        // OpenGL calls are executred in the order they are issued. So
        // glVertexArrtribPointer interprets parameters relative to the
        // currenlty bound buffer.
        //
        // There is an exception to this. Binding the GL_ELEMENT_ARRAY_BUFFER is
        // saved to the current Vertex Array Object at the time it is bound. The
        // VAO must be bound first.

        // ---------------------------------------------------------------------
        // generate an id (name) for a new vertex array object
        glGenVertexArrays(1, &vertexArrayId_);

        // generate an id (name) for a vertex buffer object
        glGenBuffers(1, &vertexBufferId_);

        // generate an id (name) for the index buffer
        glGenBuffers(1, &indexBufferId_);

        // make the vertex arrat object active, create it if necessary
        glBindVertexArray(vertexArrayId_);

        // make the buffer the active array buffer, creating it if necessary
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

        // set the size of the active array buffer
        // (target, size, data, usage)
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        // attach the active buffer to the active array with the given attributes
        // (index, size, type, normalized, stride, pointer)

        GLuint positionAttributeIndex = 0;
        GLuint textureCoordinateAttributeIndex = 1;
        GLuint normalAttributeIndex = 2;
        GLuint colorAttributeIndex = 3;
        GLuint textureIndexAttributeIndex = 4;
        GLuint specularIndexAttributeIndex = 5;
        GLuint shininessAttributeIndex = 6;
        GLuint translationAttributeIndex = 7;
        GLuint orientationAttributeIndex = 8;

        unsigned long long attributeOffset = 0;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(positionAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureCoordinateAttributeIndex);
        attributeOffset += sizeof(glm::vec2);

        glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(normalAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(colorAttributeIndex);
        attributeOffset += sizeof(glm::vec4);

        glVertexAttribPointer(textureIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureIndexAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularIndexAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularIndexAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(shininessAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(translationAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(orientationAttributeIndex);
        // attributeOffset += sizeof(glm::quat);

        // make the buffer the active element array buffer, create it if necessary
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        // set the size of the active element array buffer
        // (target, size, data, usage)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * s_bufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        // unbind the array buffer and the vertex array
        ///glBindBuffer(GL_ARRAY_BUFFER, 0);
        ///glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ///glBindVertexArray(0);

        shaderPtr_->Use();

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            texturePtrs_[i] = nullptr;

            std::string uniformName;
            uniformName.append("textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            shaderPtr_->SetInt(uniformName.c_str(), i);
        }
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        delete shaderPtr_;

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);
        glDeleteVertexArrays(1, &indexBufferId_);

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            delete texturePtrs_[i];
        }
    }

    void OpenGLRenderer::Render() const
    {
        glfwMakeContextCurrent(glfwWindowPtr_);


    }

    bool OpenGLRenderer::AddTexture(unsigned int textureSlot, unsigned char* data, int width, int height, int numberOfComponents)
    {
        glfwMakeContextCurrent(glfwWindowPtr_);

        if (textureSlot >= 0 && textureSlot < s_numberOfTextureSlots_)
        {
            OpenGLTexture*& texturePtr = texturePtrs_[textureSlot];
            if (texturePtr != nullptr)
            {
                delete texturePtr;
            }
            texturePtr = new OpenGLTexture(textureSlot, data, width, height, numberOfComponents);
        }
    }

    void OpenGLRenderer::AddVerticies(const VertexData* verticies, unsigned int count)
    {

    }

    void OpenGLRenderer::ClearVerticies()
    {

    }

    void OpenGLRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix) const
    {

    }

    void OpenGLRenderer::SetViewMatrix(const glm::mat4& viewMatrix) const
    {

    }

    void OpenGLRenderer::SetViewPosition(const glm::vec3& viewPosition) const
    {

    }

    void OpenGLRenderer::SetDirectionalLight(const DirectionalLight& directionalLight) const
    {

    }

    bool OpenGLRenderer::SetPointLight(const PointLight& pointLight, unsigned int index) const
    {

    }

    bool OpenGLRenderer::SetSpotLight(const SpotLight& spotLight, unsigned int index) const
    {

    }
}