// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-08

#include "OpenGL_Renderer.h"

#include "Platform/OpenGL_Mesh.h"
#include "Platform/OpenGL_Shader.h"
#include "Platform/OpenGL_Texture.h"

#include "Platform/OpenGL_Shaders/Generated/o_grid_frag.h"
#include "Platform/OpenGL_Shaders/Generated/o_grid_vert.h"
#include "Platform/OpenGL_Shaders/Generated/o_normal_frag.h"
#include "Platform/OpenGL_Shaders/Generated/o_normal_geom.h"
#include "Platform/OpenGL_Shaders/Generated/o_normal_vert.h"
#include "Platform/OpenGL_Shaders/Generated/o_primary_frag.h"
#include "Platform/OpenGL_Shaders/Generated/o_primary_vert.h"
#include "Platform/OpenGL_Shaders/Generated/o_screen_frag.h"
#include "Platform/OpenGL_Shaders/Generated/o_screen_vert.h"
#include "Platform/OpenGL_Shaders/Generated/o_wireframe_frag.h"
#include "Platform/OpenGL_Shaders/Generated/o_wireframe_geom.h"
#include "Platform/OpenGL_Shaders/Generated/o_wireframe_vert.h"

#include "Logger.h"
#include "Window.h"

#include "glad/glad.h"

