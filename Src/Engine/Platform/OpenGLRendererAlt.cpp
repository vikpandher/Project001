#include "OpenGLRendererAlt.h"

#include <algorithm>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Engine/Logger.h"
#include "Engine/MeshVertex.h"

#include "Engine/Platform/OpenGLShader.h"
#include "Engine/Platform/OpenGLTexture.h"
#include "Engine/Platform/ShaderSource/BatchShaderSource.h"
#include "Engine/Platform/ShaderSource/NormalShaderSource.h"
#include "Engine/Platform/ShaderSource/ScreenShaderSource.h"
#include "Engine/Platform/ShaderSource/WireFrameShaderSource.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGLRendererAlt::OpenGLRendererAlt(unsigned int width, unsigned int height)
        : isCurrentContext_(true)
        , depthTesting_(true)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
    {
        glfwWindowPtr_ = glfwGetCurrentContext();

        if (glHint == 0) // OpenGL functions need to be loaded
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                _LOG_ERROR("Failed to initialize Glad!");
            }

            _LOG_MESSAGE("OpenGL Info:");
            _LOG_MESSAGE("    Vendor: %s", glGetString(GL_VENDOR));
            _LOG_MESSAGE("    Renderer: %s", glGetString(GL_RENDERER));
            _LOG_MESSAGE("    Version: %s", glGetString(GL_VERSION));
            _LOG_MESSAGE("    Shading Language Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        }

        // This is enabled so when glViewport is used to set the viewport size,
        // glScissor can be used to limit drawing to within the viewport.
        // glEnable(GL_SCISSOR_TEST);

        // enable writing to the depth buffer
        glDepthMask(GL_TRUE);

        // cull backfaces
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * s_vertexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

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
        GLuint litAttributeIndex = 10;

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
        attributeOffset += sizeof(glm::quat);

        glVertexAttribPointer(litAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(litAttributeIndex);
        // attributeOffset += sizeof(float);

        // make the buffer the active element array buffer, create it if necessary
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        if (s_indexBufferCapacity_ % 3 != 0)
        {
            _LOG_ERROR("Index Buffer Size Not Multiple Of 3");
        }

        // set the size of the active element array buffer
        // (target, size, data, usage)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * s_indexBufferCapacity_, NULL, GL_DYNAMIC_DRAW);

        float screenQuadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &screenVertexArrayId_);
        glGenBuffers(1, &screenVertexBufferId_);
        glBindVertexArray(screenVertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, screenVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(1);

        primaryShaderPtr_ = new OpenGLShader(
            BatchShader::g_vertexShaderSource,
            BatchShader::g_fragmentShaderSource
        );
        primaryShaderPtr_->Use();

        for (int i = 0; i < s_numberOfTextureSlots_; ++i)
        {
            std::string uniformName;
            uniformName.append("u_Textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            primaryShaderPtr_->SetInt(uniformName.c_str(), i);
        }

        wireframeShaderPtr_ = new OpenGLShader(
            WireFrameShader::g_vertexShaderSource,
            WireFrameShader::g_geometryShaderSource,
            WireFrameShader::g_fragmentShaderSource
        );

        normalShaderPtr_ = new OpenGLShader(
            NormalShader::g_vertexShaderSource,
            NormalShader::g_geometryShaderSource,
            NormalShader::g_fragmentShaderSource
        );

        screenShaderPtr_ = new OpenGLShader(
            ScreenShader::g_vertexShaderSource,
            ScreenShader::g_fragmentShaderSource
        );
        screenShaderPtr_->Use();
        screenShaderPtr_->SetInt("u_ScreenTexture", 0);

        CreateFramebuffer();

        viewportX_ = 0;
        viewportY_ = 0;
        viewportWidth_ = frameBufferWidth_;
        viewportHeight_ = frameBufferHeight_;

        pointLights_.reserve(s_numberOfPointLights_);
        spotLights_.reserve(s_numberOfSpotLights_);
    }

    OpenGLRendererAlt::~OpenGLRendererAlt()
    {
        delete primaryShaderPtr_;
        delete wireframeShaderPtr_;
        delete normalShaderPtr_;
        delete screenShaderPtr_;

        ClearTextures();

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteBuffers(1, &indexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        glDeleteBuffers(1, &screenVertexBufferId_);
        glDeleteVertexArrays(1, &screenVertexArrayId_);
    }

    void OpenGLRendererAlt::SetDepthTesting(
        bool depthTesting)
    {
        depthTesting_ = depthTesting;

        if (depthTesting_)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void OpenGLRendererAlt::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        CheckAndMakeContextCurrent();

        frameBufferWidth_ = width;
        frameBufferHeight_ = height;

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        CreateFramebuffer();
    }

    void OpenGLRendererAlt::SetViewportSize(
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

    bool OpenGLRendererAlt::AddTexture(
        unsigned int textureIndex,
        unsigned int textureUnit,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int numberOfComponents)
    {
        if (textureUnit < s_numberOfTextureSlots_ && textureUnit > 0) // temp. reserving 0 for the screenTexture
        {
            CheckAndMakeContextCurrent();

            if (texturePtrMap_.find(textureIndex) != texturePtrMap_.end())
            {
                delete texturePtrMap_[textureIndex];
            }

            texturePtrMap_[textureIndex] = new OpenGLTexture(textureUnit, data, width, height, numberOfComponents);
            textureIndexToUnitBiMap_.Add(textureIndex, textureUnit);

            return true;
        }
        else
        {
            return false;
        }
    }

    bool OpenGLRendererAlt::BindTexture(
        unsigned int textureIndex,
        unsigned int textureUnit)
    {
        if (textureUnit < s_numberOfTextureSlots_ && textureUnit > 0) // temp. reserving 0 for the screenTexture
        {

            if (texturePtrMap_.find(textureIndex) != texturePtrMap_.end())
            {
                texturePtrMap_[textureIndex]->Bind(textureUnit);
                textureIndexToUnitBiMap_.Add(textureIndex, textureUnit);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    void OpenGLRendererAlt::ClearTextures()
    {
        for (std::map<unsigned int, OpenGLTexture*>::iterator iter = texturePtrMap_.begin();
            iter != texturePtrMap_.end(); ++iter)
        {
            delete iter->second;
        }
        texturePtrMap_.clear();
        textureIndexToUnitBiMap_.Clear();
    }

    bool OpenGLRendererAlt::AddMesh(
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
        bool lit)
    {
        if (translucent)
        {
            _LOG_ERROR("OpenGLRendererAlt doesn't handle translucent meshes");
        }

        if ((vertexBuffer_.size() + meshVertexCount) < s_vertexBufferCapacity_ &&
            (indexBuffer_.size() + meshIndexCount) < s_indexBufferCapacity_)
        {
            float textureSlot = -1.0f;
            if (textureIndexToUnitBiMap_.Find_X(textureIndex)) // convert textureIndex to textureSlot
            {
                textureSlot = (float)textureIndexToUnitBiMap_.Get_Using_X(textureIndex);
            }

            float specularSlot = -1.0f;
            if (textureIndexToUnitBiMap_.Find_X(specularIndex)) // convert specularIndex to textureSlot
            {
                specularSlot = (float)textureIndexToUnitBiMap_.Get_Using_X(specularIndex);
            }

            unsigned int vertexBufferOffset = (unsigned int)vertexBuffer_.size();

            for (size_t j = 0; j < meshVertexCount; ++j)
            {
                const Project001::MeshVertex& currentMeshVertex = meshVerticies[j];

                Project001::VertexData newVertex;
                newVertex.position = currentMeshVertex.position;
                newVertex.textureCoordinate = currentMeshVertex.textureCoordinate;
                newVertex.normal = currentMeshVertex.normal;
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
                newVertex.lit = lit;

                vertexBuffer_.push_back(newVertex);
            }

            for (unsigned int j = 0; j < meshIndexCount; ++j)
            {
                indexBuffer_.push_back(vertexBufferOffset + meshIndicies[j]);
            }

            return true;
        }

        return false;
    }

    void OpenGLRendererAlt::Render()
    {
        CheckAndMakeContextCurrent();

        // Render to texture frameBuffer
        // ---------------------------------------------------------------------

        glViewport(0, 0, frameBufferWidth_, frameBufferHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        if (depthTesting_)
        {
            // enable using the z buffer
            glEnable(GL_DEPTH_TEST);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        primaryShaderPtr_->Use();

        // Camera
        // ---------------------------------------------------------------------

        primaryShaderPtr_->SetMat4("u_View", viewMatrix_);
        primaryShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
        primaryShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

        // Directional Light
        // ---------------------------------------------------------------------

        primaryShaderPtr_->SetVec3("u_DirectionalLight.direction", directionalLight_.direction);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.ambient", directionalLight_.ambient);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.diffuse", directionalLight_.diffuse);
        primaryShaderPtr_->SetVec3("u_DirectionalLight.specular", directionalLight_.specular);

        // Point Lights
        // ---------------------------------------------------------------------

        std::string stringPrefix = "u_PointLights[";
        unsigned int i = 0;
        while (i < pointLights_.size())
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI +  "].position").c_str(), pointLights_[i].position);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), pointLights_[i].constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), pointLights_[i].linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), pointLights_[i].quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), pointLights_[i].ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), pointLights_[i].diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), pointLights_[i].specular);
            ++i;
        }
        PointLight emptyPointLight;
        while (i < s_numberOfPointLights_)
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptyPointLight.position);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptyPointLight.constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptyPointLight.linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptyPointLight.quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptyPointLight.ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptyPointLight.diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptyPointLight.specular);
            ++i;
        }

        // Spot Lights
        // ---------------------------------------------------------------------

        stringPrefix = "u_SpotLights[";
        i = 0;
        while (i < spotLights_.size())
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), spotLights_[i].position);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), spotLights_[i].direction);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), spotLights_[i].cutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), spotLights_[i].outerCutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), spotLights_[i].constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), spotLights_[i].linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), spotLights_[i].quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), spotLights_[i].ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), spotLights_[i].diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), spotLights_[i].specular);
            ++i;
        }
        SpotLight emptySpotLight;
        while (i < s_numberOfSpotLights_)
        {
            std::string stringI = std::to_string(i);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), emptySpotLight.position);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].direction").c_str(), emptySpotLight.direction);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].cutoff").c_str(), emptySpotLight.cutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].outerCutoff").c_str(), emptySpotLight.outerCutoff);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].constant").c_str(), emptySpotLight.constant);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].linear").c_str(), emptySpotLight.linear);
            primaryShaderPtr_->SetFloat((stringPrefix + stringI + "].quadratic").c_str(), emptySpotLight.quadratic);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].ambient").c_str(), emptySpotLight.ambient);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].diffuse").c_str(), emptySpotLight.diffuse);
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].specular").c_str(), emptySpotLight.specular);
            ++i;
        }

        glBindVertexArray(vertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        if (!vertexBuffer_.empty() && !indexBuffer_.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexData) * vertexBuffer_.size(), &vertexBuffer_[0]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indexBuffer_.size(), &indexBuffer_[0]);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);


        if (s_drawWireframe)
        {
            wireframeShaderPtr_->Use();

            wireframeShaderPtr_->SetMat4("u_View", viewMatrix_);
            wireframeShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            wireframeShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);
            wireframeShaderPtr_->SetVec4("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        if (s_drawNormals)
        {
            normalShaderPtr_->Use();

            normalShaderPtr_->SetFloat("u_Magnitude", 0.16f);
            normalShaderPtr_->SetMat4("u_View", viewMatrix_);
            normalShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            normalShaderPtr_->SetVec4("u_StartColor", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
            normalShaderPtr_->SetVec4("u_EndColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

            glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        // Render to screen frameBuffer
        // ---------------------------------------------------------------------

        glViewport(viewportX_, viewportY_, viewportWidth_, viewportHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShaderPtr_->Use();

        glBindVertexArray(screenVertexArrayId_);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void OpenGLRendererAlt::SwapBuffers()
    {
        CheckAndMakeContextCurrent();

        // Uses default platform swap interval, usually 60 fps
        glfwSwapBuffers(glfwWindowPtr_);
    }

    // protected ---------------------------------------------------------------

    void OpenGLRendererAlt::CheckAndMakeContextCurrent()
    {
        if (!isCurrentContext_)
        {
            glfwMakeContextCurrent(glfwWindowPtr_);
            isCurrentContext_ = true;
        }
    }

    void OpenGLRendererAlt::CreateFramebuffer()
    {
        glGenFramebuffers(1, &frameBufferId_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        glGenTextures(1, &screenTextureColorBufferId_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTextureColorBufferId_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferWidth_, frameBufferHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTextureColorBufferId_, 0);

        glGenRenderbuffers(1, &renderBufferId_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferWidth_, frameBufferHeight_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId_);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            // Log Error
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}