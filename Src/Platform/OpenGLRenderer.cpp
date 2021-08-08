#include "OpenGLRenderer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Logger.h"
#include "Engine/ModelStores.h"
#include "Engine/TextureStores.h"

#include "Platform/OpenGLShader.h"
#include "Platform/OpenGLTexture.h"
#include "Platform/Shaders.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLRenderer::OpenGLRenderer(ModelStores* modelStoresPtr, TextureStores* textureStoresPtr)
        : modelStoresPtr_(modelStoresPtr)
        , textureStoresPtr_(textureStoresPtr)
        , isCurrentContext_(true)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
    {
        glfwWindowPtr_ = glfwGetCurrentContext();

        // This is enabled so when glViewport is used to set the viewport size,
        // glScissor can be used to limit drawing to within the viewport.
        // glEnable(GL_SCISSOR_TEST);

        // enable using the z buffer
        glEnable(GL_DEPTH_TEST);

        // enable writing to the depth buffer
        glDepthMask(GL_TRUE);

        // draw as wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // cull backfaces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // blending
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        GLuint textureSlotAttributeIndex = 4;
        GLuint specularSlotAttributeIndex = 5;
        GLuint shininessAttributeIndex = 6;
        GLuint scaleAttributeIndex = 7;
        GLuint translationAttributeIndex = 8;
        GLuint orientationAttributeIndex = 9;

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

        glVertexAttribPointer(textureSlotAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureSlotAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularSlotAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularSlotAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(shininessAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(scaleAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(scaleAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(translationAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(orientationAttributeIndex);
        // attributeOffset += sizeof(glm::quat);

        // unbind the array buffer and the vertex array
        ///glBindBuffer(GL_ARRAY_BUFFER, 0);
        ///glBindVertexArray(0);

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            texturePtrs_[i] = nullptr;
        }

        pointLights_.reserve(s_numberOfPointLights_);
        spotLights_.reserve(s_numberOfSpotLights_);

        shaderPtr_ = new OpenGLShader(g_vertexShaderSource01_, g_fragmentShaderSource01_);
        shaderPtr_->Use();

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            std::string uniformName;
            uniformName.append("u_Textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            shaderPtr_->SetInt(uniformName.c_str(), i);
        }
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        delete shaderPtr_;

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            delete texturePtrs_[i];
        }

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);
    }

    // TODO: Create some sort of texture LRU cache so I don't have to track texture slots.
    void OpenGLRenderer::AddTexture(
        unsigned int textureSlot,
        unsigned char* data,
        int width,
        int height,
        int numberOfComponents)
    {
        CheckAndMakeContextCurrent();

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

    void OpenGLRenderer::AddModel(
        const unsigned int& modelIndex,
        const unsigned int& textureIndex,
        const unsigned int& specularIndex,
        const float& shininess,
        const glm::vec4& color,
        const glm::vec3& scale,
        const glm::vec3& position,
        const glm::quat& orientation)
    {
        ModelVertex* modelVerticies;
        glm::uint modelVertexCount;
        glm::uint* modelIndicies;
        glm::uint modelIndexCount;

        if (modelStoresPtr_->GetModel(modelIndex, modelVerticies, modelVertexCount, modelIndicies, modelIndexCount))
        {
            float textureSlot = -1.0f;
            if (textureIndex != (unsigned int)-1) // convert textureIndex to textureSlot
            {
                textureSlot = (float)textureIndex;
            }

            float specularSlot = 99.0f;
            if (specularIndex != (unsigned int)-1) // convert specularIndex to specularSlot
            {
                specularSlot = (float)specularIndex;
            }

            glm::uint vertexBufferOffset = (glm::uint)vertexBuffer_.size();

            for (unsigned int j = 0; j < modelVertexCount; ++j)
            {
                Project001::ModelVertex& currentModelVertex = modelVerticies[j];

                Project001::VertexData newVertex;
                newVertex.position = currentModelVertex.position;
                newVertex.textureCoordinte = currentModelVertex.textureCoordinte;
                newVertex.normal = currentModelVertex.normal;
                newVertex.color = color;
                newVertex.textureSlot = textureSlot;
                newVertex.specularSlot = specularSlot;
                newVertex.shininess = shininess;
                newVertex.scale = scale;
                newVertex.translation = position;
                newVertex.orientation.x = orientation.x;
                newVertex.orientation.y = orientation.y;
                newVertex.orientation.z = orientation.z;
                newVertex.orientation.w = orientation.w;

                vertexBuffer_.push_back(newVertex);
            }
        }
    }

    void OpenGLRenderer::Render()
    {
        CheckAndMakeContextCurrent();

        // Camera
        // ---------------------------------------------------------------------

        shaderPtr_->SetMat4("u_View", viewMatrix_);
        shaderPtr_->SetMat4("u_Projection", projectionMatrix_);
        shaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

        // Directional Light
        // ---------------------------------------------------------------------

        shaderPtr_->SetVec3("u_DirectionalLight.direction", directionalLight_.direction);
        shaderPtr_->SetVec3("u_DirectionalLight.ambient", directionalLight_.ambient);
        shaderPtr_->SetVec3("u_DirectionalLight.diffuse", directionalLight_.diffuse);
        shaderPtr_->SetVec3("u_DirectionalLight.specular", directionalLight_.specular);

        // Point Lights
        // ---------------------------------------------------------------------

        std::string stringPrefix = "u_PointLights[";
        unsigned int i = 0;
        while (i < pointLights_.size())
        {
            std::string stringI = std::to_string(i);
            shaderPtr_->SetVec3((stringPrefix + stringI +  "].position").c_str(), pointLights_[i].position);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), pointLights_[i].constant);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), pointLights_[i].linear);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), pointLights_[i].quadratic);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), pointLights_[i].ambient);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), pointLights_[i].diffuse);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), pointLights_[i].specular);
            ++i;
        }
        PointLight emptyPointLight;
        while (i < s_numberOfPointLights_)
        {
            std::string stringI = std::to_string(i);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptyPointLight.position);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptyPointLight.constant);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptyPointLight.linear);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptyPointLight.quadratic);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptyPointLight.ambient);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptyPointLight.diffuse);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptyPointLight.specular);
            ++i;
        }

        // Spot Lights
        // ---------------------------------------------------------------------

        stringPrefix = "u_SpotLights[";
        i = 0;
        while (i < spotLights_.size())
        {
            std::string stringI = std::to_string(i);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), spotLights_[i].position);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), spotLights_[i].direction);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), spotLights_[i].cutoff);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), spotLights_[i].outerCutoff);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), spotLights_[i].constant);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), spotLights_[i].linear);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), spotLights_[i].quadratic);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), spotLights_[i].ambient);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), spotLights_[i].diffuse);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), spotLights_[i].specular);
            ++i;
        }
        SpotLight emptySpotLight;
        while (i < s_numberOfSpotLights_)
        {
            std::string stringI = std::to_string(i);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptySpotLight.position);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), emptySpotLight.direction);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), emptySpotLight.cutoff);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), emptySpotLight.outerCutoff);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptySpotLight.constant);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptySpotLight.linear);
            shaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptySpotLight.quadratic);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptySpotLight.ambient);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptySpotLight.diffuse);
            shaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptySpotLight.specular);
            ++i;
        }

        // Render
        // ---------------------------------------------------------------------

        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        size_t numberOfVerticiesLeft = vertexBuffer_.size();
        while (numberOfVerticiesLeft > 0)
        {
            size_t numberOfVerticiesDrawnThisDrawCall;
            if (numberOfVerticiesLeft > s_bufferCapacity_)
            {
                numberOfVerticiesDrawnThisDrawCall = s_bufferCapacity_;
            }
            else
            {
                numberOfVerticiesDrawnThisDrawCall = numberOfVerticiesLeft;
            }

            size_t verticiesOffset = vertexBuffer_.size() - numberOfVerticiesLeft;

            // upload the vertex data and index data into their respective buffers
            // (target, offset, size, data)
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexData) * numberOfVerticiesDrawnThisDrawCall, &vertexBuffer_[verticiesOffset]);

            /// glBindVertexArray(vertexArrayId_);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)numberOfVerticiesDrawnThisDrawCall);

            numberOfVerticiesLeft -= numberOfVerticiesDrawnThisDrawCall;
        }

        glfwSwapBuffers(glfwWindowPtr_);
    }

    // protected ---------------------------------------------------------------

    void OpenGLRenderer::CheckAndMakeContextCurrent()
    {
        if (!isCurrentContext_)
        {
            glfwMakeContextCurrent(glfwWindowPtr_);
            isCurrentContext_ = true;
        }
    }
}