#include <string>



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_Renderer::OpenGL_Renderer(const RendererInfo& rendererInfo)
        : windowPtr_(rendererInfo.windowPtr)
        , redrawGrid_(true)
        , depthTesting_(rendererInfo.depthTesting)
        , multisampleAntiAliasing_(rendererInfo.multisampleAntiAliasing)
        , instanceBufferCapacity_(rendererInfo.instanceBufferCapacity)
        , batchedIndexBufferCapacity_(rendererInfo.batchedIndexBufferCapacity)
        , batchedVertexBufferCapacity_(rendererInfo.batchedVertexBufferCapacity)
        , frameBufferWidth_(rendererInfo.frameBufferWidth)
        , frameBufferHeight_(rendererInfo.frameBufferHeight)
        , cameraViewportX_(0.0f)
        , cameraViewportY_(0.0f)
        , cameraViewportWidth_(1.0f)
        , cameraViewportHeight_(1.0f)
        , borderColor_(0.1f, 0.1f, 0.1f, 1.0f)
        , clearColor_(0.0f, 0.0f, 0.0f, 1.0f)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
    {
        windowPtr_->MakeContextCurrent();

        SetInstanceBufferCapacity(instanceBufferCapacity_);

        SetBatchedIndexBufferCapacity(batchedIndexBufferCapacity_);
        SetBatchedVertexBufferCapacity(batchedVertexBufferCapacity_);

        if (s_drawGrid)
        {
            CreateGridBufferAndArray();
        }

        float screenQuadVertices[] = {
            // positions   // textureCoordinates
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &screenVertexArrayId_);
        glBindVertexArray(screenVertexArrayId_);

        glGenBuffers(1, &screenVertexBufferId_);
        glBindBuffer(GL_ARRAY_BUFFER, screenVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);

        const GLuint positionAttributeIndex = 0;
        const GLuint textureCoordinateAttributeIndex = 1;

        glVertexAttribPointer(positionAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
        glEnableVertexAttribArray(positionAttributeIndex);
        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(textureCoordinateAttributeIndex);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        primaryShaderPtr_ = new OpenGL_Shader(
            g_o_primary_vert,
            g_o_primary_frag
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

        gridShaderPtr_ = new OpenGL_Shader(
            g_o_grid_vert,
            g_o_grid_frag
        );

        wireframeShaderPtr_ = new OpenGL_Shader(
            g_o_wireframe_vert,
            g_o_wireframe_geom,
            g_o_wireframe_frag
        );
        wireframeShaderPtr_->Use();

        wireframeShaderPtr_->SetVec4("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        normalShaderPtr_ = new OpenGL_Shader(
            g_o_normal_vert,
            g_o_normal_geom,
            g_o_normal_frag
        );
        normalShaderPtr_->Use();

        normalShaderPtr_->SetFloat("u_Magnitude", s_normalMagnitude);
        normalShaderPtr_->SetVec4("u_StartColor", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
        normalShaderPtr_->SetVec4("u_EndColor", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        screenShaderPtr_ = new OpenGL_Shader(
            g_o_screen_vert,
            g_o_screen_frag
        );
        screenShaderPtr_->Use();

        screenShaderPtr_->SetInt("u_ScreenTexture", 0);

        glUseProgram(0);

        CreateScreenFramebuffers();

        GLint viewPortData[4];
        glGetIntegerv(GL_VIEWPORT, viewPortData);
        viewportX_ = viewPortData[0];
        viewportY_ = viewPortData[1];
        viewportWidth_ = viewPortData[2];
        viewportHeight_ = viewPortData[3];

        textureUnitStalenessValues_.resize(s_numberOfTextureUnits_, 1);

        pointLights_.reserve(s_numberOfPointLights_);
        spotLights_.reserve(s_numberOfSpotLights_);
    }

    OpenGL_Renderer::~OpenGL_Renderer()
    {
        windowPtr_->MakeContextCurrent();

        delete primaryShaderPtr_;
        delete gridShaderPtr_;
        delete wireframeShaderPtr_;
        delete normalShaderPtr_;
        delete screenShaderPtr_;

        DeleteAllMeshes();
        DeleteAllTextures();

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &instanceBufferId_);

        glDeleteVertexArrays(1, &batchedVertexArrayId_);
        glDeleteBuffers(1, &batchedVertexBufferId_);
        glDeleteBuffers(1, &batchedIndexBufferId_);

        CleanUpScreenFramebuffers();

        glDeleteVertexArrays(1, &gridVertexArrayId_);
        glDeleteBuffers(1, &gridVertexBufferId_);

        glDeleteVertexArrays(1, &screenVertexArrayId_);
        glDeleteBuffers(1, &screenVertexBufferId_);
    }

    void OpenGL_Renderer::SetMultisampleAntiAliasing(
        bool multisampleAntiAliasing)
    {
        windowPtr_->MakeContextCurrent();

        if (multisampleAntiAliasing != multisampleAntiAliasing_)
        {
            multisampleAntiAliasing_ = multisampleAntiAliasing;
            windowPtr_->MakeContextCurrent();

            CleanUpScreenFramebuffers();
            CreateScreenFramebuffers();
        }
    }

    void OpenGL_Renderer::SetInstanceBufferCapacity(unsigned int capacity)
    {
        windowPtr_->MakeContextCurrent();

        instanceBufferCapacity_ = capacity;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &instanceBufferId_);
        glGenBuffers(1, &instanceBufferId_);
        glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * instanceBufferCapacity_, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        for (AutoIdMap<OpenGL_Mesh*>::iterator iter = meshPtrMap_.IteratorAtBeginning();
            iter != meshPtrMap_.IteratorPastTheEnd(); ++iter)
        {
            iter->second->UpdateVertexArrayObject(instanceBufferId_);
        }
    }

    void OpenGL_Renderer::SetBatchedIndexBufferCapacity(unsigned int capacity)
    {
        windowPtr_->MakeContextCurrent();

        batchedIndexBufferCapacity_ = capacity;
        if (batchedIndexBufferCapacity_ % 3 != 0)
        {
            LOG_WARNING_F("Index Buffer Size Not Multiple Of 3");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &batchedIndexBufferId_);
        glGenBuffers(1, &batchedIndexBufferId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchedIndexBufferId_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * batchedIndexBufferCapacity_, NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGL_Renderer::SetBatchedVertexBufferCapacity(unsigned int capacity)
    {
        windowPtr_->MakeContextCurrent();

        batchedVertexBufferCapacity_ = capacity;

        // If you try to use a VAO that references a deleted VBO (i.e., a buffer
        // object ID that has been deleted), it will result in undefined
        // behavior. Attributes referencing a new buffer can be set, but I'm
        // just going to recreate the VAO as well.
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &batchedVertexArrayId_);
        glGenVertexArrays(1, &batchedVertexArrayId_);
        glBindVertexArray(batchedVertexArrayId_);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &batchedVertexBufferId_);
        glGenBuffers(1, &batchedVertexBufferId_);
        glBindBuffer(GL_ARRAY_BUFFER, batchedVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BatchedVertexData) * batchedVertexBufferCapacity_, NULL, GL_STREAM_DRAW);

        const GLuint positionAttributeIndex = 0;
        const GLuint textureCoordinateAttributeIndex = 1;
        const GLuint normalAttributeIndex = 2;
        const GLuint colorAttributeIndex = 3;
        const GLuint textureUnitAttributeIndex = 4;
        const GLuint specularUnitAttributeIndex = 5;
        const GLuint shininessAttributeIndex = 6;
        const GLuint scaleAttributeIndex = 7;
        const GLuint translationAttributeIndex = 8;
        const GLuint orientationAttributeIndex = 9;
        const GLuint litAttributeIndex = 10;

        unsigned long long attributeOffset = 0;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(positionAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureCoordinateAttributeIndex);
        attributeOffset += sizeof(glm::vec2);

        glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(normalAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(colorAttributeIndex);
        attributeOffset += sizeof(glm::vec4);

        glVertexAttribPointer(textureUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(textureUnitAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(specularUnitAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(shininessAttributeIndex);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(scaleAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(scaleAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(translationAttributeIndex);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(orientationAttributeIndex);
        attributeOffset += sizeof(glm::quat);

        glVertexAttribPointer(litAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(BatchedVertexData), (void*)attributeOffset);
        glEnableVertexAttribArray(litAttributeIndex);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGL_Renderer::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        windowPtr_->MakeContextCurrent();

        frameBufferWidth_ = width;
        frameBufferHeight_ = height;

        if (frameBufferWidth_ != 0 && frameBufferHeight_ != 0)
        {
            CleanUpScreenFramebuffers();
            CreateScreenFramebuffers();
        }
    }

    void OpenGL_Renderer::CreateTexture(
        unsigned int& textureId,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel,
        bool multisampleAntiAliasing,
        bool mipMaps)
    {
        windowPtr_->MakeContextCurrent();

        unsigned int textureUnit = (unsigned int)texturePtrMap_.Size() + 1; // reserving 0 for the screenTexture
        if (textureUnit >= s_numberOfTextureUnits_)
        {
            textureUnit = s_numberOfTextureUnits_ - 1;
        }

        OpenGL_Texture* newTexturePtr = new OpenGL_Texture(textureUnit, data, width, height, bytesPerPixel, multisampleAntiAliasing, mipMaps);
        texturePtrMap_.Add(textureId, newTexturePtr);
        textureIdToUnitBiMap_.Add(textureId, textureUnit);
    }

    bool OpenGL_Renderer::DeleteTexture(unsigned int textureId)
    {
        windowPtr_->MakeContextCurrent();

        AutoIdMap<OpenGL_Texture*>::iterator iter = texturePtrMap_.Find(textureId);
        if (iter != texturePtrMap_.IteratorPastTheEnd())
        {
            delete iter->second;
            texturePtrMap_.Erase(iter);
            textureIdToUnitBiMap_.EraseKey(textureId);
            return true;
        }

        return false;
    }

    void OpenGL_Renderer::DeleteAllTextures()
    {
        windowPtr_->MakeContextCurrent();

        for (AutoIdMap<OpenGL_Texture*>::iterator iter = texturePtrMap_.IteratorAtBeginning();
            iter != texturePtrMap_.IteratorPastTheEnd(); ++iter)
        {
            delete iter->second;
        }

        texturePtrMap_.Clear();
        textureIdToUnitBiMap_.Clear();
    }

    void OpenGL_Renderer::BeginRendering()
    {
        windowPtr_->MakeContextCurrent();

        glDepthMask(GL_TRUE);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::Clear()
    {
        windowPtr_->MakeContextCurrent();

        if (multisampleAntiAliasing_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
        }

        glClearColor(clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        redrawGrid_ = true;
    }

    void OpenGL_Renderer::ClearDepthOnly()
    {
        windowPtr_->MakeContextCurrent();

        if (multisampleAntiAliasing_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
        }

        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL_Renderer::CreateMesh(
        unsigned int& meshId,
        const MeshVertex* meshVertexPtr,
        unsigned int meshVertexCount,
        const unsigned int* meshIndexPtr,
        unsigned int meshIndexCount)
    {
        windowPtr_->MakeContextCurrent();

        OpenGL_Mesh* newMeshPtr= new OpenGL_Mesh(meshVertexPtr, meshVertexCount, meshIndexPtr, meshIndexCount);
        newMeshPtr->UpdateVertexArrayObject(instanceBufferId_);
        meshPtrMap_.Add(meshId, newMeshPtr);
    }

    bool OpenGL_Renderer::DeleteMesh(unsigned int meshId)
    {
        windowPtr_->MakeContextCurrent();

        AutoIdMap<OpenGL_Mesh*>::iterator iter = meshPtrMap_.Find(meshId);
        if (iter != meshPtrMap_.IteratorPastTheEnd())
        {
            delete iter->second;
            meshPtrMap_.Erase(iter);
            return true;
        }

        return false;
    }

    void OpenGL_Renderer::DeleteAllMeshes()
    {
        windowPtr_->MakeContextCurrent();

        for (AutoIdMap<OpenGL_Mesh*>::iterator iter = meshPtrMap_.IteratorAtBeginning();
            iter != meshPtrMap_.IteratorPastTheEnd(); ++iter)
        {
            delete iter->second;
        }

        meshPtrMap_.Clear();
    }

    bool OpenGL_Renderer::RenderMesh(
        unsigned int meshId,
        const MeshInstanceData* meshInstanceDataPtr,
        unsigned int meshInstanceCount)
    {
        if (meshInstanceCount == 0)
        {
            // Want to draw nothing, so draw nothing
            return true;
        }

        windowPtr_->MakeContextCurrent();

        AutoIdMap<OpenGL_Mesh*>::iterator iter = meshPtrMap_.Find(meshId);
        if (iter == meshPtrMap_.IteratorPastTheEnd())
        {
            LOG_ERROR_F("Mesh Id not found");
            return false;
        }

        OpenGL_Mesh* meshPtr = iter->second;

        if (!batchedVertexStagingBuffer_.empty() && !batchedIndexStagingBuffer_.empty())
        {
            RenderBatch();
        }

        for (unsigned int i = 0; i < meshInstanceCount; ++i)
        {
            if (instanceBuffer_.size() == instanceBufferCapacity_)
            {
                RenderMeshToTexture(meshPtr);
            }

            const MeshInstanceData& currentMeshInstanceData = meshInstanceDataPtr[i];

            unsigned int textureId = currentMeshInstanceData.textureId;
            unsigned int specularId = currentMeshInstanceData.specularId;

            bool getTextureFailed = false;

            float textureUnit = -1.0f;
            if (textureId != (unsigned int)-1)
            {
                int getTextureUnitResult = GetTextureUnit(textureId, textureUnit);
                if (getTextureUnitResult == 1)
                {
                    getTextureFailed = true;
                }
                else if (getTextureUnitResult == 2)
                {
                    LOG_ERROR_F("TextureId (" << textureId << ") not found");
                    return false;
                }
            }

            float specularUnit = -1.0f;
            if (specularId != (unsigned int)-1)
            {
                int getTextureUnitResult = GetTextureUnit(specularId, specularUnit);
                if (getTextureUnitResult == 1)
                {
                    getTextureFailed = true;
                }
                else if (getTextureUnitResult == 2)
                {
                    LOG_ERROR_F("SpecularId (" << specularId << ") not found");
                    return false;
                }
            }

            if (getTextureFailed)
            {
                RenderMeshToTexture(meshPtr);
                GetTextureUnit(textureId, textureUnit);
                GetTextureUnit(specularId, specularUnit);
            }

            InstanceData newInstance;
            newInstance.color = currentMeshInstanceData.color;
            newInstance.textureUnit = textureUnit;
            newInstance.specularUnit = specularUnit;
            newInstance.shininess = currentMeshInstanceData.shininess;
            newInstance.scale = currentMeshInstanceData.scale;
            newInstance.translation = currentMeshInstanceData.position;
            newInstance.orientation.x = currentMeshInstanceData.orientation.x;
            newInstance.orientation.y = currentMeshInstanceData.orientation.y;
            newInstance.orientation.z = currentMeshInstanceData.orientation.z;
            newInstance.orientation.w = currentMeshInstanceData.orientation.w;
            newInstance.lit = currentMeshInstanceData.lit;

            instanceBuffer_.push_back(newInstance);
        }

        RenderMeshToTexture(meshPtr);

        return true;
    }

    bool OpenGL_Renderer::AddMeshToBatch(
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
        bool lit)
    {
        if ((batchedVertexStagingBuffer_.size() + meshVertexCount) > batchedVertexBufferCapacity_ ||
            (batchedIndexStagingBuffer_.size() + meshIndexCount) > batchedIndexBufferCapacity_)
        {
            if (meshVertexCount > batchedVertexBufferCapacity_ ||
                meshIndexCount > batchedIndexBufferCapacity_)
            {
                LOG_ERROR_F("Mesh larger then buffer");
                return false;
            }

            RenderBatch();
        }

        bool getTextureFailed = false;

        float textureUnit = -1.0f;
        if (textureId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(textureId, textureUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                LOG_ERROR_F("TextureId (" << textureId << ") not found");
                return false;
            }
        }

        float specularUnit = -1.0f;
        if (specularId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(specularId, specularUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                LOG_ERROR_F("SpecularId (" << specularId << ") not found");
                return false;
            }
        }

        if (getTextureFailed)
        {
            RenderBatch();
            GetTextureUnit(textureId, textureUnit);
            GetTextureUnit(specularId, specularUnit);
        }

        unsigned int vertexBufferOffset = (unsigned int)batchedVertexStagingBuffer_.size();

        for (size_t j = 0; j < meshVertexCount; ++j)
        {
            const MeshVertex& currentMeshVertex = meshVertexPtr[j];

            BatchedVertexData newVertex;
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

            batchedVertexStagingBuffer_.push_back(newVertex);
        }

        for (unsigned int j = 0; j < meshIndexCount; ++j)
        {
            batchedIndexStagingBuffer_.push_back(vertexBufferOffset + meshIndexPtr[j]);
        }

        return true;
    }

    void OpenGL_Renderer::RenderBatch()
    {
        windowPtr_->MakeContextCurrent();

        RenderBatchToTexture();
    }

    void OpenGL_Renderer::FinishRendering()
    {
        windowPtr_->MakeContextCurrent();

        if (!batchedVertexStagingBuffer_.empty() && !batchedIndexStagingBuffer_.empty())
        {
            RenderBatchToTexture();
        }

        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ZERO, GL_ZERO, GL_ONE);

        RenderTextureToScreen();
    }

    void OpenGL_Renderer::SwapBuffers()
    {
        windowPtr_->SwapBuffers();
    }

    // protected ---------------------------------------------------------------

    void OpenGL_Renderer::CreateGridBufferAndArray()
    {
        struct GridVertexData
        {
            glm::vec3 position;
            glm::vec4 color;
        };
        std::vector<GridVertexData> gridVertices;

        for (float i = s_gridHalfExtents_; i > 0; i -= s_minorGridIncement_)
        {
            if (i + 0.00001f > 0.0f && i - 0.00001f < 0.0f)
            {
                continue;
            }

            float alpha = 0.4f;

            float fmodRemainder = std::fmodf(i, s_majorGridIncement_);
            if ((fmodRemainder + 0.00001f > 0.0f && fmodRemainder - 0.00001f < 0.0f) ||
                (fmodRemainder + 0.00001f > s_majorGridIncement_ && fmodRemainder - 0.00001f < s_majorGridIncement_))
            {
                alpha = 1.0f;
            }

            // xy plane +x lines
            gridVertices.push_back({
                glm::vec3(i, -s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, -0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, 0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane -x lines
            gridVertices.push_back({
                glm::vec3(-i, -s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, -0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, 0.00001, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, s_gridHalfExtents_, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane +y lines
            gridVertices.push_back({
                glm::vec3(-s_gridHalfExtents_, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-0.00001, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.00001, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(s_gridHalfExtents_, i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xy plane -y lines
            gridVertices.push_back({
                glm::vec3(-s_gridHalfExtents_, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-0.00001, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.00001, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(s_gridHalfExtents_, -i, 0.0f),
                glm::vec4(1.0f, 1.0f, 0.0f, alpha)
                });

            // xz plane +x lines
            gridVertices.push_back({
                glm::vec3(i, 0.0f, -s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, 0.0f, -0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, 0.0f, 0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(i, 0.0f, s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane -x lines
            gridVertices.push_back({
                glm::vec3(-i, 0.0f, -s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, 0.0f, -0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, 0.0f, 0.00001),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-i, 0.0f, s_gridHalfExtents_),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane +z lines
            gridVertices.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-0.00001, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.00001, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // xz plane -x lines
            gridVertices.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(-0.00001, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.00001, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, -i),
                glm::vec4(1.0f, 0.0f, 1.0f, alpha)
                });

            // yz plane +y lines
            gridVertices.push_back({
                glm::vec3(0.0f, i, -s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, i, -0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, i, 0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, i, s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane -y lines
            gridVertices.push_back({
                glm::vec3(0.0f, -i, -s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, -i, -0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, -i, 0.00001),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, -i, s_gridHalfExtents_),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane +z lines
            gridVertices.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, -0.00001, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, 0.00001, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });

            // yz plane -z lines
            gridVertices.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, -0.00001, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, 0.00001, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
            gridVertices.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, -i),
                glm::vec4(0.0f, 1.0f, 1.0f, alpha)
                });
        }

        // x axis
        gridVertices.push_back({
                glm::vec3(-s_gridHalfExtents_, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(-0.00001, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.00001, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(s_gridHalfExtents_, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
            });

        // y axis
        gridVertices.push_back({
                glm::vec3(0.0f, -s_gridHalfExtents_, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.0f, -0.00001, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.0f, 0.00001, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.0f, s_gridHalfExtents_, 0.0f),
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
            });

        // z axis
        gridVertices.push_back({
                glm::vec3(0.0f, 0.0f, -s_gridHalfExtents_),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.0f, 0.0f, -0.00001),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVertices.push_back({
                glm::vec3(0.0f, 0.0f, 0.00001),
                glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });
        gridVertices.push_back({
            glm::vec3(0.0f, 0.0f, s_gridHalfExtents_),
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
            });

        gridVertexCount_ = (unsigned int)gridVertices.size();

        glGenVertexArrays(1, &gridVertexArrayId_);
        glBindVertexArray(gridVertexArrayId_);

        glGenBuffers(1, &gridVertexBufferId_);
        glBindBuffer(GL_ARRAY_BUFFER, gridVertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, gridVertexCount_ * sizeof(GridVertexData), &gridVertices.front(), GL_STATIC_DRAW);

        const GLuint positionAttributeIndex = 0;
        const GLuint colorCoordinateAttributeIndex = 1;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(GridVertexData), (void*)0);
        glEnableVertexAttribArray(positionAttributeIndex);
        glVertexAttribPointer(colorCoordinateAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(GridVertexData), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(colorCoordinateAttributeIndex);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGL_Renderer::CreateScreenFramebuffers()
    {
        glActiveTexture(GL_TEXTURE0);

        if (multisampleAntiAliasing_)
        {
            glGenFramebuffers(1, &msaaFrameBufferId_);
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);

            glGenTextures(1, &msaaTextureId_);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTextureId_);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, frameBufferWidth_, frameBufferHeight_, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTextureId_, 0);
        }

        glGenFramebuffers(1, &rttFrameBufferId_);
        glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);

        glGenTextures(1, &rttTextureId_);
        glBindTexture(GL_TEXTURE_2D, rttTextureId_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameBufferWidth_, frameBufferHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rttTextureId_, 0);

        glGenRenderbuffers(1, &renderBufferId_);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId_);

        if (multisampleAntiAliasing_)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, frameBufferWidth_, frameBufferHeight_);
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId_);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBufferWidth_, frameBufferHeight_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferId_);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR_F("Framebuffer not complete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void OpenGL_Renderer::CleanUpScreenFramebuffers()
    {
        if (multisampleAntiAliasing_)
        {
            glDeleteTextures(1, &msaaTextureId_);
            glDeleteFramebuffers(1, &msaaFrameBufferId_);
        }

        glDeleteTextures(1, &rttTextureId_);
        glDeleteFramebuffers(1, &rttFrameBufferId_);

        glDeleteRenderbuffers(1, &renderBufferId_);
    }

    bool OpenGL_Renderer::BindTexture(
        unsigned int textureId,
        unsigned int textureUnit)
    {
        if (textureUnit < s_numberOfTextureUnits_ && textureUnit > 0) // reserving 0 for the screenTexture
        {
            AutoIdMap<OpenGL_Texture*>::iterator iter = texturePtrMap_.Find(textureId);
            if (iter != texturePtrMap_.IteratorPastTheEnd())
            {
                iter->second->Bind(textureUnit);
                textureIdToUnitBiMap_.Add(textureId, textureUnit);
                return true;
            }
        }

        return false;
    }

    int OpenGL_Renderer::GetTextureUnit(unsigned int textureId, float& textureUnit)
    {
        UniqueBiMap<unsigned int, unsigned int>::iterator iter = textureIdToUnitBiMap_.FindKey(textureId);
        if (iter != textureIdToUnitBiMap_.IteratorPastTheEnd())
        {
            unsigned int textureUnit_uint = iter->second;
            textureUnitStalenessValues_[textureUnit_uint] = 0;
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
                    textureUnitStalenessValues_[newTextureUnit] = 0;
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

    bool OpenGL_Renderer::GetStalestTextureUnit(unsigned int& textureUnit) const
    {
        unsigned int stalestValue = 0;

        for (unsigned int i = 1; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            if (textureUnitStalenessValues_[i] > stalestValue)
            {
                textureUnit = i;
                stalestValue = textureUnitStalenessValues_[i];
            }
        }

        return stalestValue > 0;
    }

    void OpenGL_Renderer::IncreaseTectureUnitStaleness()
    {
        for (size_t i = 1 ; i < s_numberOfTextureUnits_; ++i) // reserving 0 for the screenTexture
        {
            textureUnitStalenessValues_[i]++;
        }
    }

    void OpenGL_Renderer::RenderMeshToTexture(OpenGL_Mesh* meshPtr)
    {
        glViewport(
            GLint(frameBufferWidth_ * cameraViewportX_),
            GLint(frameBufferHeight_ * cameraViewportY_),
            GLsizei(frameBufferWidth_ * cameraViewportWidth_),
            GLsizei(frameBufferHeight_ * cameraViewportHeight_)
        );

        if (multisampleAntiAliasing_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
        }

        if (depthTesting_)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (s_drawGrid)
        {
            DrawGrid();
        }

        UsePrimaryShaderAndUpdateItsUniforms();

        glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId_);
        if (!instanceBuffer_.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData) * instanceBuffer_.size(), &instanceBuffer_[0]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (!s_cullBackface)
        {
            glCullFace(GL_FRONT);
            meshPtr->Render((unsigned int)instanceBuffer_.size());
        }
        glCullFace(GL_BACK);
        meshPtr->Render((unsigned int)instanceBuffer_.size());

        if (s_drawWireframe)
        {
            wireframeShaderPtr_->Use();

            wireframeShaderPtr_->SetMat4("u_View", viewMatrix_);
            wireframeShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            wireframeShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

            meshPtr->Render((unsigned int)instanceBuffer_.size());
        }

        if (s_drawNormals)
        {
            normalShaderPtr_->Use();

            normalShaderPtr_->SetMat4("u_View", viewMatrix_);
            normalShaderPtr_->SetMat4("u_Projection", projectionMatrix_);

            meshPtr->Render((unsigned int)instanceBuffer_.size());
        }

        IncreaseTectureUnitStaleness();

        instanceBuffer_.clear();
    }

    void OpenGL_Renderer::RenderBatchToTexture()
    {
        glViewport(
            GLint(frameBufferWidth_ * cameraViewportX_),
            GLint(frameBufferHeight_ * cameraViewportY_),
            GLsizei(frameBufferWidth_ * cameraViewportWidth_),
            GLsizei(frameBufferHeight_ * cameraViewportHeight_)
        );

        if (multisampleAntiAliasing_)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBufferId_);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, rttFrameBufferId_);
        }

        if (depthTesting_)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (s_drawGrid)
        {
            DrawGrid();
        }

        UsePrimaryShaderAndUpdateItsUniforms();

        glBindVertexArray(batchedVertexArrayId_);
        glBindBuffer(GL_ARRAY_BUFFER, batchedVertexBufferId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchedIndexBufferId_);

        if (!batchedVertexStagingBuffer_.empty() && !batchedIndexStagingBuffer_.empty())
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BatchedVertexData) * batchedVertexStagingBuffer_.size(), &batchedVertexStagingBuffer_[0]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * batchedIndexStagingBuffer_.size(), &batchedIndexStagingBuffer_[0]);
        }

        // The VBO doesn't need to be bound to draw with it
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (!s_cullBackface)
        {
            glCullFace(GL_FRONT);
            glDrawElements(GL_TRIANGLES, (GLsizei)batchedIndexStagingBuffer_.size(), GL_UNSIGNED_INT, 0);
        }
        glCullFace(GL_BACK);
        glDrawElements(GL_TRIANGLES, (GLsizei)batchedIndexStagingBuffer_.size(), GL_UNSIGNED_INT, 0);


        if (s_drawWireframe)
        {
            wireframeShaderPtr_->Use();

            wireframeShaderPtr_->SetMat4("u_View", viewMatrix_);
            wireframeShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
            wireframeShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

            glDrawElements(GL_TRIANGLES, (GLsizei)batchedIndexStagingBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        if (s_drawNormals)
        {
            normalShaderPtr_->Use();

            normalShaderPtr_->SetMat4("u_View", viewMatrix_);
            normalShaderPtr_->SetMat4("u_Projection", projectionMatrix_);

            glDrawElements(GL_TRIANGLES, (GLsizei)batchedIndexStagingBuffer_.size(), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Clear local buffers
        // ---------------------------------------------------------------------

        IncreaseTectureUnitStaleness();

        batchedVertexStagingBuffer_.clear();
        batchedIndexStagingBuffer_.clear();
    }

    void OpenGL_Renderer::DrawGrid()
    {
        redrawGrid_ = false;

        gridShaderPtr_->Use();
        gridShaderPtr_->SetMat4("u_View", viewMatrix_);
        gridShaderPtr_->SetMat4("u_Projection", projectionMatrix_);
        gridShaderPtr_->SetVec3("u_ViewPosition", viewPosition_);

        glBindVertexArray(gridVertexArrayId_);

        glDrawArrays(GL_LINES, 0, gridVertexCount_);

        glBindVertexArray(0);
    }

    void OpenGL_Renderer::RenderTextureToScreen()
    {
        if (multisampleAntiAliasing_)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFrameBufferId_);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rttFrameBufferId_);
            glBlitFramebuffer(0, 0, frameBufferWidth_, frameBufferHeight_, 0, 0, frameBufferWidth_, frameBufferHeight_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }

        glViewport(viewportX_, viewportY_, viewportWidth_, viewportHeight_);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        screenShaderPtr_->Use();

        glBindVertexArray(screenVertexArrayId_);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
    }

    void OpenGL_Renderer::UsePrimaryShaderAndUpdateItsUniforms()
    {
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
            primaryShaderPtr_->SetVec3((stringPrefix + stringI + "].position").c_str(), pointLights_[i].position);
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
    }

    const unsigned int OpenGL_Renderer::s_numberOfTextureUnits_ = 16;

    const unsigned int OpenGL_Renderer::s_numberOfPointLights_ = 8;
    const unsigned int OpenGL_Renderer::s_numberOfSpotLights_ = 4;

    const bool OpenGL_Renderer::s_cullBackface = true;
    const bool OpenGL_Renderer::s_drawWireframe = false;
    const bool OpenGL_Renderer::s_drawNormals = false;
    const bool OpenGL_Renderer::s_drawGrid = false;
    const float OpenGL_Renderer::s_normalMagnitude = 0.16f;

    const float OpenGL_Renderer::s_minorGridIncement_ = 0.1f;
    const float OpenGL_Renderer::s_majorGridIncement_ = 1.0f;
    const float OpenGL_Renderer::s_gridHalfExtents_ = 10.0f;
}