#include "OpenGL_RendererAlt.h"

#include <algorithm>
#include <string>

#include "glad/glad.h"

#include "Engine/Logger.h"
#include "Engine/MeshVertex.h"

#include "Engine/Platform/OpenGL_Shader.h"
#include "Engine/Platform/OpenGL_Texture.h"
#include "Engine/Platform/ShaderSource/BatchShaderSource.h"
#include "Engine/Platform/ShaderSource/ScreenShaderSource.h"

#include "Engine/Window.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_RendererAlt::OpenGL_RendererAlt(Window* windowPtr, unsigned int width, unsigned int height)
        : windowPtr_(windowPtr)
        , depthTesting_(true)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
    {
        windowPtr_->MakeContextCurrent();

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
        GLuint textureUnitAttributeIndex = 4;
        GLuint specularUnitAttributeIndex = 5;
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

        glVertexAttribPointer(textureUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureUnitAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularUnitAttributeIndex);
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

        primaryShaderPtr_ = new OpenGL_Shader(
            BatchShader::g_vertexShaderSource,
            BatchShader::g_fragmentShaderSource
        );
        primaryShaderPtr_->Use();

        for (unsigned int i = 0; i < s_numberOfTextureUnits_; ++i)
        {
            std::string uniformName;
            uniformName.append("u_Textures[");
            uniformName.append(std::to_string(i));
            uniformName.append("]");
            primaryShaderPtr_->SetInt(uniformName.c_str(), i);
        }

        screenShaderPtr_ = new OpenGL_Shader(
            ScreenShader::g_vertexShaderSource,
            ScreenShader::g_fragmentShaderSource
        );
        screenShaderPtr_->Use();
        screenShaderPtr_->SetInt("u_ScreenTexture", 0);

        CreateFramebuffer();

        GLint viewPortData[4];
        glGetIntegerv(GL_VIEWPORT, viewPortData);
        viewportX_ = viewPortData[0];
        viewportY_ = viewPortData[1];
        viewportWidth_ = viewPortData[2];
        viewportHeight_ = viewPortData[3];

        tectureUnitStalenessValues_.push_back(0);
        tectureUnitStalenessValues_.resize(s_numberOfTextureUnits_, 1);

        pointLights_.reserve(s_numberOfPointLights_);
        spotLights_.reserve(s_numberOfSpotLights_);
    }

    OpenGL_RendererAlt::~OpenGL_RendererAlt()
    {
        windowPtr_->MakeContextCurrent();

        delete primaryShaderPtr_;
        delete screenShaderPtr_;

        DeleteAllTextures();

        glDeleteBuffers(1, &vertexBufferId_);
        glDeleteVertexArrays(1, &vertexArrayId_);

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        glDeleteBuffers(1, &screenVertexBufferId_);
        glDeleteVertexArrays(1, &screenVertexArrayId_);
    }

    void OpenGL_RendererAlt::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        windowPtr_->MakeContextCurrent();

        frameBufferWidth_ = width;
        frameBufferHeight_ = height;

        glDeleteRenderbuffers(1, &renderBufferId_);
        glDeleteTextures(1, &screenTextureColorBufferId_);
        glDeleteFramebuffers(1, &frameBufferId_);

        CreateFramebuffer();
    }

    bool OpenGL_RendererAlt::CreateTexture(
        unsigned int& textureId,
        unsigned int textureUnit,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel)
    {
        if (textureUnit < s_numberOfTextureUnits_ && textureUnit > 0) // reserving 0 for the screenTexture
        {
            if (recycledTextureIds_.empty())
            {
                textureId = (unsigned int)texturePtrMap_.size();
            }
            else
            {
                textureId = recycledTextureIds_.front();
                recycledTextureIds_.pop_front();
            }

            texturePtrMap_[textureId] = new OpenGL_Texture(textureUnit, data, width, height, bytesPerPixel);
            textureIdToUnitBiMap_.Add(textureId, textureUnit);

            return true;
        }
        else
        {
            return false;
        }
    }

    bool OpenGL_RendererAlt::BindTexture(
        unsigned int textureId,
        unsigned int textureUnit)
    {
        if (0 < textureUnit && textureUnit < s_numberOfTextureUnits_) // reserving 0 for the screenTexture
        {
            if (texturePtrMap_.find(textureId) != texturePtrMap_.end())
            {
                texturePtrMap_[textureId]->Bind(textureUnit);
                textureIdToUnitBiMap_.Add(textureId, textureUnit);
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

    bool OpenGL_RendererAlt::DeleteTexture(unsigned int textureId)
    {
        if (texturePtrMap_.find(textureId) != texturePtrMap_.end())
        {
            delete texturePtrMap_[textureId];
            textureIdToUnitBiMap_.Remove_Using_X(textureId);
            return true;
        }

        return false;
    }

    void OpenGL_RendererAlt::DeleteAllTextures()
    {
        for (std::map<unsigned int, OpenGL_Texture*>::iterator iter = texturePtrMap_.begin();
            iter != texturePtrMap_.end(); ++iter)
        {
            delete iter->second;
        }
        texturePtrMap_.clear();
        textureIdToUnitBiMap_.Clear();
        recycledTextureIds_.clear();
    }

    bool OpenGL_RendererAlt::AddMesh(
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
        bool lit)
    {
        if (meshVertexCount > s_bufferCapacity_)
        {
            return false;
        }

        float textureUnit = -1.0f;
        if (textureId != (unsigned int)-1 && GetTextureUnit(textureId, textureUnit))
        {
            return false;
        }

        float specularUnit = -1.0f;
        if (specularId != (unsigned int)-1 && GetTextureUnit(specularId, specularUnit))
        {
            return false;
        }

        for (size_t j = 0; j < meshVertexCount; ++j)
        {
            const MeshVertex& currentMeshVertex = meshVerticies[j];

            VertexData newVertex;
            newVertex.position = currentMeshVertex.position;
            newVertex.textureCoordinate = currentMeshVertex.textureCoordinate;
            newVertex.normal = currentMeshVertex.normal;
            newVertex.color = color;
            newVertex.textureUnit = textureUnit;
            newVertex.specularUnit = specularUnit;
            newVertex.shininess = shininess;
            newVertex.scale = scale;
            newVertex.translation = position;
            newVertex.orientation.x = orientation.x;
            newVertex.orientation.y = orientation.y;
            newVertex.orientation.z = orientation.z;
            newVertex.orientation.w = orientation.w;
            newVertex.lit = lit;

            if (translucent)
            {
                translucentVertexBuffer_.push_back(newVertex);
            }
            else
            {
                vertexBuffer_.push_back(newVertex);
            }
        }

        return true;
    }

    void OpenGL_RendererAlt::PrepareCapabilities()
    {
        windowPtr_->MakeContextCurrent();

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
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE);

        // drawing triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // bind the texture attached to frameBufferId_
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTextureColorBufferId_);
    }

    void OpenGL_RendererAlt::Clear()
    {
        windowPtr_->MakeContextCurrent();

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL_RendererAlt::Render()
    {
        windowPtr_->MakeContextCurrent();

        // Sort Translucent Faces' Verticies
        // ---------------------------------------------------------------------
        // Note that faces are sorted based on the average distance of their 3
        // verticies from the camera.
        // 
        // There are cases when translucency may not be rendered correctly:
        // * If triangles intersect.
        // * If trangles overlap in a way that some fragments of a closer
        //   triangle are farther from the camera then a farther triangle's
        //   fragments.
        // 
        // The second case makes it difficult to draw 3d models at certain
        // angles. Enabling backface culling avoids the second case.

        std::vector<std::pair<size_t, float>> faceDistances;
        for (size_t i = 2; i < translucentVertexBuffer_.size(); i += 3)
        {
            const VertexData& vertex1 = translucentVertexBuffer_[i - 2];
            const glm::vec3& position1 = vertex1.position;
            const glm::vec3& translation1 = vertex1.translation;

            const VertexData& vertex2 = translucentVertexBuffer_[i - 1];
            const glm::vec3& position2 = vertex2.position;
            const glm::vec3& translation2 = vertex2.translation;

            const VertexData& vertex3 = translucentVertexBuffer_[i];
            const glm::vec3& position3 = vertex3.position;
            const glm::vec3& translation3 = vertex3.translation;

            glm::vec3 facePosition = (position1 + translation1 + position2 + translation2 + position3 + translation3) / 3.0f;
            glm::vec3 viewToFace = facePosition - viewPosition_;
            float faceDistance = std::sqrtf(viewToFace.x * viewToFace.x + viewToFace.y * viewToFace.y + viewToFace.z * viewToFace.z);

            faceDistances.push_back(std::pair<size_t, float>(i / 3, faceDistance));
        }

        struct
        {
            bool operator()(std::pair<size_t, float> a, std::pair<size_t, float> b) const
            {
                return a.second > b.second;
            }
        } comparitor;

        std::sort(faceDistances.begin(), faceDistances.end(), comparitor);

        std::vector<VertexData> sortedTranslucentVertexBuffer;
        for (size_t i = 0; i < faceDistances.size(); ++i)
        {
            size_t currentVertexIndex = faceDistances[i].first * 3;
            sortedTranslucentVertexBuffer.push_back(translucentVertexBuffer_[currentVertexIndex]);
            sortedTranslucentVertexBuffer.push_back(translucentVertexBuffer_[currentVertexIndex + 1]);
            sortedTranslucentVertexBuffer.push_back(translucentVertexBuffer_[currentVertexIndex + 2]);
        }

        // Render to texture frameBuffer
        // ---------------------------------------------------------------------

        glViewport(0, 0, frameBufferWidth_, frameBufferHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        if (depthTesting_)
        {
            // enable using the z buffer
            glEnable(GL_DEPTH_TEST);
        }

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

        RenderTriangles(vertexBuffer_);

        RenderTriangles(sortedTranslucentVertexBuffer);

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

        // Clear local buffers
        // ---------------------------------------------------------------------

        IncreaseTectureUnitStaleness();

        vertexBuffer_.clear();
        translucentVertexBuffer_.clear();
    }

    void OpenGL_RendererAlt::SwapBuffers()
    {
        windowPtr_->SwapBuffers();
    }

    // protected ---------------------------------------------------------------

    void OpenGL_RendererAlt::CreateFramebuffer()
    {
        // generate an id (name) for a frame buffer object
        glGenFramebuffers(1, &frameBufferId_);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

        glGenTextures(1, &screenTextureColorBufferId_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTextureColorBufferId_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferWidth_, frameBufferHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // attach a texture image to a framebuffer object
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTextureColorBufferId_, 0);

        // generate an id (name) for a renderbuffer object
        glGenRenderbuffers(1, &renderBufferId_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId_);

        // NOTE:
        // Renderbuffer objects can be more efficient for use in off-screen
        // render projects, but it is important to realize when to use
        // renderbuffer objects and when to use textures.The general rule is
        // that if you never need to sample data from a specific buffer, it is
        // wise to use a renderbuffer object for that specific buffer. If you
        // need to sample data from a specific buffer like colors or depth
        // values, you should use a texture attachment instead.

        // establish the data storage, format and dimensions of the renderbuffer
        // object's image
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferWidth_, frameBufferHeight_);

        // attach a renderbuffer as the logical buffer of the framebuffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId_);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            // Log Error
            _LOG_ERROR("Framebuffer is not complete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGL_RendererAlt::RenderTriangles(const std::vector<VertexData>& vertexBuffer)
    {
        size_t numberOfVerticiesLeft = vertexBuffer.size();
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

            size_t verticiesOffset = vertexBuffer.size() - numberOfVerticiesLeft;

            // upload the vertex data and index data into their respective buffers
            // (target, offset, size, data)
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexData) * numberOfVerticiesDrawnThisDrawCall, &vertexBuffer[verticiesOffset]);

            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)numberOfVerticiesDrawnThisDrawCall);

            numberOfVerticiesLeft -= numberOfVerticiesDrawnThisDrawCall;
        }
    }

    int OpenGL_RendererAlt::GetTextureUnit(unsigned int textureId, float& textureUnit)
    {
        if (textureIdToUnitBiMap_.Find_X(textureId))
        {
            unsigned int textureUnit_uint = textureIdToUnitBiMap_.Get_Using_X(textureId);
            tectureUnitStalenessValues_[textureUnit_uint] = 0;
            textureUnit = (float)textureUnit_uint;
        }
        else
        {
            unsigned int newTextureUnit = 1;
            if (GetStalestTextureUnit(newTextureUnit))
            {
                if (BindTexture(textureId, newTextureUnit))
                {
                    textureUnit = (float)newTextureUnit;
                    tectureUnitStalenessValues_[newTextureUnit] = 0;
                }
                else
                {
                    // texture doesn't exist
                    return 2;
                }
            }
            else
            {
                // no more room for this texture
                return 1;
            }
        }

        return 0;
    }

    bool OpenGL_RendererAlt::GetStalestTextureUnit(unsigned int& textureUnit) const
    {
        unsigned int stalestValue = 0;

        for (unsigned int i = 1; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            if (tectureUnitStalenessValues_[i] > stalestValue)
            {
                textureUnit = i;
                stalestValue = tectureUnitStalenessValues_[i];
            }
        }

        return stalestValue > 0;
    }

    void OpenGL_RendererAlt::IncreaseTectureUnitStaleness()
    {
        for (size_t i = 1; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            tectureUnitStalenessValues_[i]++;
        }
    }

    const unsigned int OpenGL_RendererAlt::s_bufferCapacity_ = 36 * 113;

    const unsigned int OpenGL_RendererAlt::s_numberOfTextureUnits_ = 16;

    const unsigned int OpenGL_RendererAlt::s_numberOfPointLights_ = 8;
    const unsigned int OpenGL_RendererAlt::s_numberOfSpotLights_ = 4;
}